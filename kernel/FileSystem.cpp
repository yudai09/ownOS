//ファイルを扱いやすくするClass
#include "FileSystem.h"
#include "KGlobal.h"

File *FileSystem::fopen(const kstring &pathname){
  // parse filename into vector of filename
  const char *cname = pathname.to_char();
  kvector<kstring> path(MAX_FILE_NR);
  //  kvector<kstring> path(1);
  //カレントノードからのパス指定はまだ
  File *invalid_file=new File();
  switch(cname[0]){
  case '/':
    goto from_root;  
    break;
  default:
    kprintf("File::invalid path name is specified \n");
    return invalid_file;
  }
  //ルートからのパス指定
 from_root:
  kprintf("len %d \n",pathname.length());
  u32_t cpos,ppos;//パス区切り文字の位置
  ppos = 0;
  cpos = 1;
  u32_t num_file=0;
  /*変な入力されたらおかしくなるかもしれない*/
  while(1){  
    if(cpos==pathname.length() || cname[cpos]=='/' ){
      kprintf("pos %d %d \n",cpos,ppos);
      kstring afile(cname+ppos+1,cpos - ppos -1);
      kprintf("afile %s \n",afile.to_char());
      path[num_file++]=afile;
      ppos = cpos;
    }
    if(cpos==pathname.length())
      break;
    cpos++;
  }
  path.set_nr_elem(num_file);

  kprintf("file name is parsed \n");
  for(int i=0;i<path.length();i++){
    kprintf("%s:",path[i].to_char());
  }
  kprintf("\n");
  kprintf("num files is %d \n",path.length());
  Ext2::ext2_inode inode = root_fs->get_inode_path(path);
  if(!inode.is_valid()){
    return invalid_file;
  }
  File *file=new File(path,inode,this);
  kprintf("fopen end \n");
  //get_inode
  return file;
}

bool FileSystem::read_block(u32_t pos,void *dbuffer,Ext2::ext2_inode inode){
  root_fs->read_block_pos(pos,dbuffer,inode);
  //always true;;
  return true;
}

////////////////////////
//class File
////////////////////////

// u32_t File::operator[](u32_t x){
  
//   fs.read_block()
  
// } 

//使用上の注意　ブロックサイズ以上の読み込みはできない
//要キャッシュ！！
bool File::read(u32_t addr,size_t size,void *dbuffer){
  u8_t *buffer;
  u32_t block_size=fs->block_size();
  u32_t block_mask=fs->block_size()-1;
  kprintf("addr %x size %x \n",addr,size);
  //サイズはブロックサイズ以上ではないか
  if(size>block_size)
    return NULL;
  //読み出し範囲がブロックをまたぐか
  // またぐ場合は二つブロックを読む

  if((addr&block_mask+size)/block_size!=0){
    buffer = new u8_t[block_size*2];
    kprintf("read 2 block from %x \n",addr&~block_mask);
    fs->read_block(addr&~block_mask,buffer,inode);
    fs->read_block(addr&~block_mask+block_size,buffer+block_size,inode);
  }
  else{
    buffer = new u8_t[block_size];
    kprintf("read 1 block from %x \n",addr&~block_mask);
    fs->read_block(addr&~block_mask,buffer,inode);
  }
  kprintf("read buffer %x %x %x \n",buffer[0],buffer[1],buffer[2]);
  memcpy(dbuffer,buffer+(addr&block_mask),size);
  delete[] buffer;//キャッシュしていないので開放する
  return true;
  //  return (void *)(buffer+(addr&block_mask));
}
