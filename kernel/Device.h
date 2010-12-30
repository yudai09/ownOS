#ifndef DEVICE_H
#define DEVICE_H

#include"kcom.h"

class Device
{
 public:
  Device(char* n);
  ~Device();
		
  /*v*/ u32_t		open(u32_t flag);
  /*v*/ u32_t		close();
  /*v*/ u32_t		read(u8_t* buffer,u32_t size);
  /*v*/ u32_t		write(u8_t* buffer,u32_t size);
  /*v*/ u32_t		ioctl(u32_t id,u8_t* buffer);
  /*v*/ u32_t		remove();
  /*v*/ void	scan();
  //  /*v*/ File* 	create(char* n,u8_t t);
};

#endif
