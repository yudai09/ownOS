/*
 *  linux/include/linux/ext2_fs.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */
#ifndef EXT2_H
#define EXT2_H

#include "ata.h"
#include "kcom.h"



class Ext2{
private:
  enum{
    EXT2_DEFAULT_RESERVE_BLOCKS  = 8,
    /*max window size: 1024(direct blocks) + 3([t,d]indirect blocks) */
    EXT2_MAX_RESERVE_BLOCKS         = 1027,
    EXT2_RESERVE_WINDOW_NOT_ALLOCATED = 0,

	/* Bad blocks inode */
	EXT2_BAD_INO		 = 1,
    /* Root inode */
    EXT2_ROOT_INO		 = 2,
	/* Boot loader inode */
    EXT2_BOOT_LOADER_INO	 = 5,
	/* Undelete directory inode */
    EXT2_UNDEL_DIR_INO	 = 6,
    /* First non-reserved inode for old ext2 filesystems */
    EXT2_GOOD_OLD_FIRST_INO	= 11,
    /*
     * Maximal count of links to a file
     */
    EXT2_LINK_MAX		= 32000,
    /*
     * Macro-instructions used to manage fragments
     */
    EXT2_MIN_FRAG_SIZE		= 1024,
	EXT2_MAX_FRAG_SIZE		= 4096,
    EXT2_MIN_FRAG_LOG_SIZE		  = 10,
    //# define EXT2_FRAG_SIZE(s)		(EXT2_MIN_FRAG_SIZE << (s)->s_log_frag_size)
    //# define EXT2_FRAGS_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / EXT2_FRAG_SIZE(s))
  };

  struct ext2_group_desc
  {
    u32_t	bg_block_bitmap;		/* Blocks bitmap block */
    u32_t	bg_inode_bitmap;		/* Inodes bitmap block */
    u32_t	bg_inode_table;		/* Inodes table block */
    u16_t	bg_free_blocks_count;	/* Free blocks count */
    u16_t	bg_free_inodes_count;	/* Free inodes count */
    u16_t	bg_used_dirs_count;	/* Directories count */
    u16_t	bg_pad;
    u32_t	bg_reserved[3];
  };

/*
 * Structure of an inode on the disk
 */
 public:

  struct ext2_inode {
  private:
    enum {
      FT_UNKNOWN,
      FT_FIFO     = 0x01,
      FT_CHRDEV   = 0x02,
      FT_DIR      = 0x04,
      FT_BLKDEV   = 0x06,
      FT_REG_FILE = 0x08,
      FT_SYMLINK  = 0x0a,
      FT_SOCK     = 0x0c,
      FT_MAX,
    };
  public:
    enum{
      EXT2_NDIR_BLOCKS = 12,
      EXT2_IND_BLOCK	 = EXT2_NDIR_BLOCKS,
      EXT2_DIND_BLOCK	 = (EXT2_IND_BLOCK + 1),
      EXT2_TIND_BLOCK	 = (EXT2_DIND_BLOCK + 1),
      EXT2_N_BLOCKS	 = (EXT2_TIND_BLOCK + 1),
    };

