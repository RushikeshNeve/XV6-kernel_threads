#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "clonecntl.h"

int func_args(void *args)
{
    int *var = args;
    (*var)++;
    return 0;
}

int y;
int func_files(void *arg)
{
    int y = open("a.txt", O_CREATE | O_RDONLY);
    if (y == -1)
    {
        return 1;
    }
    printf(1, "Child process ID: %d\n", getpid());
    printf(1, "File descriptor: %d\n", y);
    return 0;
}

int func_parent(void *arg)
{
    int *parent_pid = arg;
    *parent_pid = getpid();
    return 0 ;
}

void test_Args()
{
    int var = 1000;
    int x = var;
    char *p = malloc(4096);
    p += 4096;
    int tid = clone(func_args, (void *)p, 0, &var);
    join(tid);
    if(x+1 == var){
        printf(1,"Arguments Done\n");
    }
    else{
        printf(1,"Arguments Failed\n");
    }
    return;
}

void test_Files()
{
    char *stack;
    int pid;
    stack = malloc(4096);
    int fd = open("clone_vm.c", O_CREATE | O_WRONLY);
    sleep(2);
    pid = clone(func_files, stack + 4096, CLONE_FILES, 0);
    if (pid == -1)
    {
        return;
    }
    sleep(2);
    printf(1, "Parent process ID: %d\n", getpid());
    printf(1, "File descriptor: %d\n", fd);
    return;
}

void test_Parent()
{
    char *stack = malloc(4096);
    int clone_flags = CLONE_PARENT;
    int thread_parent_pid = 0;
    int tid = clone(func_parent, stack + 4096, clone_flags,&thread_parent_pid);
    join(tid);
    if(thread_parent_pid == getpid()){
        printf(1,"CLONE_PARENT Done\n");
    }
    else {
        printf(1,"CLONE_PARENT Failed\n");
    }
    return;
}

int main()
{
    printf(1, "Testing for Arguments\n");
    test_Args();
    printf(1, "Testing for files CLONE_FILES\n");
    test_Files();
    printf(1, "Testing for parent CLONE_PARENT\n");
    test_Parent();
    exit();
}