#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/random.h>
#include <linux/version.h>
#include <linux/time.h>
#include <linux/buffer_head.h>
#include <linux/blkdev.h>

#include "ryanfs.h"

static int ryanfs_iterate_dir(struct file *filp, struct dir_context *ctx)
{
    printk(KERN_INFO "STARTING ITERATE!");
    // loff_t pos;
    struct inode *inode;
    struct super_block *sb;
    struct buffer_head *bh;
    struct ryanfs_inode *ryanfs_inode;
    struct ryanfs_dentry *record;
    int i;
    int over;

    // pos = ctx->pos;
    inode = file_inode(filp);
    sb = inode->i_sb;
    printk(KERN_INFO "inode done");
    // navigate to beginning of inodes to get the root inode
    bh = sb_bread(sb, RYANFS_INODE_BLOCK_BASE);
    if (bh == NULL)
    {
        printk(KERN_ERR "could not read block\n");
        return -1;
    }
    // fill inode variable with data from fs root inode
    ryanfs_inode = (struct ryanfs_inode *)bh->b_data;
    // ryanfs_inode = inode->i_private;
    printk(KERN_INFO "root inode child count: %d", ryanfs_inode->dir_child_count);

    // navigate buffer head to first data block where the dentry is stored
    bh = sb_bread(sb, RYANFS_DATA_BLOCK_BASE);
    // fill variable with dentry data
    record = (struct ryanfs_dentry *)bh->b_data;
    printk(KERN_INFO "readme dentry filename: %s", record->filename);

    for (i = 0; i < ryanfs_inode->dir_child_count; i++)
    {
        // iterate through each dentry child
        printk(KERN_INFO "ITERATING THROUGHT CHILD %d", i);
        // call dir_emit() with each dentry child data, filename, and inode_num
        over = dir_emit(ctx, record->filename, RYANFS_FILENAME_MAX_LENGTH, record->inode_num, DT_UNKNOWN);
        if (over)
        {
            printk(KERN_DEBUG "Read %s from folder %s, ctx->pos: %lld\n", record->filename, filp->f_path.dentry->d_name.name, ctx->pos);
            ctx->pos++;
            break;
        }
        // move position ahead
        ctx->pos++;
    }
    brelse(bh);
    printk(KERN_INFO "DONE ITERATE!");
    return 0;
}

static const struct file_operations ryanfs_dir_ops = {
    .iterate = ryanfs_iterate_dir,
};

struct dentry *ryanfs_lookup(struct inode *parent_inode, struct dentry *child_dentry, unsigned int flags)
{
    printk(KERN_INFO "STARTING LOOKUP!");
    struct ryanfs_inode *parent = parent_inode->i_private;
    struct super_block *sb = parent_inode->i_sb;
    struct buffer_head *bh;
    struct ryanfs_dentry *record;
    struct ryanfs_inode *ryanfs_child_inode;
    struct inode *child_inode;
    int i;

    // navigate to data block of the given dentry provided in the function
    bh = sb_bread(sb, RYANFS_DATA_BLOCK_BASE + parent->data_block_num);
    // fill variable with data from the data block
    record = (struct ryanfs_dentry *)bh->b_data;
    for (i = 0; i < parent->dir_child_count; i++)
    {
        // scan through each dentry in the fs from the given dentry root and see if one matches
        if(!strcmp(record->filename, child_dentry->d_name.name))
        {
            printk(KERN_INFO "FOUND A DENTRY");
            // call d_add() function if dentry is found and matches after scan
            d_add(child_dentry, child_inode);
            return NULL;
        }
    }

    printk(KERN_INFO "DID NOT FIND A DENTRY");
    return NULL;
}

static const struct inode_operations ryanfs_dir_inode_ops = {
    .lookup = ryanfs_lookup,
};

ssize_t ryanfs_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
    struct super_block *sb;
    struct inode *inoide;
    struct ryanfs_inode *ryanfs_inode;
    struct buffer_head *bh;
    char *buffer;
    int nbytes;

    inode = file_inode(filp);
    sb = inode->i_sb;
    ryanfs_inode = inode->i_private;

    if (*ppos >= ryanfs_inode->file_size)
    {
        return 0;
    }

    bh = sb_bread(sb, ryanfs_inode->data_block_num);
    // create buffer with chars of data read from the data block with the readme.txt file in it
    buffer = (char *)bh->b_data + *ppos;
    nbytes = min((size_t)(ryanfs_inode->file_size - *ppos), len);

    brelse(bh);
    *ppos += nbytes;
    return nbytes;
}

