const unsigned int HOLLYFS_MAGIC_NUM = 25;
const unsigned int HOLLYFS_FILE_TYPE_DIR = 5;
const unsigned int HOLLYFS_FILE_TYPE_FILE = 6;
#define HOLLYFS_FILENAME_MAX_LENGTH 255;

const unsigned int HOLLYFS_SUPER_BLOCK_SIZE = 8192;
const unsigned int HOLLYFS_BLOCK_SIZE = 512;
#define HOLLYFS_DATA_BLOCK_COUNT 2048;
#define HOLLYFS_INODE_BLOCK_COUNT 2032;

const unsigned int HOLLYFS_DATA_BLOCK_BASE = 16;
const unsigned int HOLLYFS_INODE_BLOCK_BASE = 2064;

struct hollyfs_superblock 
{
    unsigned int magic_num;

    unsigned short data_block_map[HOLLYFS_DATA_BLOCK_COUNT];
    unsigned short inode_block_map[HOLLYFS_INODE_BLOCK_COUNT];
};
typedef struct hollyfs_superblock hollyfs_superblock;

struct hollyfs_inode 
{
    unsigned int inode_num;
    unsigned int data_block_num;
    unsigned int file_size;
    unsigned int dir_child_count;
    unsigned int type;
};
typedef struct hollyfs_inode hollyfs_inode;

struct hollyfs_dentry 
{
    unsigned int inode_num;
    char filename[HOLLYFS_FILENAME_MAX_LENGTH];
};
typedef struct hollyfs_dentry hollyfs_dentry;