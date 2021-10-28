#include <linux/module.h>

static int __init start_hello(void)
{
    printk(KERN_ALERT "Hello, world from the Linux kernel!\n");
    return 0;
}

static void __exit end_hello(void)
{
    printk(KERN_ALERT "Short is the life of an LKM\n");
}

module_init(start_hello);
module_exit(end_hello);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ryan Lawlor");
MODULE_DESCRIPTION("This module demonstrates a basic LKM");