static const struct file_operations ryanfs_file_ops = {
    .read = ryanfs_read,
};

int ryanfs_fill_super(struct super_block *sb, void *data, int silent)
{
    struct inode *root_inode;
    struct ryanfs_inode *root_ryanfs_inode;
    struct buffer_head *bh;
    struct ryanfs_superblock *ryanfs_sb;
    // sets the buffer head to the first block in the partition, where the sb is
    bh = sb_bread(sb, 0);

    // fills the ryanfs superblock variable with the data at the superblock data block
    ryanfs_sb = (struct ryanfs_superblock *)bh->b_data;
    printk(KERN_INFO "The magic number obtained in disk is: [%d]\n", ryanfs_sb->magic_num);

    sb->s_magic = ryanfs_sb->magic_num;
    sb->s_fs_info = ryanfs_sb;
    sb->s_maxbytes = ryanfs_sb->block_size;

    // printk(KERN_INFO "Finished filling in superblock");

    bh = sb_bread(sb, RYANFS_INODE_BLOCK_BASE);

    // printk(KERN_INFO "Set new b head for inode");

    root_inode = new_inode(sb);
    root_inode->i_ino = 0;
    inode_init_owner(root_inode, NULL, S_IFDIR);
    root_inode->i_sb = sb;
    root_inode->i_op = &ryanfs_dir_inode_ops;
    root_inode->i_fop = &ryanfs_dir_ops;
    root_inode->i_private = root_ryanfs_inode;
    sb->s_root = d_make_root(root_inode);

    printk(KERN_INFO "Finished fill super!");
    
    brelse(bh);
    return 0;
}

static struct dentry *ryanfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
    struct dentry *ret;

    // actual mount system call that is called when the file system is registered and then mounted.
    // gives the function the necessary data, most importantly calls the fill super method.
    ret = mount_bdev(fs_type, flags, dev_name, data, ryanfs_fill_super);

    if (unlikely(IS_ERR(ret)))
    {
        printk(KERN_ERR "Error mounting ryanfs");
    }
    else
    {
        printk(KERN_INFO "ryanfs is successfully mounted");
    }

    return ret;
}

static void ryanfs_kill_superblock(struct super_block *sb)
{
    printk(KERN_INFO "ryanfs superblock is destroyed. Unmount successful.\n");
    // method to kill the fs and sb
    kill_block_super(sb);
    return;
}

static struct file_system_type ryanfs_fs_type = {
    // create file system struct with necessary data
    .owner = THIS_MODULE,
    .name = "ryanfs",
    // this call the fs mount function above
    .mount = ryanfs_mount,
    // kills the fs
    .kill_sb = ryanfs_kill_superblock,
    .fs_flags = FS_REQUIRES_DEV,
};

static int __init start_hello(void)
{
    int ret;
    printk(KERN_ALERT "Hello, world from the Linux kernel!\n");
    // call to register file system given the file system type struct
    ret = register_filesystem(&ryanfs_fs_type);
    if (likely(ret == 0))
    {
        printk(KERN_INFO "Successfully registered ryanfs\n");
    }
    else
    {
        printk(KERN_ERR "Failed to register ryanfs. Error code: %d\n", ret);
    }
    return ret;
}

static void __exit end_hello(void)
{
    int ret;
    printk(KERN_ALERT "Short is the life of an LKM\n");
    // call to unregister file system given the file system type struct
    ret = unregister_filesystem(&ryanfs_fs_type);
    if (likely(ret == 0))
    {
        printk(KERN_INFO "Successfully unregistered ryanfs\n");
    }
    else
    {
        printk(KERN_ERR "Failed to unregister ryanfs. Error code: %d\n", ret);
    }
}

module_init(start_hello);
module_exit(end_hello);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ryan Lawlor");
MODULE_DESCRIPTION("This module demonstrates a basic LKM");

// partition device name: mmcblk0p3 ?

/*
* Here is a rough outline of the ryanfs partition
* --------------------------------------------------------------
* |  sb  |     32 data blocks       |    32 inode blocks       |
* --------------------------------------------------------------
* 0    X1 X2                     X3 X4                       262144               
*
* There are 32 data blocks, 32 inode blocks, and 16 blocks used by super-block
* All blocks are 4096 bytes large, the total fs is 80 blocks
*/