#include <stdio.h>
#include <stdlib.h>


#include <fcntl.h> // Opening and closing files
#include <errno.h> // Error handling

#include <linux/fs.h> // to define the file system types and the file system magic numbers.
#include <ext2fs/ext2_fs.h> // Ext2 file system structures

#include <sys/types.h> // to define the data types used in system calls

#include <unistd.h> // to provide access to the POSIX operating system API

// #include <sys/stat.h>

#include <string.h> // String manipulation
#include <time.h> // Time functions
#include <uuid/uuid.h> // UUID generation - Universally Unique Identifier
#include <math.h> // Mathematical functions
#include <getopt.h> // Command line argument parsing
#include <stdbool.h> // Boolean data type






typedef struct{

    char *path; // Path to the device
    int bs; // Block size
    int bpg; // Number of blocks per group
    char *label; // Volume label
     
} params;



int passing_args(int argc, char *argv[], params *options);
