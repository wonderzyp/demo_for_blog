#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <linux/slab.h>

#define PROC_NAME "waitqueue_demo"

static struct task_struct *waiter_task;
static struct proc_dir_entry *proc_entry;

static wait_queue_head_t demo_wq;
static atomic_t event_flag = ATOMIC_INIT(0);
static atomic_t wake_count = ATOMIC_INIT(0);

static int waiter_fn(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("waitqueue_demo: thread sleeping\n");

        wait_event_interruptible(
            demo_wq,
            atomic_read(&event_flag) || kthread_should_stop()
        );

        if (kthread_should_stop())
            break;

        if (atomic_xchg(&event_flag, 0)) {
            atomic_inc(&wake_count);
            pr_info("waitqueue_demo: thread awakened, wake_count=%d\n",
                atomic_read(&wake_count));
        }
    }

    pr_info("waitqueue_demo: thread exiting\n");
    return 0;
}

static ssize_t demo_read(struct file *file, char __user *buf,
             size_t count, loff_t *ppos)
{
    char tmp[128];
    int len;

    len = scnprintf(tmp, sizeof(tmp),
            "event_flag=%d wake_count=%d\n",
            atomic_read(&event_flag),
            atomic_read(&wake_count));

    return simple_read_from_buffer(buf, count, ppos, tmp, len);
}

static ssize_t demo_write(struct file *file, const char __user *buf,
              size_t count, loff_t *ppos)
{
    char kbuf[32];

    if (count == 0)
        return 0;

    if (count >= sizeof(kbuf))
        count = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;

    kbuf[count] = '\0';

    atomic_set(&event_flag, 1);
    wake_up_interruptible(&demo_wq);

    pr_info("waitqueue_demo: userspace triggered wakeup, input=%s\n", kbuf);
    return count;
}

static const struct proc_ops demo_proc_ops = {
    .proc_read  = demo_read,
    .proc_write = demo_write,
};

static int __init waitqueue_demo_init(void)
{
    init_waitqueue_head(&demo_wq);

    proc_entry = proc_create(PROC_NAME, 0666, NULL, &demo_proc_ops);
    if (!proc_entry)
        return -ENOMEM;

    waiter_task = kthread_run(waiter_fn, NULL, "waitqueue_demo_thread");
    if (IS_ERR(waiter_task)) {
        proc_remove(proc_entry);
        return PTR_ERR(waiter_task);
    }

    pr_info("waitqueue_demo: module loaded\n");
    pr_info("waitqueue_demo: use 'echo 1 > /proc/%s' to wake thread\n", PROC_NAME);
    return 0;
}

static void __exit waitqueue_demo_exit(void)
{
    if (waiter_task) {
        wake_up_interruptible(&demo_wq);
        kthread_stop(waiter_task);
    }

    if (proc_entry)
        proc_remove(proc_entry);

    pr_info("waitqueue_demo: module unloaded\n");
}

module_init(waitqueue_demo_init);
module_exit(waitqueue_demo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux waitqueue demo module");