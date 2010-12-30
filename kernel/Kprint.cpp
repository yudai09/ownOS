#include"com.h"
#include"Kprint.h"


//$B%3%s%9%H%i%/%?4X?t(B
Kprint::Kprint(){

	  charMonitor=(u16_t *)CharMonitorAddr;//$B%b%K%?$N%"%I%l%9$r$$$l$k(B
	  color=0x0f;
	  fleshdata=(color<<8)|0x20;
	  fresh();
}
void Kprint::setColor(u8_t nextcolor){
	color=nextcolor;
	fleshdata=(color<<8)|0x20;
}
void Kprint::fresh(){
  int i;
  column=0;//$B8=:_JT=8Cf$N9T$*$h$SNs$r=i4|2=(B
  row=0;
  for(i=0;i<80*25-1;i++)
	  charMonitor[i]=fleshdata;
}
void Kprint::fresh(u8_t nextcolor){
	color=nextcolor;
	fleshdata=(color<<8)|0x20;
	fresh();
}
void Kprint::print(const char *Message){
	print(Message,color);//デフォルトカラーでのPrint
}
void Kprint::print(const char *Message,u8_t dcolor){
	int len=strlen(Message);

	for(int i=0;len>0;i++,len--){
		if(column>=80){
			//$B2#$,#2#5$rD6$($?$i=D$r$R$H$DA}$d$7$F2#$r#0$+$i;O$a$k(B
			row++;
			column=0;
		}
		if(row>=25){
			//$B=D$,#8#09T$rD6$($?$i0l9T$:$D$:$i$7$F$$$/(B
			//$BL$<BAu(B
			for(int j=0;j<80*24-1;j++)
				charMonitor[j]=charMonitor[j+80];
			row=24;
			column=0;
		}
		int where=(row*80+column);

		u8_t *ByteP=(u8_t *)&(charMonitor[where]);
		*ByteP=Message[i];
		ByteP++;
		*ByteP=dcolor;
		column++;
	}
	while(column<80){
		u16_t where=(row*80+column);
		u8_t *ByteP=(u8_t *)&(charMonitor[where]);
		*ByteP=' ';
		ByteP++;
		*ByteP=dcolor;
		column++;
	}


	if(row!=25)
		row++;
	column=0;
}
void Kprint::printNum(int num){
	char str[32];
	print(num2str(num,str));
}
void Kprint::printNum(u32_t num){
	char str[1024];
	print(num2str_hex(num,str));
}
static void putcp(void* p,char c)
{
	*(*((char**)p))++ = c;
}


void Kprint::tk_printf(char *s,char *fmt, ...)
{
	va_list va;
	char *s_b=s;
	va_start(va,fmt);

	tfp_format(&s,putcp,fmt,va);
	putcp(&s,0);
	print(s_b);
	va_end(va);
}
