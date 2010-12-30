#include "Ext2.h"

static u32_t block_size;
static u32_t inode_size;

u32_t block_index2addr(u32_t index){
  static u32_t first_blockaddr = 0x7e00;
  return first_blockaddr + index*block_size;
}

bool set_ext2_root(Ata *ata){
  /*今のところ第０ブロックは0x7e00*/
  u32_t first_blockaddr = 0x7e00;
  
  ext2_super_block *super_block=(ext2_super_block *)kmalloc(sizeof(ext2_super_block));
  ata->read(first_blockaddr+0x400,(u8_t *)super_block,sizeof(ext2_super_block));// = (ext2_super_block *)0x8200;
  block_size = super_block->block_size();
  inode_size = super_block->inode_size();
  
  ext2_group_desc *group_desc0=(ext2_group_desc *)kmalloc(sizeof(ext2_group_desc));
  ata->read(first_blockaddr+0x400+0x400,(u8_t *)group_desc0,sizeof(ext2_group_desc));

  kprintf("\n block_size() %x \n",super_block->block_size());
  kprintf("bg_inode_table %x \n",group_desc0->bg_inode_table);
  

  u32_t index_of_root_inode = 2;
  //0
  //u32_t block_group_of_root_inode =  index_of_root_inode / super_block->blocks_per_group();
  //2
  u32_t index_of_root_inode_in_block_group = (index_of_root_inode -1)%super_block->inodes_per_group();
 
  ext2_inode *inode = (ext2_inode *)kmalloc(sizeof(ext2_inode));
  u32_t addr_of_inode = block_index2addr(group_desc0->bg_inode_table) 
    + inode_size * index_of_root_inode_in_block_group;
  ata->read(addr_of_inode,(u8_t *)inode,sizeof(ext2_inode));
  kprintf("imode %x isize %x \n",inode->i_mode,inode->i_size);
  //root inode is retrieved
  
  // `ls' root directory
  // extract first block
  u32_t addr_of_dir_info = block_index2addr(inode->i_block[0]);
  kprintf("addr_of_dir_info %x \n",addr_of_dir_info);
  u8_t *buffer = (u8_t *)kmalloc(0x400);
  ata->read(addr_of_dir_info,buffer,0x400);

  
  ext2_dir_entry *directory=(ext2_dir_entry *)buffer;

  do{
    kprintf("inode %x filename %s lec_ren %x\n",directory->inode,directory->name,directory->rec_len);    
    directory = (ext2_dir_entry *)((u32_t)directory + directory->rec_len);
  }while((u32_t) directory < (u32_t)buffer + 0x400);

  return false;
}




