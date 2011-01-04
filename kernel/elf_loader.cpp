#include"kcom.h"
#include "elf.h"
#include "ata.h" //finally erase this line
#include "Ext2.h"

//read 32bit x86 executables only.
//test program for block device driver
//static const char magic[] = "\177ELF";


bool load_elf_executable(kvector<kstring> &path,Ext2 &fs){
  u8_t *buffer = new u8_t[fs.blocksize()];
  fs.read_cn_block(0,buffer);
  Elf32_Ehdr x86_32_xh;
  {
    strncpy((char *)(x86_32_xh.e_ident),ELFMAG,SELFMAG);
    x86_32_xh.e_ident[0x4]=1;//32bit
    x86_32_xh.e_ident[0x5]=1;//little endian
    x86_32_xh.e_ident[0x6]=1;//always one
    //[0x7-0x10] padding
    x86_32_xh.e_ident[0x7]=0;
    x86_32_xh.e_ident[0x8]=0;
    x86_32_xh.e_ident[0x9]=0;
    x86_32_xh.e_ident[0xa]=0;
    x86_32_xh.e_ident[0xb]=0;
    x86_32_xh.e_ident[0xc]=0;
    x86_32_xh.e_ident[0xd]=0;
    x86_32_xh.e_ident[0xe]=0;
    x86_32_xh.e_ident[0xf]=0;
  }
  
  Elf32_Ehdr &header = *(Elf32_Ehdr *)buffer;
  //kprintf("%s \n",header->e_ident);

  /*
   * indentification check
   */
  for(int i=0;i<sizeof(header.e_ident);i++){
    if(x86_32_xh.e_ident[i]!=header.e_ident[i]){
      kprintf("loading file is not executable or x86 format\n");
      return false;
    }
  }
  /*
   * file type check
   */
  switch(header.e_type){
  case ET_EXEC:
    break;
  default:
    kprintf("File type is not supported now \n");
    return false;
  }
  /*
  * archtecture x86
  */
  if(header.e_machine != EM_386){
    kprintf("not x86 executable %x\n",header.e_machine);
    return false;
  }
  
}
