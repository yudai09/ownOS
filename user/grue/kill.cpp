// kill
// Send a signal.

// Minimal implementation:

#include <errno.h>
#undef errno
extern int errno;
int kill(int pid, int sig){
  errno=EINVAL;
  return(-1);
}
