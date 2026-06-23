#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/cpuset.h>
#include <linux/hrtimer.h>
#include <linux/rwsem.h>
#include <linux/completion.h>

MODULE_LICENSE("GPL");

static struct proc_dir_entry *proc_entry;

static DEFINE_MUTEX(mutex_a);
static DEFINE_MUTEX(mutex_b);
static DEFINE_SPINLOCK(spin_a);
static DEFINE_SPINLOCK(spin_b);

static struct task_struct *thread_ab1;
static struct task_struct *thread_ab2;
static struct task_struct *thread_self;
static struct task_struct *thread_atomic;
static struct task_struct *thread_atomic_waiter;
static struct task_struct *irq_deadlock_thread;

static DECLARE_RWSEM(rwsem_x);
static DECLARE_COMPLETION(never_complete);

static int scenario_running;

static char result_buf[4096];
static int result_len;

static void result_append(const char *fmt, ...)
{
	va_list args;
	int remaining = sizeof(result_buf) - result_len;
	if (remaining <= 0)
		return;
	va_start(args, fmt);
	result_len += vsnprintf(result_buf + result_len, remaining, fmt, args);
	va_end(args);
}

/*
 * Scenario 1: AB-BA Deadlock (Lock Order Inversion)
 */
static int abba_thread1(void *data)
{
	result_append("[AB-BA] Thread 1: trying to lock mutex_a...\n");
	mutex_lock(&mutex_a);
	result_append("[AB-BA] Thread 1: acquired mutex_a, now trying to lock mutex_b...\n");
	msleep(500);
	result_append("[AB-BA] Thread 1: waiting for mutex_b (potential deadlock)...\n");
	mutex_lock(&mutex_b);
	result_append("[AB-BA] Thread 1: acquired mutex_b (should not reach here if deadlocked)\n");
	mutex_unlock(&mutex_b);
	mutex_unlock(&mutex_a);
	return 0;
}

static int abba_thread2(void *data)
{
	result_append("[AB-BA] Thread 2: trying to lock mutex_b...\n");
	mutex_lock(&mutex_b);
	result_append("[AB-BA] Thread 2: acquired mutex_b, now trying to lock mutex_a...\n");
	msleep(500);
	result_append("[AB-BA] Thread 2: waiting for mutex_a (potential deadlock)...\n");
	mutex_lock(&mutex_a);
	result_append("[AB-BA] Thread 2: acquired mutex_a (should not reach here if deadlocked)\n");
	mutex_unlock(&mutex_a);
	mutex_unlock(&mutex_b);
	return 0;
}

static void run_abba_deadlock(void)
{
	result_len = 0;
	result_append("=== Scenario 1: AB-BA Deadlock ===\n");
	result_append("Thread 1: lock(A) -> lock(B)\n");
	result_append("Thread 2: lock(B) -> lock(A)\n\n");

	thread_ab1 = kthread_run(abba_thread1, NULL, "abba_t1");
	thread_ab2 = kthread_run(abba_thread2, NULL, "abba_t2");
}

/*
 * Scenario 2: AB-BA Deadlock with Spinlocks
 */
static int spin_abba_thread1(void *data)
{
	unsigned long flags;

	spin_lock_irqsave(&spin_a, flags);
	result_append("[Spin-AB-BA] Thread 1: acquired spin_a, now trying to lock spin_b...\n");
	mdelay(1000);
	result_append("[Spin-AB-BA] Thread 1: waiting for spin_b (deadlock)...\n");
	spin_lock(&spin_b);
	spin_unlock(&spin_b);
	spin_unlock_irqrestore(&spin_a, flags);
	return 0;
}

static int spin_abba_thread2(void *data)
{
	unsigned long flags;

	mdelay(500);
	spin_lock_irqsave(&spin_b, flags);
	result_append("[Spin-AB-BA] Thread 2: acquired spin_b, now trying to lock spin_a...\n");
	mdelay(1000);
	result_append("[Spin-AB-BA] Thread 2: waiting for spin_a (deadlock)...\n");
	spin_lock(&spin_a);
	spin_unlock(&spin_a);
	spin_unlock_irqrestore(&spin_b, flags);
	return 0;
}

static struct task_struct *spin_thread1;
static struct task_struct *spin_thread2;

static void run_spin_abba_deadlock(void)
{
	result_len = 0;
	result_append("=== Scenario 2: AB-BA Deadlock with Spinlocks ===\n");

	spin_thread1 = kthread_run(spin_abba_thread1, NULL, "spin_abba_t1");
	spin_thread2 = kthread_run(spin_abba_thread2, NULL, "spin_abba_t2");
}

/*
 * Scenario 3: Sleeping While Holding a Spinlock -> Real Deadlock (same CPU)
 */
static DECLARE_COMPLETION(atomic_sleep_held);

