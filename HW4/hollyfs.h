

struct hollyfs_inode
{
    mode_t mode;
    uint64_t inode_no;
    uint64_t data_block_number;

    union
    {
        uint64_t file_size;
        uint64_t dir_children_count;
    };
};

struct hollyfs_super_block
{
    uint64_t version;
    uint64_t magic;
    uint64_t block_size;

    // uint64_t inodes_count;

    uint64_t free_blocks;

    // struct journal_s *journal;

    char padding[4048];
};