#include "types.h"
#include "stat.h"
#include "user.h"
int change(void* args) {
    printf(1,"Clone worked!!!\n");
    int *var = args;
    printf(1,"Variable value child before: %d\n", *var);
    (*var)++;
    printf(1,"Variable value child after: %d\n", *var);
    return 0;
}

int main(int argc, char* argv[]) {
    int var = 1000;
    char *p = malloc(4096);
    p += 4096;
    printf(1,"Variable value parent before: %d\n", var);
    sleep(5);
    int tid = clone(change, (void*)p, 0, &var);
    sleep(5);
    printf(1,"Variable value parent after: %d\n", var);
    printf(1,"TID - %d\n",tid);
    printf(1,"parent about to exit\n");
    exit();
}