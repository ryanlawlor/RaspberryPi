// #include "hollyfs.h"

#include <linux/module.h>
#include <linux/namei.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/parser.h>
#include <linux/unistd.h>
// #include <linux/stdio.h>
#include <linux/random.h>
#include <linux/version.h>
#include <linux/time.h>


static struct file_system_type hollyfs_fs_type = {
    .owner = THIS_MODULE,
    .name = "hollyfs",
    // .get_sb = holly_get_sb,
    .kill_sb = kill_block_super,
    .fs_flags = FS_REQUIRES_DEV,
};

static struct super_block hollyfs_superblock = {
    dev_t s_dev;
    unsigned char s_blocksize;
    unsigned long s_flags;
    struct file_system_type *s_type;
    // struct super_operations *s_op;
    struct dentry *s_root;
};

static struct inode hollyfs_inode = {
    unsigned long i_ino;
    umode_t i_mode;
    uid_t i_uid;
    gid_t i_gid;
    kdev_t i_rdev;
    loff_t i_size;
    struct timespec i_atime;
    struct timespec i_ctime;
    struct timespec i_mtime;
    struct super_block *i_sb;
    struct inode_operations *i_op;
    struct address_space *i_mapping;
    struct list_head i_dentry;
};

static struct dentry hollyfs_dentry = {
    struct inode *d_inode;
    struct dentry *d_parent;
    struct qstr d_name;
    struct super_block *d_sb;
    struct dentry_operations *d_op;
    struct list_head d_subdirs;
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

// partition device name: mmcblk0p3 ?

/*
* Here is a rough outline of the hollyfs partition
* --------------------------------------------------------------
* |  sb  |     32 data blocks       |    32 inode blocks       |
* --------------------------------------------------------------
* 0    X1 X2                     X3 X4                       262144               
*
* There are Z data blocks, 32 inode blocks, and W blocks used by super-block
* All blocks are 4096 bytes large, the total fs is 64 blocks
*/