#include "types.h"
#include "stat.h"
#include "user.h"

#define N 10
int sum(void *arg)
{

  int x = 50;
  int y = 11;
  return x + y;
}

void clonetest(void)
{
  int n, pid;
  void *p = 0, *arg = 0;
  int flag = 0;

  printf(1, "clone test\n");
  int x = 0;
  for (n = 0; n < N; n++)
  {
    pid = clone(sum, p, flag, arg);
    if (pid < 0)
      break;
    if (pid == 0)
    {
      sleep(50);
      printf(1, "%d\n", x);
      exit();
    }
    else
    {
      x++;
      wait();
    }
  }

  if (n == N)
  {
    printf(1, "clone claimed to work N times!\n", N);
    exit();
  }

  printf(1, "clone test OK\n");
}

int main(void)
{
  clonetest();
  exit();
}