static int atomic_sleep_thread(void *data)
{
	spin_lock(&spin_b);
	result_append("[Sleep-in-Atomic] Thread A: acquired spin_b, signaling Thread B...\n");
	complete(&atomic_sleep_held);
	result_append("[Sleep-in-Atomic] Thread A: schedule() yields CPU, Thread B spins on same CPU -> deadlock!\n");
	msleep(2000);
	result_append("[Sleep-in-Atomic] Thread A: woke up, releasing spin_b\n");
	spin_unlock(&spin_b);
	return 0;
}

static int atomic_sleep_waiter(void *data)
{
	wait_for_completion(&atomic_sleep_held);
	result_append("[Sleep-in-Atomic] Thread B: trying to acquire spin_b on same CPU...\n");
	spin_lock(&spin_b);
	spin_unlock(&spin_b);
	return 0;
}

static void run_atomic_sleep(void)
{
	struct cpumask single_cpu;

	result_len = 0;
	result_append("=== Scenario 3: Sleeping While Holding Spinlock ===\n");

	reinit_completion(&atomic_sleep_held);

	cpumask_clear(&single_cpu);
	cpumask_set_cpu(1, &single_cpu);

	thread_atomic = kthread_create(atomic_sleep_thread, NULL, "atomic_sleep");
	if (IS_ERR(thread_atomic)) {
		thread_atomic = NULL;
		return;
	}
	set_cpus_allowed_ptr(thread_atomic, &single_cpu);

	thread_atomic_waiter = kthread_create(atomic_sleep_waiter, NULL, "atomic_waiter");
	if (IS_ERR(thread_atomic_waiter)) {
		thread_atomic_waiter = NULL;
		return;
	}
	set_cpus_allowed_ptr(thread_atomic_waiter, &single_cpu);

	wake_up_process(thread_atomic);
	wake_up_process(thread_atomic_waiter);
}

/*
 * Scenario 4: Self-Deadlock (Recursive Spinlock Acquisition)
 */
static int self_deadlock_thread(void *data)
{
	result_append("[Self-Deadlock] Thread: acquiring spin_a...\n");
	spin_lock(&spin_a);
	result_append("[Self-Deadlock] Thread: acquired spin_a, now trying to acquire spin_a again...\n");
	spin_lock(&spin_a);
	spin_unlock(&spin_a);
	spin_unlock(&spin_a);
	return 0;
}

static void run_self_deadlock(void)
{
	result_len = 0;
	result_append("=== Scenario 4: Self-Deadlock (Recursive Spinlock) ===\n");

	thread_self = kthread_run(self_deadlock_thread, NULL, "self_dead");
}

/*
 * Scenario 5: Interrupt-Context Deadlock
 */
static DEFINE_RAW_SPINLOCK(spin_irq);
static struct hrtimer irq_deadlock_timer;

static enum hrtimer_restart irq_deadlock_timer_fn(struct hrtimer *timer)
{
	result_append("[IRQ-Deadlock] Callback fired at jiffies=%lu\n", jiffies);
	result_append("[IRQ-Deadlock] hrtimer fired in hard IRQ context!\n");
	result_append("[IRQ-Deadlock] IRQ handler: trying spin_lock(&spin_irq)...\n");
	result_append("[IRQ-Deadlock] Not trigger when interrupts disabled...\n");
	raw_spin_lock(&spin_irq);
	result_append("[IRQ-Deadlock] IRQ handler: acquired spin_irq\n");
	raw_spin_unlock(&spin_irq);
	return HRTIMER_NORESTART;
}

static int irq_deadlock_fn(void *data)
{
	unsigned long flags;
	struct cpumask single_cpu;
	ktime_t delay;

	cpumask_clear(&single_cpu);
	cpumask_set_cpu(1, &single_cpu);
	set_cpus_allowed_ptr(current, &single_cpu);

	raw_spin_lock_irqsave(&spin_irq, flags);
	result_append("[IRQ-Deadlock] Thread: acquired spin_irq, interrupts disabled on this CPU.\n");

	hrtimer_init(&irq_deadlock_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED_HARD);
	irq_deadlock_timer.function = irq_deadlock_timer_fn;

	delay = ms_to_ktime(100);
	hrtimer_start(&irq_deadlock_timer, delay, HRTIMER_MODE_REL_PINNED_HARD);
	result_append("[IRQ-Deadlock] Timer started at jiffies=%lu\n", jiffies);
	result_append("[IRQ-Deadlock] Thread: started hrtimer (100ms), interrupts disabled -> timer won't fire here.\n");
	result_append("[IRQ-Deadlock] Thread: hrtimer will fire after unlock, when interrupts are re-enabled.\n");

	mdelay(5000);

	result_append("[IRQ-Deadlock] Thread: releasing spin_irq and restoring interrupts\n");
	raw_spin_unlock_irqrestore(&spin_irq, flags);
	hrtimer_cancel(&irq_deadlock_timer);
	return 0;
}

