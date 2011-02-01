#include"kcom.h"
#include"FileSystem.h"
#include"elf_loader.h"
// 引数はオープン済みの実行ファイルのファイルディスクリプタ
// 実行ファイルの引数はとれない
int exec(File *file){
  //ELF形式にのみ対応
  load_elf_executable(file);
}
