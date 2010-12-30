#ifndef ERRORH
#define ERRORH
#include"Kprint.h"
#include"pageFault.h"

void error(const char *string);

class Error{
public :
	enum CodeDef{SEGV,PAGEFAULT,OTHER};
	short errorCode;
	union{//CodeDefによって中身が異なる（ように扱う）
		PageFault pageFault;
	};
};
void dumpRegs();


#endif
