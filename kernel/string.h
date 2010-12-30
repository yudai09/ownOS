#ifndef STRINGH
#define STRINGH
#include"Type.hpp"
#define STRLEN_MAX   0x400

typedef unsigned int size_t;
void *memset(void* buf, int value, size_t size);
void bzero(void *s,int  n);
int strlen(const char *str);
char *num2str(int num,char *str);
char *num2str(u32_t num,char *str);
char *strcat(char *dest,char *str1,char *str2);
void strcpy(char *d,const char *s);
void strncpy(char *d,const char *s,size_t len);
extern "C" {void *memcpy(void *s1,const void* s2,size_t n);}
void *memcpy(void *s1,const void* s2,size_t n);
unsigned short strcmp(char *s,char *s2,int num);
extern "C" {void *memmove(void* to, const void* from, register size_t n);}
void* memmove(void* to, const void* from, register size_t n);
void strcpy(char *d,const char *s);
char *num2str_hex(u32_t num,char *str);
#endif
