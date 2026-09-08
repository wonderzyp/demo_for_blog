#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/of.h>

static int count;

static irqreturn_t demo_handler(int irq, void *dev_id)
{
    count++;
    pr_info("[gpio-irq-demo] IRQ %d fired, count=%d\n", irq, count);
    return IRQ_HANDLED;
}

static int demo_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct gpio_desc *desc;
    int irq, ret;

    /* 从 DT 的 gpios 属性解析，内部自动 request + 设方向 */
    desc = devm_gpiod_get(dev, "irq", GPIOD_IN);
    if (IS_ERR(desc))
        return PTR_ERR(desc);

    irq = gpiod_to_irq(desc);
    if (irq < 0)
        return irq;

    ret = devm_request_irq(dev, irq, demo_handler, IRQF_TRIGGER_RISING,
                           "gpio-irq-demo", desc);
    if (ret)
        return ret;

    dev_info(dev, "GPIO -> IRQ %d (RISING) registered\n", irq);
    return 0;
}

static const struct of_device_id demo_ids[] = {
    { .compatible = "linux,gpio-irq-demo" },   /* 对应 dts 里的 compatible */
    { }
};
MODULE_DEVICE_TABLE(of, demo_ids);

static struct platform_driver demo_driver = {
    .driver = {
        .name = "gpio-irq-demo",
        .of_match_table = demo_ids,
    },
    .probe = demo_probe,
};
module_platform_driver(demo_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PL061 GPIO irq demo (DT + platform driver)");