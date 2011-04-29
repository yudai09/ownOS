#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>

#include<Ipc.h>
class Test{
  int x;
 public:
  Test(){
    x = 1;
  }
};
Test test;

int main(void){
  //  printf("hello");うまくいかない・・・
  puts("hello");
  for(;;);
}