    u16_t	i_mode;		/* File mode */
    u16_t	i_uid;		/* Low 16 bits of Owner Uid */
    u32_t	i_size;		/* Size in bytes */
    u32_t	i_atime;	/* Access time */
    u32_t	i_ctime;	/* Creation time */
    u32_t	i_mtime;	/* Modification time */
    u32_t	i_dtime;	/* Deletion Time */
    u16_t	i_gid;		/* Low 16 bits of Group Id */
    u16_t	i_links_count;	/* Links count */
    u32_t	i_blocks;	/* Blocks count */
    u32_t	i_flags;	/* File flags */
    union {
      struct {
        u32_t  l_i_reserved1;
      } linux1;
      struct {
        u32_t  h_i_translator;
      } hurd1;
      struct {
        u32_t  m_i_reserved1;
      } masix1;
    } osd1;				/* OS dependent 1 */
    u32_t	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
    u32_t	i_generation;	/* File version (for NFS) */
    u32_t	i_file_acl;	/* File ACL */
    u32_t	i_dir_acl;	/* Directory ACL */
    u32_t	i_faddr;	/* Fragment address */
    union {
      struct {
        u8_t	l_i_frag;	/* Fragment number */
        u8_t	l_i_fsize;	/* Fragment size */
        u16_t	i_pad1;
        u16_t	l_i_uid_high;	/* these 2 fields    */
        u16_t	l_i_gid_high;	/* were reserved2[0] */
        u32_t	l_i_reserved2;
      } linux2;
      struct {
        u8_t	h_i_frag;	/* Fragment number */
        u8_t	h_i_fsize;	/* Fragment size */
        u16_t	h_i_mode_high;
        u16_t	h_i_uid_high;
        u16_t	h_i_gid_high;
        u32_t	h_i_author;
      } hurd2;
      struct {
        u8_t	m_i_frag;	/* Fragment number */
        u8_t	m_i_fsize;	/* Fragment size */
        u16_t	m_pad1;
        u32_t	m_i_reserved2[2];
      } masix2;
    } osd2;				/* OS dependent 2 */
    ext2_inode(){
      i_mode = FT_UNKNOWN;
    }
    bool is_directory(){
      if(i_mode >> 12 & FT_DIR)
        return true;
      return false;
    }
    bool is_valid(){
      u8_t type = i_mode >> 12;
      if(FT_UNKNOWN < type && type < FT_MAX)
        return true;
      return false;
    }
  };
 private:
  class ext2_super_block {
  private:
    u32_t	s_inodes_count;		/* Inodes count */
    u32_t	s_blocks_count;		/* Blocks count */
    u32_t	s_r_blocks_count;	/* Reserved blocks count */
    u32_t	s_free_blocks_count;	/* Free blocks count */
    u32_t	s_free_inodes_count;	/* Free inodes count */
    u32_t	s_first_data_block;	/* First Data Block */
    u32_t	s_log_block_size;	/* Block size */
    u32_t	s_log_frag_size;	/* Fragment size */
    u32_t	s_blocks_per_group;	/* # Blocks per group */
    u32_t	s_frags_per_group;	/* # Fragments per group */
    u32_t	s_inodes_per_group;	/* # Inodes per group */
    u32_t	s_mtime;		/* Mount time */
    u32_t	s_wtime;		/* Write time */
    u16_t	s_mnt_count;		/* Mount count */
    u16_t	s_max_mnt_count;	/* Maximal mount count */
    u16_t	s_magic;		/* Magic signature */
    u16_t	s_state;		/* File system state */
    u16_t	s_errors;		/* Behaviour when detecting errors */
    u16_t	s_minor_rev_level; 	/* minor revision level */
    u32_t	s_lastcheck;		/* time of last check */
    u32_t	s_checkinterval;	/* max. time between checks */
    u32_t	s_creator_os;		/* OS */
    u32_t	s_rev_level;		/* Revision level */
    u16_t	s_def_resuid;		/* Default uid for reserved blocks */
    u16_t	s_def_resgid;		/* Default gid for reserved blocks */
    /*
     * These fields are for EXT2_DYNAMIC_REV superblocks only.
     *
     * Note: the difference between the compatible feature set and
     * the incompatible feature set is that if there is a bit set
     * in the incompatible feature set that the kernel doesn't
     * know about, it should refuse to mount the filesystem.
     * 
     * e2fsck's requirements are more strict; if it doesn't know
     * about a feature in either the compatible or incompatible
     * feature set, it must abort and not try to meddle with
     * things it doesn't understand...
     */
    u32_t	s_first_ino; 		/* First non-reserved inode */
    u16_t   s_inode_size; 		/* size of inode structure */
    u16_t	s_block_group_nr; 	/* block group # of this superblock */
    u32_t	s_feature_compat; 	/* compatible feature set */
    u32_t	s_feature_incompat; 	/* incompatible feature set */
    u32_t	s_feature_ro_compat; 	/* readonly-compatible feature set */
    u8_t	s_uuid[16];		/* 128-bit uuid for volume */
    char	s_volume_name[16]; 	/* volume name */
    char	s_last_mounted[64]; 	/* directory where last mounted */
    u32_t	s_algorithm_usage_bitmap; /* For compression */
    /*
     * Performance hints.  Directory preallocation should only
     * happen if the EXT2_COMPAT_PREALLOC flag is on.
     */
    u8_t	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
    u8_t	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
    u16_t	s_padding1;
    /*
     * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
     */
    u8_t	s_journal_uuid[16];	/* uuid of journal superblock */
    u32_t	s_journal_inum;		/* inode number of journal file */
    u32_t	s_journal_dev;		/* device number of journal file */
    u32_t	s_last_orphan;		/* start of list of inodes to delete */
    u32_t	s_hash_seed[4];		/* HTREE hash seed */
    u8_t	s_def_hash_version;	/* Default hash version to use */
    u8_t	s_reserved_char_pad;
    u16_t	s_reserved_word_pad;
    u32_t	s_default_mount_opts;
    u32_t	s_first_meta_bg; 	/* First metablock block group */
    u32_t	s_reserved[190];	/* Padding to the end of the block */

