#include "elf.h"
#include "elf_loader.h"
#include "asmfunc.h"
#include "task.h"
#include "KGlobal.h"

//read 32bit x86 executables only.
//test program for block device driver
//static const char magic[] = "\177ELF";


bool load_elf_executable(File *file){

  //  fs.read_cn_block(0,buffer);
  //  file->read
  
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
  
  Elf32_Ehdr header;
  file->read(0,sizeof(header),&header);
  //  Elf32_Ehdr &header = *(Elf32_Ehdr *)buffer;
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
  kprintf("ehsize %x phoff %x shoff %x \n ",header.e_ehsize,header.e_phoff,header.e_shoff);
  u32_t phoff = header.e_phoff;//+sizeof(header);
  u32_t shoff = header.e_shoff;//+sizeof(header);
  Elf32_Phdr *phdr=new Elf32_Phdr[header.e_phnum];
  Elf32_Shdr *shdr=new Elf32_Shdr[header.e_shnum];
  file->read(phoff,sizeof(Elf32_Phdr)*header.e_phnum,phdr);
  file->read(shoff,sizeof(Elf32_Shdr)*header.e_shnum,shdr);

  kprintf("entry point %x num %x \n",header.e_entry,header.e_phnum);
  for(int i=0;i<header.e_phnum;i++){
    switch(phdr->p_type){
    case PT_LOAD:
      kprintf("pt_load:");
      kprintf("(type %x offset %x vaddr %x size %x) \n",
              phdr->p_type,phdr->p_offset,phdr->p_vaddr,phdr->p_memsz);
      // int nr_pages = phdr->p_memsz/PFAllocator::ChunkSize;//ページ数
      // u32_t **pages = new u32_t *[nr_pages];
      /*ページテーブルを設定
       領域も確保される*/
      //      cli_asm();//一応排他処理

      varMem.enableSpace((u32_t *)phdr->p_vaddr,(u32_t)phdr->p_memsz,
                         (entry_t *)pManager.pCurrent->mm->cr3,
                         VarMem::userpage);
      // sti_asm();
      //      for(;;);
      int block_size = file->block_size();
      u32_t dest_addr =  (u32_t)phdr->p_vaddr;
      int nr_blocks = phdr->p_memsz/block_size;
      for(int i=0;i<nr_blocks;i++){
        file->read(phdr->p_offset+i*block_size,block_size,(u32_t *)(dest_addr+i*block_size));
      }
      //残り
      u32_t remainsz = phdr->p_memsz - nr_blocks * block_size;
      file->read(phdr->p_offset+nr_blocks*block_size,remainsz,(u32_t *)(dest_addr+nr_blocks*block_size));
      kprintf("source %x dest %x size %x \n",
              phdr->p_offset+nr_blocks*block_size,(u32_t *)(dest_addr+nr_blocks*block_size),remainsz);
      break;
    }
    phdr++;
  }
  // void (*entry_point)()=(void (*)())header.e_entry;
  // kprintf("entrypoint %x \n",(u32_t)entry_point);
  //  entry_point();
  after_exec(header.e_entry);
}
