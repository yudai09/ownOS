#include "Ext2.h"

static u32_t block_size;
static u32_t inode_size;
static ext2_super_block *super_block;
/*These values are not accurate, but ok */
static u32_t super_block_offset = 0x400;
static u32_t group_desc_offset = 0x800;
Ata *atadriver;
/*fist block address is now 0x7e00 (this is not accurate..)*/
static u32_t first_blockaddr = 0x7e00;
ext2_inode root_inode;

u32_t block_index2addr(u32_t index){
  return first_blockaddr + index*block_size;
}

ext2_inode get_inode(u32_t inode_index){
  u32_t block_group_of_inode = (inode_index -1) / super_block->inodes_per_group();
  u32_t index_in_bgroup = (inode_index -1) % super_block->inodes_per_group();
  //read block group descriptor
  u32_t addr_bgroup_desc = (first_blockaddr + block_group_of_inode 
                            * super_block->blocks_per_group() * block_size) + group_desc_offset;
  ext2_group_desc *bgroup = new ext2_group_desc();
  atadriver->read(addr_bgroup_desc,(u8_t *)bgroup,sizeof(ext2_group_desc));
  ext2_inode *inode =  new ext2_inode();
  atadriver->read(block_index2addr(bgroup->bg_inode_table)+index_in_bgroup*sizeof(ext2_inode),
                  (u8_t *)inode,sizeof(ext2_inode));
  return *inode;  
}
ext2_inode find_inode_from_direntries(u8_t *buffer,kstring &search_for){
  ext2_inode invalid_inode;
  ext2_dir_entry_2 *file=(ext2_dir_entry_2 *)buffer;
    do{
      kstring file_name(file->name,file->name_len);
      kprintf("inode %x filename %s lec_ren %x filetype %x\n"
              ,file->inode,file_name.to_char(),file->rec_len,file->file_type);    
      if(search_for==file_name){
        return get_inode(file->inode);
      }
      file = (ext2_dir_entry_2 *)((u32_t)file + file->rec_len);
    }while((u32_t) file < (u32_t)buffer + 0x400);  
    return invalid_inode;
}
ext2_inode get_inode_path(kvector<kstring> path,ext2_inode inode){
//default inode = root inode
  ext2_inode current_node = inode;
  ext2_inode invalid_inode;

  for(int i=0;i<path.length();i++){
    kprintf("%s:",path[i].to_char());
  }
  kprintf("\n");

  for(int i=0;i<path.length();i++){
    if(!current_node.is_directory())
      return invalid_inode;
    u32_t addr_of_dir_info = block_index2addr(current_node.i_block[0]);
    kprintf("addr_of_dir_info %x \n",addr_of_dir_info);
    u8_t *buffer = (u8_t *)kmalloc(0x400);
    atadriver->read(addr_of_dir_info,buffer,0x400);
    current_node = find_inode_from_direntries(buffer,path[i]);
    if(!current_node.is_valid()){
      kprintf("invalid node %x \n",current_node.i_mode);
      return invalid_inode;
    }
  }
  return current_node;
}
/*
  read inode

  (b = block size)

  num of block < 12                       ->  direct block (12)
  num of block < 12+b/4               -> first indirect block
  num of block < (b/4)^2+(b/4)+12         -> second inderect bloc
  num of block < (b/4)^3+(b/4)^2+(b/4)+12 -> third inderect block
*/
void read_block(u32_t block_index,u8_t *buffer){
  u32_t addr = block_index2addr(block_index);
  atadriver->read(addr,buffer,block_size);
}


bool read_block(ext2_inode node,u32_t pos){
  int accessblock = pos / block_size;
  if(pos+block_size > node.i_size){
    kprintf("exceed size limit %x, pos %x \n",node.i_size,pos);
    return false;
  }
  /* te ki to */
  int nr_entry = block_size / 4;
  int maxnr_dir_block = 12;
  int maxnr_Ind_block = 12+nr_entry;
  int maxnr_Dind_block = maxnr_Ind_block+(nr_entry)*(nr_entry);
  int maxnr_Tind_block = maxnr_Dind_block+(nr_entry)*(nr_entry)*(nr_entry);
  
  u8_t *buffer = new u8_t[block_size];

  if(accessblock < maxnr_dir_block){
    read_block(node.i_block[accessblock],buffer);
    kprintf("Dir %x \n",buffer[0],block_size);
  }else if(accessblock < maxnr_Ind_block){
    read_block(node.i_block[ext2_inode::EXT2_IND_BLOCK],buffer);
    int block_index = ((u32_t *)buffer)[accessblock-maxnr_dir_block];
    read_block(block_index,buffer);
  }else if(accessblock < maxnr_Dind_block){
    read_block(node.i_block[ext2_inode::EXT2_DIND_BLOCK],buffer);
    int first_index = ((u32_t *)buffer)[(accessblock-maxnr_Ind_block)/nr_entry];
    read_block(first_index,buffer);
    int second_index = ((u32_t *)buffer)[(accessblock-maxnr_Ind_block)%nr_entry];
    read_block(second_index,buffer);
  }else if(accessblock < maxnr_Tind_block){
    read_block(node.i_block[ext2_inode::EXT2_TIND_BLOCK],buffer);
    int first_index = ((u32_t *)buffer)[(accessblock-maxnr_Dind_block)/(nr_entry*nr_entry)];
    read_block(first_index,buffer);
    int second_index = ((u32_t *)buffer)[((accessblock-maxnr_Dind_block)/nr_entry)%(nr_entry)];
    read_block(second_index,buffer);
    int third_index = ((u32_t *)buffer)[(accessblock-maxnr_Dind_block)%(nr_entry)];
    read_block(third_index,buffer);
  }
  kprintf("read buffer \n");
  for(int i=0;i<0xf;i+=2)
    kprintf("%x%x ",buffer[i],buffer[i+1]);
  kprintf("\n");
}

bool set_ext2_root(Ata *ata){
  atadriver = ata;

  super_block=new ext2_super_block();
  ata->read(first_blockaddr+0x400,(u8_t *)super_block,sizeof(ext2_super_block));// = (ext2_super_block *)0x8200;
  block_size = super_block->block_size();
  inode_size = super_block->inode_size();
  
  ext2_group_desc *group_desc0=new ext2_group_desc();

  ata->read(first_blockaddr+0x400+0x400,(u8_t *)group_desc0,sizeof(ext2_group_desc));

  kprintf("block_size() %x \n",super_block->block_size());
  kprintf("bg_inode_table %x \n",group_desc0->bg_inode_table);

  root_inode = get_inode(2);
  if(root_inode.is_directory()){
    kprintf("root inode size %x type %x \n",root_inode.i_size,root_inode.i_mode);
    
  }
  return true;
}





