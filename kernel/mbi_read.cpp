/* kernel.c - the C part of the kernel */
/* Copyright (C) 1999  Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include "multiboot.h"
#include "tk_printf.h"
#include "kcom.h"
#include "elf.h"
#include "KGlobal.h"
/* Macros. */

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))


/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */

void mbi_read(void *addr,unsigned int magic)
{
  multiboot_info_t *mbi;

  /* Clear the screen. */
  tk_cls ();

  /* Am I booted by a Multiboot-compliant boot loader? */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
      tk_printf ("Invalid magic number: 0x%x\n", (unsigned) magic);
      return;
    }

  /* Set MBI to the address of the Multiboot information structure. */
  mbi = (multiboot_info_t *) addr;

  /* Print out the flags. */
  tk_printf ("flags = 0x%x\n", (unsigned) mbi->flags);

  /* Are mem_* valid? */
  if (CHECK_FLAG (mbi->flags, 0))
    tk_printf ("mem_lower = %uKB, mem_upper = %uKB\n",
	       (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

  /* Is boot_device valid? */
  if (CHECK_FLAG (mbi->flags, 1))
    tk_printf ("boot_device = 0x%x\n", (unsigned) mbi->boot_device);

  /* Is the command line passed? */
  if (CHECK_FLAG (mbi->flags, 2))
    tk_printf ("cmdline = %s\n", (char *) mbi->cmdline);
	
  /* Are mods_* valid? */
  if (CHECK_FLAG (mbi->flags, 3))
    {
      multiboot_module_t *mod;
      int i;
      /*
      tk_printf ("mods_count = %d, mods_addr = 0x%x\n",
		 (int) mbi->mods_count, (int) mbi->mods_addr);
      for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
	   i < mbi->mods_count;
	   i++, mod++)
		tk_printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
		   (unsigned) mod->mod_start,
		   (unsigned) mod->mod_end,
		   (char *) mod->cmdline);
	*/
    }

  /* Bits 4 and 5 are mutually exclusive! */
  if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
      tk_printf ("Both bits 4 and 5 are set.\n");
      return;
    }

  /* Is the symbol table of a.out valid? */
  if (CHECK_FLAG (mbi->flags, 4))
    {
      multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

      /*		tk_printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
			"strsize = 0x%x, addr = 0x%x\n",
			(unsigned) multiboot_aout_sym->tabsize,
			(unsigned) multiboot_aout_sym->strsize,
			(unsigned) multiboot_aout_sym->addr);*/
    }



  /* Are mmap_* valid? */
  if (CHECK_FLAG (mbi->flags, 6))
    {
      multiboot_memory_map_t *mmap;

      /*		tk_printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
			(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);*/

      bootinfo.mem_size=0;
      for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
	   (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
	   mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
					      + mmap->size + sizeof (mmap->size))){

	/*
	 * 空き領域を登録する作業
	 * ここの部分は３２BIT用にアドレスの上部を破棄している
	 * カーネル領域も空き領域に含まれているのでそれは後で除外する
	 * */

	if(mmap->type==0x01 || mmap->type==0x03){
	  	tk_printf (" size = 0x%x, base_addr = 0x%x%x,"
		" length = 0x%x%x, type = 0x%x\n",
		(unsigned) mmap->size,
		mmap->addr_high,
		mmap->addr_low,
		mmap->len_high,
		mmap->len_low,
		(unsigned) mmap->type);
	  u32_t addr_end=mmap->addr_low+mmap->len_low;
	  if(bootinfo.mem_size<addr_end)
	    bootinfo.mem_size =  addr_end;
	  pFAllocator.addChunk(mmap->addr_low,mmap->addr_low+mmap->len_low);

	}
      }
      //kprintf("max addr = %x \n",bootinfo.mem_size);
    }
  /* Is the section header table of ELF valid? */
  if (CHECK_FLAG (mbi->flags, 5))
    {
      multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);
	    
      // tk_printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
      // 		 " addr = 0x%x, shndx = 0x%x\n",
      // 		 (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
      // 		 (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
      Elf32_Shdr *entry=(Elf32_Shdr *) multiboot_elf_sec->addr;
      for(int i=0;i<multiboot_elf_sec->num;i++){
	/*tk_printf("type %x,addr %x,offset %x,size %x \n",
	  entry->sh_type,entry->sh_addr,entry->sh_offset,entry->sh_size);*/
	/*カーネルがロードされているメモリ領域を空き領域から除外する*/

	pFAllocator.removeChunk(entry->sh_addr,entry->sh_addr+entry->sh_size);
	/**/
	entry++;
      }

    }
	
}