    enum{
      EXT2_MIN_BLOCK_SIZE = 1024,
      EXT2_MAX_BLOCK_SIZE = 4096,
      EXT2_MIN_BLOCK_LOG_SIZE = 10,
    };
  public: 
    inline u32_t block_size(){
      return EXT2_MIN_BLOCK_SIZE << s_log_block_size;
    }
    inline size_t inode_size(){
      return s_inode_size;
    }
    inline u32_t first_inode(){
      return s_first_ino;
    }
    inline u32_t blocks_per_group(){
      return s_blocks_per_group;
    }
    inline u32_t inodes_per_group(){
      return s_inodes_per_group;
    }
    /* # define EXT2_BLOCK_SIZE(s)		(EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size) */
    /* #define	EXT2_ADDR_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (u32_t)) */
    /* # define EXT2_BLOCK_SIZE_BITS(s)	((s)->s_log_block_size + 10) */
    /* #define EXT2_INODE_SIZE(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ?  \ */
    /*                              EXT2_GOOD_OLD_INODE_SIZE :                 \ */
    /*                              (s)->s_inode_size) */
    /* #define EXT2_FIRST_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ?  \ */
    /*                              EXT2_GOOD_OLD_FIRST_INO :                  \ */
    /*                              (s)->s_first_ino) */
  };
  struct ext2_dir_entry {
  private:
    static const u8_t EXT2_NAME_LEN = 255;
  public:
    u32_t	inode;			/* Inode number */
    u16_t	rec_len;		/* Directory entry length */
    u16_t	name_len;		/* Name length */
    char	name[EXT2_NAME_LEN];	/* File name */
  };
  /*
   * The new version of the directory entry.  Since EXT2 structures are
   * stored in intel byte order, and the name_len field could never be
   * bigger than 255 chars, it's safe to reclaim the extra byte for the
   * file_type field.
   */
  struct ext2_dir_entry_2 {
  private:
    static const u8_t EXT2_NAME_LEN = 255;
  public:
    u32_t	inode;			/* Inode number */
    u16_t	rec_len;		/* Directory entry length */
    u8_t	name_len;		/* Name length */
    u8_t	file_type;
    char	name[EXT2_NAME_LEN];	/* File name */
  };

  /*These values are not accurate, but ok */
  static const u32_t super_block_offset = 0x400;
  static const u32_t group_desc_offset = 0x800;
  Ata *atadriver;
  /*fist block address is now 0x7e00 (this is not accurate..)*/
  static const u32_t first_blockaddr = 0x7e00;
//ext2_inode root_inode;
  ext2_super_block *super_block;
  ext2_inode root_inode;
  ext2_inode current_inode;

  
  
  u32_t inode_size;

  u32_t block_index2addr(u32_t index);
  ext2_inode get_inode(u32_t inode_index);
  ext2_inode find_inode_from_direntries(u8_t *buffer,kstring &search_for);
  

