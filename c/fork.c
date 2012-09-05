#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
  int i, a;
  for(i = 0; i < 2; i++) {
    fork();
    printf("ppid=%d, pid=%d, i=%d \n", getppid(), getpid(), i);
  }
  sleep(10);
  return 0;
}
