/**
 *	MyOS
 *	Copyright (C) 2010 - 2011 Samy Pess
 *	
 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef __ATA__
#define __ATA__

#include "kcom.h"
#include "Device.h"
#include "ioctl.h"

/* #include <runtime/types.h> */
/* #include <core/device.h> */
/* #include <io.h>  */

/*
 *	Ata info
 */
#define ATA_SR_BSY     0x80
#define ATA_SR_DRDY    0x40
#define ATA_SR_DF      0x20
#define ATA_SR_DSC     0x10
#define ATA_SR_DRQ     0x08
#define ATA_SR_CORR    0x04
#define ATA_SR_IDX     0x02
#define ATA_SR_ERR     0x01

#define ATA_ER_BBK      0x80
#define ATA_ER_UNC      0x40
#define ATA_ER_MC       0x20
#define ATA_ER_IDNF     0x10
#define ATA_ER_MCR      0x08
#define ATA_ER_ABRT     0x04
#define ATA_ER_TK0NF    0x02
#define ATA_ER_AMNF     0x01

#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01
 
#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01
 
// Directions:
#define      ATA_READ      0x00
#define      ATA_WRITE     0x01

struct IDEChannelRegisters {
  u16_t base;  // I/O Base.
  u16_t ctrl;  // Control Base
  u16_t bmide; // Bus Master IDE
  u8_t  nIEN;  // nIEN (No Interrupt);
};

struct ide_device {
  u8_t  Reserved;    // 0 (Empty) or 1 (This Drive really exists).
  u8_t  Channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
  u8_t  Drive;       // 0 (Master Drive) or 1 (Slave Drive).
  u16_t Type;        // 0: ATA, 1:ATAPI.
  u16_t Signature;   // Drive Signature
  u16_t Capabilities;// Features.
  u32_t   CommandSets; // Command Sets Supported.
  u32_t   Size;        // Size in Sectors.
  u8_t  Model[41];   // Model in string.
};

/*
 *	Ata class
 */
/* : public Device */
class Ata : public Device
{
 public:
  Ata(char* n);		
  ~Ata();
  Ata(char* n,u8_t drive);
		
  u32_t		open(u32_t flag);
  u32_t		close();
  u32_t		read(u32_t pos,u8_t* buffer,u32_t size);
  u32_t		write(u32_t pos,u8_t* buffer,u32_t size);
  u32_t		write(u8_t* buffer,u32_t size);

  u32_t		ioctl(u32_t id,u8_t* buffer);
  u32_t		remove();
  void	scan();
  /* File*	create(char* n,u8_t t); */
		
		
  void	ide_initialize(u32_t BAR0,u32_t BAR1,u32_t BAR2,u32_t BAR3,u32_t BAR4);
		
  u8_t		ide_read(u8_t channel,u8_t reg);
  void	ide_write(u8_t channel,u8_t reg,u8_t data);
		
  void	ide_read_buffer(u8_t channel,u8_t reg,u32_t buffer,u32_t quads);
  u8_t		ide_polling(u8_t channel, u32_t advanced_check);
  u8_t 		ide_print_error(u32_t drive,u8_t err);
		
  u8_t		ide_ata_access(u8_t direction,u8_t drive,u32_t lba, u8_t numsects,u16_t selector,u32_t edi);
  /* u8_t 		ide_atapi_read(u8_t drive,u32_t lba,u8_t numsects,u16_t selector,u32_t edi); */
		
  void	ide_wait_irq();
  void	ide_irq();
		
  void	ide_read_sectors(u8_t drive,u8_t numsects,u32_t lba,u16_t es,u32_t edi);
  void	ide_write_sectors(u8_t drive,u8_t numsects,u32_t lba,u16_t es,u32_t edi);
		
  void	ide_atapi_eject(u8_t drive);
		
  void	sleep(int t);
		
  static u8_t ide_irq_invoked;
		
 private:
  u8_t	ata_drive;
		
	
  static int isInit;
		
  char					package[8];
		
  static IDEChannelRegisters		channels[2];		//ata ide channel
  static ide_device				ide_devices[4];		//ata ide device
		
		
  //u8_t ide_buf[2048] = {0};
  static u8_t atapi_packet[12];

};

#endif