 public:
  u32_t block_size;
  ext2_inode invalid_inode;//̵����inode
  inline u32_t blocksize(){
    return block_size;
  }
  //  bool set_current_path(kvector<kstring> path);
  ext2_inode get_inode_path(kvector<kstring> path);
  void read_block(u32_t block_index,u8_t *buffer);
  //  bool read_cn_block(u32_t pos,void *buffer);
  bool read_block_pos(u32_t pos,void *dbuffer,ext2_inode inode);
  //bool set_ext2_root(Ata *ata);
  Ext2(Ata *ata);
};






  /*
   * Macro-instructions used to manage group descriptors
   */
  /* # define EXT2_BLOCKS_PER_GROUP(s)	((s)->s_blocks_per_group) */
  /* # define EXT2_DESC_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (struct ext2_group_desc)) */
  /* # define EXT2_INODES_PER_GROUP(s)	((s)->s_inodes_per_group) */
  
/*
 * Inode flags (GETFLAGS/SETFLAGS)
 */
  //	EXT2_SECRM_FL			 = FS_SECRM_FL,
  /* Undelete */
  //EXT2_UNRM_FL			 = FS_UNRM_FL,	
  /* #define	EXT2_COMPR_FL			FS_COMPR_FL	/\* Compress file *\/ */
  /* #define EXT2_SYNC_FL			FS_SYNC_FL	/\* Synchronous updates *\/ */
  /* #define EXT2_IMMUTABLE_FL		FS_IMMUTABLE_FL	/\* Immutable file *\/ */
  /* #define EXT2_APPEND_FL			FS_APPEND_FL	/\* writes to file may only append *\/ */
  /* #define EXT2_NODUMP_FL			FS_NODUMP_FL	/\* do not dump file *\/ */
  /* #define EXT2_NOATIME_FL			FS_NOATIME_FL	/\* do not update atime *\/ */
  /* /\* Reserved for compression usage... *\/ */
  /* #define EXT2_DIRTY_FL			FS_DIRTY_FL */
  /* #define EXT2_COMPRBLK_FL		FS_COMPRBLK_FL	/\* One or more compressed clusters *\/ */
  /* #define EXT2_NOCOMP_FL			FS_NOCOMP_FL	/\* Don't compress *\/ */
  /* #define EXT2_ECOMPR_FL			FS_ECOMPR_FL	/\* Compression error *\/ */
  /* /\* End compression flags --- maybe not all used *\/	 */
  /* #define EXT2_BTREE_FL			FS_BTREE_FL	/\* btree format dir *\/ */
  /* #define EXT2_INDEX_FL			FS_INDEX_FL	/\* hash-indexed directory *\/ */
  /* #define EXT2_IMAGIC_FL			FS_IMAGIC_FL	/\* AFS directory *\/ */
  /* #define EXT2_JOURNAL_DATA_FL		FS_JOURNAL_DATA_FL /\* Reserved for ext3 *\/ */
  /* #define EXT2_NOTAIL_FL			FS_NOTAIL_FL	/\* file tail should not be merged *\/ */
  /* #define EXT2_DIRSYNC_FL			FS_DIRSYNC_FL	/\* dirsync behaviour (directories only) *\/ */
  /* #define EXT2_TOPDIR_FL			FS_TOPDIR_FL	/\* Top of directory hierarchies*\/ */
  /* #define EXT2_RESERVED_FL		FS_RESERVED_FL	/\* reserved for ext2 lib *\/ */

  /* #define EXT2_FL_USER_VISIBLE		FS_FL_USER_VISIBLE	/\* User visible flags *\/ */
  /* #define EXT2_FL_USER_MODIFIABLE		FS_FL_USER_MODIFIABLE	/\* User modifiable flags *\/ */

  /* /\* Flags that should be inherited by new inodes from their parent. *\/ */
  /* #define EXT2_FL_INHERITED (EXT2_SECRM_FL | EXT2_UNRM_FL | EXT2_COMPR_FL | \ */
  /*                            EXT2_SYNC_FL | EXT2_IMMUTABLE_FL | EXT2_APPEND_FL | \ */
  /*                            EXT2_NODUMP_FL | EXT2_NOATIME_FL | EXT2_COMPRBLK_FL| \ */
  /*                            EXT2_NOCOMP_FL | EXT2_JOURNAL_DATA_FL |      \ */
  /*                            EXT2_NOTAIL_FL | EXT2_DIRSYNC_FL) */

  /* /\* Flags that are appropriate for regular files (all but dir-specific ones). *\/ */
  /* #define EXT2_REG_FLMASK (~(EXT2_DIRSYNC_FL | EXT2_TOPDIR_FL)) */

  /*     /\* Flags that are appropriate for non-directories/regular files. *\/ */
  /* #define EXT2_OTHER_FLMASK (EXT2_NODUMP_FL | EXT2_NOATIME_FL) */
  /* Mask out flags that are inappropriate for the given type of inode. */
  /* static __inline__ u32_t ext2_mask_flags(umode_t mode, u32_t flags) */
  /* { */
  /*   if (S_ISDIR(mode)) */
  /*     return flags; */
  /*   else if (S_ISREG(mode)) */
  /*     return flags & EXT2_REG_FLMASK; */
  /*   else */
  /*     return flags & EXT2_OTHER_FLMASK; */
  /* } */
  
  /*
   * ioctl commands
   */
