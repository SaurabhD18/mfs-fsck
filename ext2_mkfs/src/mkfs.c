#include <stdio.h>
#include "ext2_helpers.h"

#define DISK_PATH "my_ext2_disk.img"

int main() {
    printf("🚀 Initializing ext2 file system on %s...\n", DISK_PATH);

    int fd = open_disk(DISK_PATH);

    write_superblock(fd);

    printf("✅ File system formatted successfully!\n");

    close_disk(fd);
    return 0;
}

