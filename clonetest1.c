#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "clonecntl.h"

int func_args(void *args)
{
    printf(1, "Clone worked!!!\n");
    int *var = args;
    printf(1, "Variable value child before: %d\n", *var);
    (*var)++;
    printf(1, "Variable value child after: %d\n", *var);
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

    printf(1, "Child process with PID %d\n", getpid());
    printf(1, "Parent process of child with PID %d is %d\n", getpid(), getppid());
    return 0;
}

void test_Args()
{
    int var = 1000;
    char *p = malloc(4096);
    p += 4096;
    printf(1, "Variable value parent before: %d\n", var);
    sleep(2);
    int tid = clone(func_args, (void *)p, 0, &var);
    sleep(5);
    printf(1, "Variable value parent after: %d\n", var);
    printf(1, "TID - %d\n", tid);
    printf(1, "parent about to exit\n");
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
    clone(func_parent, stack + 4096, clone_flags, 0);
    sleep(5);
    printf(1, "Parent process with PID %d\n", getpid());
    printf(1, "Parent process exiting with PID & parent PID %d %d\n", getpid(), getppid());
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