/* #define	EXT2_IOC_GETFLAGS		FS_IOC_GETFLAGS */
/* #define	EXT2_IOC_SETFLAGS		FS_IOC_SETFLAGS */
/* #define	EXT2_IOC_GETVERSION		FS_IOC_GETVERSION */
/* #define	EXT2_IOC_SETVERSION		FS_IOC_SETVERSION */
/* #define	EXT2_IOC_GETRSVSZ		_IOR('f', 5, long) */
/* #define	EXT2_IOC_SETRSVSZ		_IOW('f', 6, long) */

/* /\* */
/*  * ioctl commands in 32 bit emulation */
/*  *\/ */
/* #define EXT2_IOC32_GETFLAGS		FS_IOC32_GETFLAGS */
/* #define EXT2_IOC32_SETFLAGS		FS_IOC32_SETFLAGS */
/* #define EXT2_IOC32_GETVERSION		FS_IOC32_GETVERSION */
/* #define EXT2_IOC32_SETVERSION		FS_IOC32_SETVERSION */

/* #define i_size_high	i_dir_acl */

/* #if defined(__KERNEL__) || defined(__linux__) */
/* #define i_reserved1	osd1.linux1.l_i_reserved1 */
/* #define i_frag		osd2.linux2.l_i_frag */
/* #define i_fsize		osd2.linux2.l_i_fsize */
/* #define i_uid_low	i_uid */
/* #define i_gid_low	i_gid */
/* #define i_uid_high	osd2.linux2.l_i_uid_high */
/* #define i_gid_high	osd2.linux2.l_i_gid_high */
/* #define i_reserved2	osd2.linux2.l_i_reserved2 */
/* #endif */

/* #ifdef	__hurd__ */
/* #define i_translator	osd1.hurd1.h_i_translator */
/* #define i_frag		osd2.hurd2.h_i_frag */
/* #define i_fsize		osd2.hurd2.h_i_fsize */
/* #define i_uid_high	osd2.hurd2.h_i_uid_high */
/* #define i_gid_high	osd2.hurd2.h_i_gid_high */
/* #define i_author	osd2.hurd2.h_i_author */
/* #endif */

/* #ifdef	__masix__ */
/* #define i_reserved1	osd1.masix1.m_i_reserved1 */
/* #define i_frag		osd2.masix2.m_i_frag */
/* #define i_fsize		osd2.masix2.m_i_fsize */
/* #define i_reserved2	osd2.masix2.m_i_reserved2 */
/* #endif */

/* /\* */
/*  * File system states */
/*  *\/ */
/* #define	EXT2_VALID_FS			0x0001	/\* Unmounted cleanly *\/ */
/* #define	EXT2_ERROR_FS			0x0002	/\* Errors detected *\/ */

