#include"string.h"
#include"com.h"

void *memset(void* buf, int value, size_t size) {

    char *p = (char*)buf;

    while (size > 0) {
        *p = value;
        p++;
        size--;
    }
    return buf;
}


int strlen(const char *str){
  int i;
  for(i=0;i<STRLEN_MAX;i++){
    if(str[i]=='\0')
      break;
  }
  return i;
}

char *strcat(char *dest,char *str1,char *str2){
  int len1,len2;
  int i,j;
  len1=strlen(str1);
  len2=strlen(str2);
  for(i=0;i<len1;i++){
    dest[i]=str1[i];
  }
  for(i=len1,j=0;j<len2;i++,j++){
    dest[i]=str2[j];
  }
  return dest;
}
void bzero(void *s, int n){
  char *byte=(char *)s;
  int i;
  for(i=0;i<n;i++){
    byte[i]=0;
  }
}

void strcpy(char *d,const char *s){
  int len=strlen(s);
  for(int i=0;i<=len;i++){
    d[i]=s[i];
  }
}
void strncpy(char *d,const char *s,size_t len){
  len=MIN(strlen(s),len);
  for(int i=0;i<=len;i++){
    d[i]=s[i];
  }
}


char *num2str_hex(u32_t num,char *str){//strは１０文字
	short hexn;
	u8_t hexc;
	str[0]='0';
	str[1]='x';
	for(int i=2;i<sizeof(u32_t)*2+2;i++){//１６進数表示（1バイトが2桁）
		str[i]='0';
	}
	int tail=sizeof(u32_t)*2+1;//
	for(int i=0;num!=0;i++){

		hexn=num%0x10;
		num/=0x10;
		switch(hexn){
		case 10: hexc='a';break;
		case 11: hexc='b';break;
		case 12: hexc='c';break;
		case 13: hexc='d';break;
		case 14: hexc='e';break;
		case 15: hexc='f';break;
		default:
			hexc='0'+hexn;
		}
		str[tail-i]=hexc;
	}
	return str;
}

char  *num2str(int num,char *str){
	int i;
	int tmp;
	u32_t fig=1;
  
  tmp=num;

  if(num<0){
	 str[0]='-';
     num2str(ABS(num),str+1);
     return str;
  }
  else if(num==0){
	  strcpy(str,"0");
	  return str;
  }
  
  
  while(tmp!=0){
    tmp/=10;
    fig*=10;
  }
  fig/=10;
  i=0;

  while(fig!=0){
    str[i++]='0'+num/fig;
    num%=fig;
    fig/=10;
  }

  str[i]='\0';
  //  for(;;);
  //  print(str,i);

  return str;
  }
unsigned short strcmp(const char *s1,const char *s2,int num)
{
   if(strlen(s1)!=strlen(s2))
	   return 1;
	int len=MIN(num,strlen(s1));

	for(int i=0;i<len;i++){
		if(s1[i]!=s2[i])
			return 1;
	}
	return 0;
}
unsigned short strncmp(const char *s1,const char *s2,int num)
{
  for(int i=0;i<num;i++){
    if(s1[i]!=s2[i])
      return 1;
  }
  return 0;
}

typedef unsigned int size_t;

void *memcpy(void *s1,const void* s2,size_t n){
  char *d=(char *)s1;
  char *s=(char *)s2;
  size_t i;
  for(i=0;i<n;i++)
    d[i]=s[i];
  return (void *)NULL;
}//��¤�Τ�ʣ�̤ˤҤĤ褦

void* memmove(void* to, const void* from, register size_t n)
{
        register char*  out = (char*)to;
        register char*  in = (char*)from;

        if (n <= 0)     /* works if size_t is signed or not */
                ;
        else if (in + n <= out || out + n <= in)
                return(memcpy(to, from, n));    /* hope it's fast*/
        else if (out < in)
                do *out++ = *in++; while (--n > 0);
        else
        {
                out += n;
                in += n;
                do *--out = *--in; while(--n > 0);
        }
        return(to);
}
