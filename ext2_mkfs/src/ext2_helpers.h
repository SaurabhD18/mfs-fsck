#ifndef EXT2_HELPERS_H
#define EXT2_HELPERS_H

#include <ext2fs/ext2_fs.h>

// Functions
int open_disk(const char *disk_path);
void write_superblock(int fd);
void close_disk(int fd);

#endif // EXT2_HELPERS_H

