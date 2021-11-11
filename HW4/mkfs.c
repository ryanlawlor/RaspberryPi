#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ryanfs.h"

int write_to_dev(int block_num, void *data, int data_size, int fd)
{
    // move to correct byte based on fs block size and where the write command is set to
    off_t s_out = lseek(fd, block_num * RYANFS_BLOCK_SIZE, SEEK_SET);
    if (s_out == -1)
    {
        return -1;
    }
    // actually call the write command with the provided data
    return write(fd, data, data_size);
}

int main()
{
    int fd;
    // call the open command on the partition
    fd = open("/dev/mmcblk0p3", O_RDWR);
    if (fd == -1)
    {
        perror("Error opening the device");
        return -1;
    }

    // 1
    // sb in first block
    ryanfs_superblock *sb = malloc(sizeof(ryanfs_superblock));
    // set magic num and block size for super block
    sb->magic_num = RYANFS_MAGIC_NUM;
    sb->block_size = RYANFS_BLOCK_SIZE;
    // call write function with the given data that will eventually call write system call
    write_to_dev(0, sb, sizeof(ryanfs_superblock), fd);
    free(sb);
    printf("Wrote super block!\n");

    // 2 
    // root inode
    ryanfs_inode *root_inode = malloc(sizeof(ryanfs_inode));
    // set the root node information
    root_inode->inode_num = 0;
    root_inode->data_block_num = 0;
    root_inode->dir_child_count = 1;
    root_inode->type = RYANFS_FILE_TYPE_DIR;
    printf("Root inode child count: %d\n", root_inode->dir_child_count);
    write_to_dev(RYANFS_INODE_BLOCK_BASE, root_inode, sizeof(ryanfs_inode), fd);
    free(root_inode);
    printf("Wrote root inode!\n");

    // 3 
    // readme dentry
    ryanfs_dentry *readme_dentry = malloc(sizeof(ryanfs_dentry));
    // fill in readme dentry data and call write function
    readme_dentry->inode_num = 1;
    strcpy(readme_dentry->filename, "readme.txt");
    write_to_dev(RYANFS_DATA_BLOCK_BASE, readme_dentry, sizeof(ryanfs_dentry), fd);
    free(readme_dentry);
    printf("Wrote readme dentry!\n");

    // create readme text file string
    char readme_text[] = "Hi from Ryan!";

    // 4 
    // readme inode
    ryanfs_inode *readme_inode = malloc(sizeof(ryanfs_inode));
    // create readme file inode and fill in data, call write function
    readme_inode->inode_num = 1;
    readme_inode->data_block_num = 1;
    readme_inode->dir_child_count = NULL;
    readme_inode->type = RYANFS_FILE_TYPE_FILE;
    readme_inode->file_size = sizeof(readme_text);
    readme_inode->mode = S_IFREG;
    write_to_dev(RYANFS_INODE_BLOCK_BASE + 1, readme_inode, sizeof(ryanfs_inode), fd);
    free(readme_inode);
    printf("Wrote readme inode!\n");

    // 5 
    // readme.txt file in data block 1
    // write data to readme file data block
    write_to_dev(RYANFS_DATA_BLOCK_BASE + 1, readme_text, sizeof(readme_text), fd);
    printf("Wrote readme block!\n");

    close(fd);
    return 0;
}