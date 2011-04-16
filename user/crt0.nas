global _start
extern main
extern start_ctors, end_ctors, start_dtors, end_dtors

section .text

static_ctors_loop:
   mov ebx, start_ctors
   jmp .test
.body:
   call [ebx]
 
   add ebx,4
.test:
   cmp ebx, end_ctors
   jb .body
   call main
static_dtors_loop:
   mov ebx, start_dtors
   jmp .test
.body:
   call [ebx]
   add ebx,4
.test:
   cmp ebx, end_dtors
   jb .body
;;
loop:
   jmp loop

    