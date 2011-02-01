#include"FileSystem.h"

class File{
  FileSystem *fs;
  kvector<kstring> file_path;
  Ext2::ext2_inode inode;
  bool is_valid;
 public:
  File(){}
  File(kvector<kstring> &path,Ext2::ext2_inode &inode,FileSystem *filesystem)
   :file_path(path),
    fs(filesystem)
    {
      this->inode=inode;
      is_valid = false;
    }
  
  //  u32_t operator[](u32_t x);
  void *read(u32_t addr,size_t size);
  
};
