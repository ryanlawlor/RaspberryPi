#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init start_hello(void)
{
    printk(KERN_ALERT "Hello, world from the Linux kernel\n");
    return 0;
}

static void __exit end_hello(void)
{
    printk(KERN_ALERT "Short is the life of an LKM\n");
}

module_init(start_hello);
module_init(end_hello);
