#include <linux/module.h>
#include <linux/namei.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/parser.h>

static struct file_system_type hollyfs_fs_type = {
    .owner = THIS_MODULE,
    .name = "hollyfs",
    // .get_sb = holly_get_sb,
    .kill_sb = kill_block_super,
    .fs_flags = FS_REQUIRES_DEV,
};

static int __init start_hello(void)
{
    int ret;
    printk(KERN_ALERT "Hello, world from the Linux kernel!\n");
    ret = register_filesystem(&hollyfs_fs_type);
    if (likely(0 == ret))
    {
        printk(KERN_INFO "Successfully registered hollyfs\n");
    }
    else
    {
        printk(KERN_ERR "Failed to register hollyfs. Error code: %d\n", ret);
    }
    return ret;
}

static void __exit end_hello(void)
{
    int ret;
    printk(KERN_ALERT "Short is the life of an LKM\n");
    ret = unregister_filesystem(&hollyfs_fs_type);
    if (likely(ret == 0))
    {
        printk(KERN_INFO "Successfully unregistered hollyfs\n");
    }
    else
    {
        printk(KERN_ERR "Failed to unregister hollyfs. Error code: %d\n", ret);
    }
}

module_init(start_hello);
module_exit(end_hello);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ryan Lawlor");
MODULE_DESCRIPTION("This module demonstrates a basic LKM");