#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "ext2_helpers.h"
#include "../include/ext2_defs.h"



// Open the disk image file
int open_disk(const char *disk_path) {
    int fd = open(disk_path, O_RDWR);
    if (fd < 0) {
        perror("Error opening disk image");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// Write the superblock
void write_superblock(int fd) {
    struct ext2_super_block sb;
    memset(&sb, 0, sizeof(sb));

    sb.s_inodes_count = 1024;    // Number of inodes
    sb.s_blocks_count = 8192;    // Number of blocks
    sb.s_r_blocks_count = 100;   // Reserved blocks
    sb.s_free_blocks_count = 8000;
    sb.s_free_inodes_count = 1000;
    sb.s_first_data_block = 1;
    sb.s_log_block_size = 0;  // 1024-byte blocks
    sb.s_magic = EXT2_SUPER_MAGIC;

    lseek(fd, SUPERBLOCK_OFFSET, SEEK_SET);
    write(fd, &sb, sizeof(sb));
}


void close_disk(int fd) {
    close(fd);
}


