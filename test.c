#include "types.h"
#include "stat.h"
#include "user.h"

int sum(void *arg)
{
  int x=10;
  return x;
}

int
main()
{ 
  void *p=0,*arg=0;
  int flag=0;
  clone(sum,p,flag,arg);
  exit();
}

