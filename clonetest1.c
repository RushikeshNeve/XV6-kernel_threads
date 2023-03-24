#include "types.h"
#include "stat.h"
#include "user.h"
// #include "defs.h"

int sum(void *arg)
{
  printf(1,"cat: write error");
  int x=50;
  int y=11;
  return x+y;
}

int
main()
{ 
  void *p=0,*arg=0;
  int flag=0;
  // printf(1,"sahil\n");
  clone(sum,p,flag,arg);
  // printf(1,"neve\n");
  exit();
}

