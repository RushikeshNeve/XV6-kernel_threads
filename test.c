#include "types.h"
#include "stat.h"
#include "user.h"
#include "clonecntl.h"
#include "fcntl.h"
#include "thread.h"

#define MAX 16
#define MAX_THREAD 4
int global = 10 , c1 = 0,c2 = 0,c = 0,run = 1;

int a[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220 };
int sum[4] = { 0 };
int part = 0;
struct tickLock lock;

int func_thread1(void * args){
	exit();
}

int func_kill(void * args){
	while(1);
	exit();
}

int func_args1(void * args){
	int p = *(int *)args;

	printf(1,"arguments test");
	if(p == 10){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	exit();
}


void Test_multiple_thread_create(void){
	int tid[3];
	int args[3],k,flag1 = 0,flag2 = 0;
	for(k = 0; k < 3; k++) {
		args[k] = 10 + k;
		tid[k] = thread_create( func_thread1, &args[k]);
		if(tid[k] < 0){
			flag1 = 1;
		}
		sleep(1);
	}

	for(int l = 0 ; l < 3 ; l++){
		int reval = thread_join(tid[l]);
		if(reval < 0){
			flag2 = 1;
		}

	}
	printf(1,"multiple thread creation test ");
	if(flag1 == 0){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	printf(1,"multiple thread join test ");
	if(flag2 == 0){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	return ;
}

void Test_thread_kill(void){
	int tid;
	int arg;
	tid = thread_create( func_kill,&arg);
	int reval = thread_exit(tid);
	printf(1,"thread kill test ");
	if(reval == 0){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
}

void Test_thread_create(void){
	int tid;
	int arg;
	tid = thread_create(func_thread1,&arg);
	printf(1,"thread create test ");
	if(tid > 0){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	int retval = thread_join(tid);
	printf(1,"thread join test ");
	if(retval > 0){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	return;
}

int testlock1(void * args){
	while(run==1){
		tickLock_acquire(&lock);
		c++;
		tickLock_release(&lock);
		c1++;
	}
	exit();

}
int testlock2(void * args){
	while(run==1){
		tickLock_acquire(&lock);
		c++;
		tickLock_release(&lock);
		c2++;
	}
	exit();

}

void Test_Lock(void){
	int tid1,tid2,arg1,arg2;
	tickLock_init(&lock);
	tid1 = thread_create(testlock1, &arg1);
	tid2 = thread_create(testlock2, &arg2);
	run = 0;
	thread_join(tid1);
	thread_join(tid2);
	printf(1,"lock test ");
	if(c1+c2 == c){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}

}

void Test_args(void){
	int tid;
	int arg  = global;
	tid = thread_create(func_args1,&arg);
	thread_join(tid);
	return;
}

int sum_array(void* arg)
{
	int thread_part = part++;

	for (int i = thread_part * (MAX / 4); i < (thread_part + 1) * (MAX / 4); i++)
		sum[thread_part] += a[i];
        exit();
}

void Test_sum()
{

    int tid[MAX_THREAD];
	for (int i = 0; i < MAX_THREAD; i++)
		tid[i] = thread_create(sum_array,0);

	// joining 4 threads i.e. waiting for all 4 threads to complete
	for (int i = 0; i < MAX_THREAD; i++)
		thread_join(tid[i]);

	// adding sum of all 4 parts
	int total_sum = 0;
	for (int i = 0; i < MAX_THREAD; i++)
		total_sum += sum[i];

	printf(1,"Sum test ");
	if(total_sum == 600){
		printf(1,"- passed!\n");
	}
	else{
		printf(1,"failed!\n");
	}
	return;
}

//kernel test:
struct temp{
    int a;
    int b;
    int sum;
};

int func_args(void *args)
{
    int *var = args;
    (*var)++;
    exit();
}

void test_Args(){
    int var = 1000;
    int x = var;
    char *p = malloc(4096);
    p += 4096;
    int tid = clone(func_args, (void *)p, CLONE_VM, &var);
    join(tid);
    free(p-4096);
    if(x+1 == var){
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    return;
}

int func_multiple_args(void *args)
{
    struct temp *var = (struct temp *)args;
    var->sum = var->a + var->b;
    exit();
}

void test_Multiple_Args()
{
    char *p = malloc(4096);
    struct temp *tmp = (struct temp *)malloc(sizeof(struct temp));
    tmp->a = 100;
    tmp->b = 50;
    tmp->sum = 0;
    p += 4096;
    int tid = clone(func_multiple_args, (void *)p, CLONE_VM, tmp);
    join(tid);
    free(p-4096);
    if(tmp->sum == (tmp->a + tmp->b)){
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    return;
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
    free(stack-4096);
    return;
}

int func_tkill(void *args)
{
    while(1);
    exit();
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
    free(stack-4096);
    if(ret == 0) {
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    return;
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
    exit();
}

void test_FS() {
    char *stack;
    int pid;
    stack = malloc(4096);
    int inoChild = 0;
    pid = clone(func_fs, stack + 4096,0,&inoChild);
    if (pid == -1) {
        printf(1, "clone failed\n");
        return;
    }
    join(pid);
    free(stack-4096);
    struct stat  st2;
    if (stat(".", &st2) < 0) {
        printf(1, "stat failed\n");
        return;
    }
    if (inoChild == st2.ino ) {
        printf(1,"Passed!\n");
    } else {
        printf(1,"Failed!\n");
    }
}

int func_exec(void *args)
{
  char *exec_argv[] = {"echo","exec passed!", 0};
  exec(exec_argv[0], exec_argv);
  exit();
}

void Test_Exec(void){
    char *stack;
    stack = malloc(4096);
    clone(func_exec, stack + 4096,CLONE_THREAD, 0);
    sleep(1);
    return;
}

int main(){
    printf(1,"\nstarting usrland Test...\n");
	Test_thread_create();
	Test_multiple_thread_create();
	Test_thread_kill();
	Test_Lock();
	Test_args();
    Test_sum();
    // test_quicksort();

    printf(1,"\nstarting kernel Test...\n");
    printf(1, "Testing for Arguments: ");
    test_Args();
    printf(1, "Testing for Multiple Arguments: ");
    test_Multiple_Args();
    printf(1, "Testing for CLONE_FILES: ");
    test_Files();
    printf(1, "Testing for tkill: ");
    test_tkill();
    printf(1, "Testing for CLONE_FS: ");
    test_FS();
    printf(1, "Testing for EXEC: ");
    Test_Exec();
	exit();
}