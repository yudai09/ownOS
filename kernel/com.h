#ifndef COM_H_
#define COM_H_

#include"Type.hpp"
#include"string.h"
//macro
#define MAX(x,y) ((x>y)? x:y)
#define MIN(x,y) ((x<y)? x:y)
#define ABS(x) ((x>0)? x:-x)
#define ALIGN_4K(x) ((x&~(0xfff)))
#define MOD_4K(x) (x&0xfff)
#define CONCAT(x,y) x##y
#define TRUE	1
#define FALSE	0
#define NULL 0
#endif
