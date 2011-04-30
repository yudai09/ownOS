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
  //////////////////////////////////////////////
  // for(int i=0;i<10;i++)                    //
  //   printf("hello");//うまくいかない・・・ //
  //////////////////////////////////////////////
  puts("hello");
  for(;;);
}



