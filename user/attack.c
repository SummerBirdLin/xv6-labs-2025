#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // Your code here.
  int total_bytes = 4096*16;
  char* buf = sbrk(total_bytes);
  if (buf == (char*)-1) {
    exit(1);
  }
  for (char* i = buf; i < buf + total_bytes - 32; i++){
    if (memcmp(i, "This may help.", 14) == 0) {
      // 找到了！密码就在 p + 16
      printf("%s\n", i + 16);
      exit(0);
    }
  }
  exit(1);
}
