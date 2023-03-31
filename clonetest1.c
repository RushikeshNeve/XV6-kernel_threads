#include "types.h"
#include "stat.h"
#include "user.h"
int demo(void* args) {
    printf(1,"Clone worked!!!\n");
    return 9;
}

int main(int argc, char* argv[]) {
    int arg = 1;
    char *p = sbrk(4096);
    p += 4096;
    int tid = clone(demo, (void*)p, 0, &arg);
    printf(1,"TID - %d\n",tid);
    printf(1,"parent about to exit\n");
    exit();
}