static void run_irq_deadlock(void)
{
	result_len = 0;
	result_append("=== Scenario 5: Interrupt-Context Deadlock ===\n");

	irq_deadlock_thread = kthread_run(irq_deadlock_fn, NULL, "irq_dead");
}

static int deadlock_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s", result_buf);
	return 0;
}

static int deadlock_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, deadlock_proc_show, NULL);
}

static ssize_t deadlock_proc_write(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos)
{
	char cmd[32];

	if (count >= sizeof(cmd))
		return -EINVAL;
	if (copy_from_user(cmd, buf, count))
		return -EFAULT;
	cmd[count] = '\0';

	if (cmd[count - 1] == '\n')
		cmd[count - 1] = '\0';

	if (scenario_running) {
		result_len = 0;
		result_append("A scenario is already running. Please wait.\n");
		return count;
	}

	scenario_running = 1;

	if (strcmp(cmd, "1") == 0 || strcmp(cmd, "abba") == 0) {
		run_abba_deadlock();
	} else if (strcmp(cmd, "2") == 0 || strcmp(cmd, "spin_abba") == 0) {
		run_spin_abba_deadlock();
	} else if (strcmp(cmd, "3") == 0 || strcmp(cmd, "sleep") == 0) {
		run_atomic_sleep();
	} else if (strcmp(cmd, "4") == 0 || strcmp(cmd, "self") == 0) {
		run_self_deadlock();
	} else if (strcmp(cmd, "5") == 0 || strcmp(cmd, "irq") == 0) {
		run_irq_deadlock();
	} else if (strcmp(cmd, "help") == 0) {
		result_len = 0;
		result_append("Available deadlock scenarios:\n");
		result_append("  1 or abba       - AB-BA mutex deadlock (lock order inversion)\n");
		result_append("  2 or spin_abba  - AB-BA spinlock deadlock\n");
		result_append("  3 or sleep      - Sleeping while holding spinlock -> real deadlock\n");
		result_append("  4 or self       - Self-deadlock (recursive spinlock)\n");
		result_append("  5 or irq        - Interrupt-context deadlock (spin_lock vs IRQ)\n");
		result_append("\nWARNING: These scenarios WILL cause real deadlocks!\n");
		result_append("Only use on test systems. Your kernel may hang.\n");
		scenario_running = 0;
	} else {
		result_len = 0;
		result_append("Unknown command. Write 'help' for available scenarios.\n");
		scenario_running = 0;
	}

	return count;
}

static const struct proc_ops deadlock_proc_ops = {
	.proc_open	= deadlock_proc_open,
	.proc_read	= seq_read,
	.proc_write	= deadlock_proc_write,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init deadlock_demo_init(void)
{
	proc_entry = proc_create("deadlock_demo", 0666, NULL, &deadlock_proc_ops);
	if (!proc_entry) {
		pr_err("deadlock_demo: failed to create /proc entry\n");
		return -ENOMEM;
	}

	result_len = 0;
	result_append("Deadlock Demo Module Loaded\n");
	result_append("===========================\n");
	result_append("Write to /proc/deadlock_demo to trigger scenarios:\n");
	result_append("  1 or abba       - AB-BA mutex deadlock\n");
	result_append("  2 or self       - Self-deadlock (recursive spinlock)\n");
	result_append("  3 or sleep      - Sleeping while holding spinlock\n");
	result_append("  4 or spin_abba  - AB-BA spinlock deadlock\n");
	result_append("  5 or irq        - Interrupt-context deadlock\n");
	result_append("\nWARNING: These will cause REAL deadlocks!\n");
	result_append("Only use on test systems. Your kernel may hang.\n");

	pr_info("deadlock_demo: module loaded, use /proc/deadlock_demo\n");
	return 0;
}

static void __exit deadlock_demo_exit(void)
{
	if (thread_ab1)
		kthread_stop(thread_ab1);
	if (thread_ab2)
		kthread_stop(thread_ab2);
	if (thread_self)
		kthread_stop(thread_self);
	if (thread_atomic)
		kthread_stop(thread_atomic);
	if (thread_atomic_waiter)
		kthread_stop(thread_atomic_waiter);
	if (spin_thread1)
		kthread_stop(spin_thread1);
	if (spin_thread2)
		kthread_stop(spin_thread2);
	if (irq_deadlock_thread)
		kthread_stop(irq_deadlock_thread);

	proc_remove(proc_entry);
	pr_info("deadlock_demo: module unloaded\n");
}

module_init(deadlock_demo_init);
module_exit(deadlock_demo_exit);