/* /\* */
/*  * Mount flags */
/*  *\/ */
/* #define EXT2_MOUNT_CHECK		0x000001  /\* Do mount-time checks *\/ */
/* #define EXT2_MOUNT_OLDALLOC		0x000002  /\* Don't use the new Orlov allocator *\/ */
/* #define EXT2_MOUNT_GRPID		0x000004  /\* Create files with directory's group *\/ */
/* #define EXT2_MOUNT_DEBUG		0x000008  /\* Some debugging messages *\/ */
/* #define EXT2_MOUNT_ERRORS_CONT		0x000010  /\* Continue on errors *\/ */
/* #define EXT2_MOUNT_ERRORS_RO		0x000020  /\* Remount fs ro on errors *\/ */
/* #define EXT2_MOUNT_ERRORS_PANIC		0x000040  /\* Panic on errors *\/ */
/* #define EXT2_MOUNT_MINIX_DF		0x000080  /\* Mimics the Minix statfs *\/ */
/* #define EXT2_MOUNT_NOBH			0x000100  /\* No buffer_heads *\/ */
/* #define EXT2_MOUNT_NO_UID32		0x000200  /\* Disable 32-bit UIDs *\/ */
/* #define EXT2_MOUNT_XATTR_USER		0x004000  /\* Extended user attributes *\/ */
/* #define EXT2_MOUNT_POSIX_ACL		0x008000  /\* POSIX Access Control Lists *\/ */
/* #define EXT2_MOUNT_XIP			0x010000  /\* Execute in place *\/ */
/* #define EXT2_MOUNT_USRQUOTA		0x020000  /\* user quota *\/ */
/* #define EXT2_MOUNT_GRPQUOTA		0x040000  /\* group quota *\/ */
/* #define EXT2_MOUNT_RESERVATION		0x080000  /\* Preallocation *\/ */


/* #define clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt */
/* #define set_opt(o, opt)			o |= EXT2_MOUNT_##opt */
/* #define test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \ */
/*                                  EXT2_MOUNT_##opt) */
/* /\* */
/*  * Maximal mount counts between two filesystem checks */
/*  *\/ */
/* #define EXT2_DFL_MAX_MNT_COUNT		20	/\* Allow 20 mounts *\/ */
/* #define EXT2_DFL_CHECKINTERVAL		0	/\* Don't use interval check *\/ */

/* /\* */
/*  * Behaviour when detecting errors */
/*  *\/ */
/* #define EXT2_ERRORS_CONTINUE		1	/\* Continue execution *\/ */
/* #define EXT2_ERRORS_RO			2	/\* Remount fs read-only *\/ */
/* #define EXT2_ERRORS_PANIC		3	/\* Panic *\/ */
/* #define EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE */

/* /\* */
/*  * Structure of the super block */
/*  *\/ */



/*
 * Codes for operating systems
 */
/* #define EXT2_OS_LINUX		0 */
/* #define EXT2_OS_HURD		1 */
/* #define EXT2_OS_MASIX		2 */
/* #define EXT2_OS_FREEBSD		3 */
/* #define EXT2_OS_LITES		4 */

/*
 * Revision levels
 */
/* #define EXT2_GOOD_OLD_REV	0	/\* The good old (original) format *\/ */
/* #define EXT2_DYNAMIC_REV	1 	/\* V2 format w/ dynamic inode sizes *\/ */

/* #define EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV */
/* #define EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV */

/* #define EXT2_GOOD_OLD_INODE_SIZE 128 */

/*
 * Feature set definitions
 */

