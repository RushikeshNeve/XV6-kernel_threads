#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "clonecntl.h"

int func_args(void *args)
{
    int *var = args;
    (*var)++;
    exit();
}
int func_thread1(void * arg){
    int * var = (int *)arg;
    *var = getpid();
    exit();
}

int func_thread(void *arg){
    int  *childpid = (int  *)arg;
    char * stack = malloc(4096);
    int pid  = clone(func_thread1,(void *)stack,CLONE_THREAD,childpid);
    join(pid);
    exit();
}

int func_files(void *arg)
{
    int * fd = (int *) arg;
    char buf[256];
    int r = read(*fd, buf, 20);
    buf[r] = '\0';
    if(r == 20){
      printf(1,"Failed!\n");
    }else{
      printf(1,"Passed!\n");
    }
    exit();
}

int func_fs(void *arg) {
    int *inoChild = (int *)arg;
    if (mkdir("tmp") < 0) {
        printf(1, "mkdir failed\n");
        return -1;
    }
    if (chdir("tmp") < 0) {
        printf(1, "chdir failed\n");
        return -1;
    }
    struct stat st;
    if (stat(".", &st) < 0) {
        printf(1, "stat failed\n");
        return -1;
    }
    (*inoChild) = st.ino;
    printf(1, "Child inode: %d\n", (*inoChild));
    exit();
}

int func_tkill(void *args)
{
    while(1);
    exit();
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
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    return;
}

void test_Files()
{
    char *stack;
    int pid;
    stack = malloc(4096);
    int fd = open("test.txt", O_CREATE | O_RDWR);
    char *buf;
    buf =  "Modified by Parent!\n";
    write(fd, buf, strlen(buf));
    pid = clone(func_files, stack + 4096,CLONE_FILES, &fd);
    join(pid);
    close(fd);
    free(stack);
    return;
}

void test_FS() {
    char *stack;
    int pid;
    stack = malloc(4096);
    int inoChild = 0;
    pid = clone(func_fs, stack + 4096,CLONE_FS,&inoChild);
    if (pid == -1) {
        printf(1, "clone failed\n");
        return;
    }
    join(pid);
    struct stat  st2;
    if (stat(".", &st2) < 0) {
        printf(1, "stat failed\n");
        return;
    }
    printf(1, "Parent inode: %d\n", st2.ino);
    if (inoChild == st2.ino ) {
        printf(1,"Passed!\n");
    } else {
        printf(1,"Failed!\n");
    }
}

void test_Thread(void){
    char *stack;
    int pid;
    int child_tgid ;
    stack = malloc(4096);
    pid = clone(func_thread, stack + 4096,CLONE_THREAD, &child_tgid);
    if (pid == -1) {
        return;
    }
    join(pid);
    if (child_tgid == getpid()) {
        printf(1,"Passed!\n");
    } else {
        printf(1,"Failed!\n");
    }
    return;
}

void test_tkill(void){
    char *stack;
    int pid;
    int child_tgid ;
    stack = malloc(4096);
    pid = clone(func_tkill, stack + 4096,CLONE_THREAD, &child_tgid);
    if (pid == -1) {
        return;
    }
    int ret =  tkill(pid);
    if(ret == 0) {
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    return;
}

int main()
{
    printf(1, "Testing for Arguments: ");
    test_Args();
    printf(1, "Testing for CLONE_FILES: ");
    test_Files();
    printf(1, "Testing for tkill: ");
    test_tkill();
    // printf(1, "Testing for CLONE_FS: ");
    // test_FS();
    printf(1, "Testing for CLONE_THREAD: ");
    test_Thread();
    exit();
}