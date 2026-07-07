#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/atomic.h>

#define DEVICE_NAME "poll_select_demo"

static DECLARE_WAIT_QUEUE_HEAD(demo_waitq);
static struct timer_list demo_timer;

static atomic_t data_ready = ATOMIC_INIT(0);
static atomic_t event_count = ATOMIC_INIT(0);

static void demo_timer_fn(struct timer_list *timer)
{
    atomic_inc(&event_count);
    atomic_set(&data_ready, 1);

    pr_info("poll_select_demo: data ready, event_count=%d\n",
        atomic_read(&event_count));

    wake_up_interruptible(&demo_waitq);

    mod_timer(&demo_timer, jiffies + msecs_to_jiffies(5000));
}

static ssize_t demo_read(struct file *file, char __user *buf,
             size_t count, loff_t *ppos)
{
    char kbuf[128];
    int len;
    int cnt;

    for (;;) {
        if (atomic_xchg(&data_ready, 0))
            break;

        if (file->f_flags & O_NONBLOCK)
            return -EAGAIN;

        if (wait_event_interruptible(demo_waitq,
                         atomic_read(&data_ready)))
            return -ERESTARTSYS;
    }

    cnt = atomic_read(&event_count);
    len = snprintf(kbuf, sizeof(kbuf),
               "kernel event %d from /dev/%s\n",
               cnt, DEVICE_NAME);

    if (count < len)
        len = count;

    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    return len;
}

static __poll_t demo_poll(struct file *file, poll_table *wait)
{
    __poll_t mask = 0;

    /*
     * poll_wait() does not block here.
     * It only registers current process into demo_waitq.
     * Later wake_up_interruptible(&demo_waitq) wakes select/poll users.
     */
    poll_wait(file, &demo_waitq, wait);

    if (atomic_read(&data_ready))
        mask |= POLLIN | POLLRDNORM;

    return mask;
}

static int demo_open(struct inode *inode, struct file *file)
{
    pr_info("poll_select_demo: open\n");
    return 0;
}

static int demo_release(struct inode *inode, struct file *file)
{
    pr_info("poll_select_demo: release\n");
    return 0;
}

static const struct file_operations demo_fops = {
    .owner		= THIS_MODULE,
    .open		= demo_open,
    .release	= demo_release,
    .read		= demo_read,
    .poll		= demo_poll,
};

static struct miscdevice demo_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &demo_fops,
};

static int __init demo_init(void)
{
    int ret;

    ret = misc_register(&demo_miscdev);
    if (ret)
        return ret;

    timer_setup(&demo_timer, demo_timer_fn, 0);
    mod_timer(&demo_timer, jiffies + msecs_to_jiffies(1000));

    pr_info("poll_select_demo: loaded, device=/dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit demo_exit(void)
{
    del_timer_sync(&demo_timer);
    misc_deregister(&demo_miscdev);

    pr_info("poll_select_demo: unloaded\n");
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Demo kernel module for poll/select/epoll cooperation");