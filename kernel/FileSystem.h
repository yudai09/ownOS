#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H
#include "kcom.h"
#include "Ext2.h"
class File;
class FileSystem{
  //パス中に含まれるファイル名の最大数
  static const u32_t MAX_FILE_NR=20;
  Ext2 *root_fs;
 public:
  FileSystem(){
  }
  FileSystem(Ext2 *root_fs){
    this->root_fs = root_fs;
  }
  inline u32_t block_size(){ return root_fs->block_size;}
  File *fopen(const kstring &pathname);
  bool read_block(u32_t pos,void *dbuffer,Ext2::ext2_inode inode);

};
class File{
  FileSystem *fs;
  kvector<kstring> file_path;
  Ext2::ext2_inode inode;
 public:
  bool is_valid;
  File(){is_valid = false;}
  File(kvector<kstring> &path,Ext2::ext2_inode &inode,FileSystem *filesystem)
    :fs(filesystem),
    file_path(path)
    {
      this->inode=inode;
      is_valid = true;
    }
  inline u32_t block_size(){return fs->block_size();}  
  //  u32_t operator[](u32_t x);
  bool read(u32_t addr,size_t size,void *buffer);
  bool read_block(u32_t addr,size_t size,void *dbuffer);
  
};

#endif
