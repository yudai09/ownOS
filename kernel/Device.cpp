#include "Device.h"

Device::~Device(){
	
}

Device::Device(char* n)
{
  //	fsm.addFile("/dev",this);
}

u32_t	Device::open(u32_t flag){
  //	return NOT_DEFINED;
  return 0;
}

u32_t	Device::close(){
  //	return NOT_DEFINED;
  return 0;
}

u32_t	Device::read(u8_t* buffer,u32_t size){
  //	return NOT_DEFINED;
  return 0;
}

u32_t	Device::write(u8_t* buffer,u32_t size){
  //	return NOT_DEFINED;
  return 0;
}

u32_t	Device::ioctl(u32_t id,u8_t* buffer){
  //	return NOT_DEFINED;
  return 0;
}

u32_t	Device::remove(){
  	delete this;
}

void Device::scan(){

}

// File* Device::create(char* n,u8_t t){
// 	return NULL;
// }