/* #define EXT2_HAS_COMPAT_FEATURE(sb,mask)                        \ */
/*   ( EXT2_SB(sb)->s_es->s_feature_compat & cpu_to_le32(mask) ) */
/* #define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)                         \ */
/*   ( EXT2_SB(sb)->s_es->s_feature_ro_compat & cpu_to_le32(mask) ) */
/* #define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)                      \ */
/*   ( EXT2_SB(sb)->s_es->s_feature_incompat & cpu_to_le32(mask) ) */
/* #define EXT2_SET_COMPAT_FEATURE(sb,mask)                    \ */
/*   EXT2_SB(sb)->s_es->s_feature_compat |= cpu_to_le32(mask) */
/* #define EXT2_SET_RO_COMPAT_FEATURE(sb,mask)                     \ */
/*   EXT2_SB(sb)->s_es->s_feature_ro_compat |= cpu_to_le32(mask) */
/* #define EXT2_SET_INCOMPAT_FEATURE(sb,mask)                      \ */
/*   EXT2_SB(sb)->s_es->s_feature_incompat |= cpu_to_le32(mask) */
/* #define EXT2_CLEAR_COMPAT_FEATURE(sb,mask)                  \ */
/*   EXT2_SB(sb)->s_es->s_feature_compat &= ~cpu_to_le32(mask) */
/* #define EXT2_CLEAR_RO_COMPAT_FEATURE(sb,mask)                   \ */
/*   EXT2_SB(sb)->s_es->s_feature_ro_compat &= ~cpu_to_le32(mask) */
/* #define EXT2_CLEAR_INCOMPAT_FEATURE(sb,mask)                    \ */
/*   EXT2_SB(sb)->s_es->s_feature_incompat &= ~cpu_to_le32(mask) */

/* #define EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001 */
/* #define EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002 */
/* #define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004 */
/* #define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008 */
/* #define EXT2_FEATURE_COMPAT_RESIZE_INO		0x0010 */
/* #define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020 */
/* #define EXT2_FEATURE_COMPAT_ANY			0xffffffff */

/* #define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001 */
/* #define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002 */
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 */
/* #define EXT2_FEATURE_RO_COMPAT_ANY		0xffffffff */

/* #define EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001 */
/* #define EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002 */
/* #define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 */
/* #define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 */
/* #define EXT2_FEATURE_INCOMPAT_META_BG		0x0010 */
/* #define EXT2_FEATURE_INCOMPAT_ANY		0xffffffff */

/* #define EXT2_FEATURE_COMPAT_SUPP	EXT2_FEATURE_COMPAT_EXT_ATTR */
/* #define EXT2_FEATURE_INCOMPAT_SUPP	(EXT2_FEATURE_INCOMPAT_FILETYPE|    \ */
/*                                      EXT2_FEATURE_INCOMPAT_META_BG) */
/* #define EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \ */
/*                                      EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \ */
/*                                      EXT2_FEATURE_RO_COMPAT_BTREE_DIR) */
/* #define EXT2_FEATURE_RO_COMPAT_UNSUPPORTED	~EXT2_FEATURE_RO_COMPAT_SUPP */
/* #define EXT2_FEATURE_INCOMPAT_UNSUPPORTED	~EXT2_FEATURE_INCOMPAT_SUPP */

/* /\* */
/*  * Default values for user and/or group using reserved blocks */
/*  *\/ */
/* #define	EXT2_DEF_RESUID		0 */
/* #define	EXT2_DEF_RESGID		0 */

/* /\* */
/*  * Default mount options */
/*  *\/ */
/* #define EXT2_DEFM_DEBUG		0x0001 */
/* #define EXT2_DEFM_BSDGROUPS	0x0002 */
/* #define EXT2_DEFM_XATTR_USER	0x0004 */
/* #define EXT2_DEFM_ACL		0x0008 */
/* #define EXT2_DEFM_UID16		0x0010 */
/* /\* Not used by ext2, but reserved for use by ext3 *\/ */
/* #define EXT3_DEFM_JMODE		0x0060  */
/* #define EXT3_DEFM_JMODE_DATA	0x0020 */
/* #define EXT3_DEFM_JMODE_ORDERED	0x0040 */
/* #define EXT3_DEFM_JMODE_WBACK	0x0060 */

/*
 * Structure of a directory entry
 */






/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */


/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
/* #define EXT2_DIR_PAD		 	4 */
/* #define EXT2_DIR_ROUND 			(EXT2_DIR_PAD - 1) */
/* #define EXT2_DIR_REC_LEN(name_len)	(((name_len) + 8 + EXT2_DIR_ROUND) & \ */
/*                                      ~EXT2_DIR_ROUND) */
/* #define EXT2_MAX_REC_LEN		((1<<16)-1) */




#endif
