#include"kcom.h"
#include"FileSystem.h"
#include"elf_loader.h"
// �����ϥ����ץ�Ѥߤμ¹ԥե�����Υե�����ǥ�������ץ�
// �¹ԥե�����ΰ����ϤȤ�ʤ�
int exec(File *file){
  //ELF�����ˤΤ��б�
  load_elf_executable(file);
}
