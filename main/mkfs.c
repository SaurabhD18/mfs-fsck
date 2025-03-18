#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <uuid/uuid.h>
#include <math.h>
#include <getopt.h>
#include <stdbool.h>
#include "mkfs.h"

int passing_args(int argc, char *argv[], params *options) {
    if (argc < 2) {
        printf("Usage: %s <device_path> [-b <block_size>] [-g <blocks_per_group>] [-L <label>]\n", argv[0]);
        return -1;
    }

    options->path = argv[1];  // First argument is the device path

    // optarg is a global variable defined in the getopt.h library, automatically set by getopt() whenever an option that requires an argument (like -b 1024) is parsed
    int opt;
    while ((opt = getopt(argc - 1, argv + 1, "b:g:L:")) != -1) {
        switch (opt) {
            case 'b':
                options->bs = atoi(optarg);
                break;
            case 'g':
                options->bpg = atoi(optarg);
                break;
            case 'L':
                options->label = optarg;
                break;
            default:
                printf("Usage: %s <device_path> -b <block_size> -g <blocks_per_group> -L <label>\n", argv[0]);
                return -1;
        }
    }

    return 0;  
}






// <------------ FOR REFERENCE :- EXT2 STRUCT Declaration CODE  ------------>

// struct ext2_super_block {
//     /*000*/	__u32	s_inodes_count;		/* Inodes count */
//         __u32	s_blocks_count;		/* Blocks count */
//         __u32	s_r_blocks_count;	/* Reserved blocks count */
//         __u32	s_free_blocks_count;	/* Free blocks count */
//     /*010*/	__u32	s_free_inodes_count;	/* Free inodes count */
//         __u32	s_first_data_block;	/* First Data Block */
//         __u32	s_log_block_size;	/* Block size */
//         __u32	s_log_cluster_size;	/* Allocation cluster size */
//     /*020*/	__u32	s_blocks_per_group;	/* # Blocks per group */
//         __u32	s_clusters_per_group;	/* # Fragments per group */
//         __u32	s_inodes_per_group;	/* # Inodes per group */
//         __u32	s_mtime;		/* Mount time */
//     /*030*/	__u32	s_wtime;		/* Write time */
//         __u16	s_mnt_count;		/* Mount count */
//         __s16	s_max_mnt_count;	/* Maximal mount count */
//         __u16	s_magic;		/* Magic signature */
//         __u16	s_state;		/* File system state */
//         __u16	s_errors;		/* Behaviour when detecting errors */
//         __u16	s_minor_rev_level;	/* minor revision level */
//     /*040*/	__u32	s_lastcheck;		/* time of last check */
//         __u32	s_checkinterval;	/* max. time between checks */
//         __u32	s_creator_os;		/* OS */
//         __u32	s_rev_level;		/* Revision level */
//     /*050*/	__u16	s_def_resuid;		/* Default uid for reserved blocks */
//         __u16	s_def_resgid;		/* Default gid for reserved blocks */
//         /*
//          * These fields are for EXT2_DYNAMIC_REV superblocks only.
//          *
//          * Note: the difference between the compatible feature set and
//          * the incompatible feature set is that if there is a bit set
//          * in the incompatible feature set that the kernel doesn't
//          * know about, it should refuse to mount the filesystem.
//          *
//          * e2fsck's requirements are more strict; if it doesn't know
//          * about a feature in either the compatible or incompatible
//          * feature set, it must abort and not try to meddle with
//          * things it doesn't understand...
//          */
//         __u32	s_first_ino;		/* First non-reserved inode */
//         __u16   s_inode_size;		/* size of inode structure */
//         __u16	s_block_group_nr;	/* block group # of this superblock */
//         __u32	s_feature_compat;	/* compatible feature set */
//     /*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
//         __u32	s_feature_ro_compat;	/* readonly-compatible feature set */
//     /*068*/	__u8	s_uuid[16];		/* 128-bit uuid for volume */
//     /*078*/	char	s_volume_name[EXT2_LABEL_LEN];	/* volume name */
//     /*088*/	char	s_last_mounted[64];	/* directory where last mounted */
//     /*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
//         /*
//          * Performance hints.  Directory preallocation should only
//          * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
//          */
//         __u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
//         __u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
//         __u16	s_reserved_gdt_blocks;	/* Per group table for online growth */
//         /*
//          * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
//          */
//     /*0d0*/	__u8	s_journal_uuid[16];	/* uuid of journal superblock */
//     /*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
//         __u32	s_journal_dev;		/* device number of journal file */
//         __u32	s_last_orphan;		/* start of list of inodes to delete */
//     /*0ec*/	__u32	s_hash_seed[4];		/* HTREE hash seed */
//     /*0fc*/	__u8	s_def_hash_version;	/* Default hash version to use */
//         __u8	s_jnl_backup_type;	/* Default type of journal backup */
//         __u16	s_desc_size;		/* Group desc. size: INCOMPAT_64BIT */
//     /*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
//         __u32	s_first_meta_bg;	/* First metablock group */
//         __u32	s_mkfs_time;		/* When the filesystem was created */
//     /*10c*/	__u32	s_jnl_blocks[17];	/* Backup of the journal inode */
//     /*150*/	__u32	s_blocks_count_hi;	/* Blocks count high 32bits */
//         __u32	s_r_blocks_count_hi;	/* Reserved blocks count high 32 bits*/
//         __u32	s_free_blocks_hi;	/* Free blocks count */
//         __u16	s_min_extra_isize;	/* All inodes have at least # bytes */
//         __u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
//     /*160*/	__u32	s_flags;		/* Miscellaneous flags */
//         __u16	s_raid_stride;		/* RAID stride in blocks */
//         __u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
//         __u64	s_mmp_block;		/* Block for multi-mount protection */
//     /*170*/	__u32	s_raid_stripe_width;	/* blocks on all data disks (N*stride)*/
//         __u8	s_log_groups_per_flex;	/* FLEX_BG group size */
//         __u8	s_checksum_type;	/* metadata checksum algorithm */
//         __u8	s_encryption_level;	/* versioning level for encryption */
//         __u8	s_reserved_pad;		/* Padding to next 32bits */
//         __u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
//     /*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
//         __u32	s_snapshot_id;		/* sequential ID of active snapshot */
//         __u64	s_snapshot_r_blocks_count; /* active snapshot reserved blocks */
//     /*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
//     #define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
//         __u32	s_error_count;		/* number of fs errors */
//         __u32	s_first_error_time;	/* first time an error happened */
//         __u32	s_first_error_ino;	/* inode involved in first error */
//     /*1a0*/	__u64	s_first_error_block;	/* block involved in first error */
//         __u8	s_first_error_func[32];	/* function where the error happened */
//     /*1c8*/	__u32	s_first_error_line;	/* line number where error happened */
//         __u32	s_last_error_time;	/* most recent time of an error */
//     /*1d0*/	__u32	s_last_error_ino;	/* inode involved in last error */
//         __u32	s_last_error_line;	/* line number where error happened */
//         __u64	s_last_error_block;	/* block involved of last error */
//     /*1e0*/	__u8	s_last_error_func[32];	/* function where the error happened */
//     #define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)
//     /*200*/	__u8	s_mount_opts[64];
//     /*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
//         __u32	s_grp_quota_inum;	/* inode number of group quota file */
//         __u32	s_overhead_blocks;	/* overhead blocks/clusters in fs */
//     /*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
//     /*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
//     /*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
//     /*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
//         __le32  s_prj_quota_inum;	/* inode for tracking project quota */
//     /*270*/	__le32	s_checksum_seed;	/* crc32c(orig_uuid) if csum_seed set */
//     /*274*/	__u8	s_wtime_hi;
//         __u8	s_mtime_hi;
//         __u8	s_mkfs_time_hi;
//         __u8	s_lastcheck_hi;
//         __u8	s_first_error_time_hi;
//         __u8	s_last_error_time_hi;
//         __u8	s_pad[2];
//     /*27c*/ __le16	s_encoding;		/* Filename charset encoding */
//         __le16	s_encoding_flags;	/* Filename charset encoding flags */
//         __le32	s_reserved[95];		/* Padding to the end of the block */
//     /*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
//     };
// <------------ FOR REFERENCE :- EXT2 STRUCT Declaration CODE  ------------>