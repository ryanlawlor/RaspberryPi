const unsigned int RYANFS_MAGIC_NUM = 25;
const unsigned int RYANFS_FILE_TYPE_DIR = 5;
const unsigned int RYANFS_FILE_TYPE_FILE = 6;
#define RYANFS_FILENAME_MAX_LENGTH 255

const unsigned int RYANFS_SUPER_BLOCK_SIZE = 8192;
const unsigned int RYANFS_BLOCK_SIZE = 512;
#define RYANFS_DATA_BLOCK_COUNT 2048
#define RYANFS_INODE_BLOCK_COUNT 2032

const unsigned int RYANFS_DATA_BLOCK_BASE = 16;
const unsigned int RYANFS_INODE_BLOCK_BASE = 2064;

struct ryanfs_superblock 
{
    unsigned int magic_num;
    unsigned int block_size;
};
typedef struct ryanfs_superblock ryanfs_superblock;

struct ryanfs_inode 
{
    unsigned int inode_num;
    unsigned int data_block_num;
    unsigned int file_size;
    unsigned int dir_child_count;
    unsigned int type;
    mode_t mode;
};
typedef struct ryanfs_inode ryanfs_inode;

struct ryanfs_dentry 
{
    unsigned int inode_num;
    char filename[RYANFS_FILENAME_MAX_LENGTH];
};
typedef struct ryanfs_dentry ryanfs_dentry;