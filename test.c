#include "types.h"
#include "stat.h"
#include "user.h"
#include "clonecntl.h"
#include "fcntl.h"
#include "thread.h"

#define MAX 16
#define MAX_THREAD 4
#define NUM_THREADS 4
int global = 10 , c1 = 0,c2 = 0,c = 0,run = 1;

int a[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220 };
int sum[4] = { 0 };
int part = 0;
int array[10] = {82,57,120,118,170,1,97,50,5,22};
int arr1[10] = {82,57,120,118,170,1,97,50,5,22};
int max = 0;

struct thread_data {
    int start;
    int end;
};
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

int max_thread(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    int local_max = 0;
    for(int i = my_data->start; i < my_data->end; i++) {
        if(array[i] > local_max) {
            local_max = array[i];
        }
    }
    if(local_max > max) {
        max = local_max;
    }
    exit();
}

void thread_max_test() {

    struct thread_data thread_data_array[NUM_THREADS];
    int tid[NUM_THREADS];
    int chunk_size = 10 / NUM_THREADS;
    for(int i = 0; i < NUM_THREADS; i++) {
        thread_data_array[i].start = i * chunk_size;
        thread_data_array[i].end = (i + 1) * chunk_size;
        tid[i] = thread_create( max_thread,&thread_data_array[i]);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        thread_join(tid[i]);
    }
    int expected_max = 0;
    for(int i = 0; i < 10; i++) {
        if(array[i] > expected_max) {
            expected_max = array[i];
        }
    }
    printf(1,"Max test ");
    if(max == expected_max) {
        printf(1,"- passed!\n");
    } else {
        printf(1,"- failed!\n");
    }
}

void merge(int start, int mid, int end) {
    int i, j, k;
    int n1 = mid - start + 1;
    int n2 = end - mid;
    int left[n1], right[n2];

    for (i = 0; i < n1; i++)
        left[i] = arr1[start + i];
    for (j = 0; j < n2; j++)
        right[j] = arr1[mid + 1 + j];

    i = 0;
    j = 0;
    k = start;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr1[k] = left[i];
            i++;
        } else {
            arr1[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr1[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr1[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort(int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;
        merge_sort(start, mid);
        merge_sort(mid + 1, end);
        merge(start, mid, end);
    }
}

int sort_thread(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    merge_sort(my_data->start, my_data->end);
    exit();
}

void thread_sort_test() {

    struct thread_data thread_data_array[2];
    int tid[5];
    int chunk_size = 10 / 2;
    for(int i = 0; i < 2; i++) {
        thread_data_array[i].start = i * chunk_size;
        thread_data_array[i].end = (i + 1) * chunk_size;
        tid[i] = thread_create(sort_thread,&thread_data_array[i]);
    }
    for(int i = 0; i < 2; i++) {
        thread_join(tid[i]);
    }
    merge(0, (10 / 2) - 1, 10 - 1);
    int flag = 1;
    for(int i = 0; i < 10 - 1; i++) {
        if(arr1[i] > arr1[i+1]) {
            flag = 0;
            break;
        }
    }
    printf(1,"Sort test ");
    if(flag) {
        printf(1,"- passed!\n");
    } else {
        printf(1,"- failed!\n");
    }
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
    char *stack1 = malloc(4096);
    stack1 += 4096;
    int tid1 = clone(func_args, (void *)stack1, 0, &var);
    join(tid1);
    free(stack1-4096);
    printf(1, "Testing for Arguments(flag not set) : ");
    if(x+1 == var){
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    char *stack2 = malloc(4096);
    stack2 += 4096;
    printf(1, "Testing for Arguments(flag set): ");
    int tid2 = clone(func_args, (void *)stack2, CLONE_VM, &var);
    join(tid2);
    free(stack2-4096);
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
    char *stack1 = malloc(4096);
    stack1 += 4096;
    struct temp *tmp1 = (struct temp *)malloc(sizeof(struct temp));
    tmp1->a = 100;
    tmp1->b = 50;
    tmp1->sum = 0;
    int tid1 = clone(func_multiple_args, (void *)stack1, 0, tmp1);
    join(tid1);
    free(stack1-4096);
    printf(1, "Testing for Multiple Arguments(flag not set): ");
    if(tmp1->sum == (tmp1->a + tmp1->b)){
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    free(tmp1);
    struct temp *tmp2 = (struct temp *)malloc(sizeof(struct temp));
    tmp2->a = 100;
    tmp2->b = 50;
    tmp2->sum = 0;
    char *stack2 = malloc(4096);
    stack2 += 4096;
    int tid2 = clone(func_multiple_args, (void *)stack2, CLONE_VM, tmp2);
    join(tid2);
    free(stack2-4096);
    printf(1, "Testing for Multiple Arguments(flag set): ");
    if(tmp2->sum == (tmp2->a + tmp2->b)){
        printf(1,"Passed!\n");
    }
    else{
        printf(1,"Failed!\n");
    }
    free(tmp2);
    return;
}

int func_files(void *arg)
{
    int * fd = (int *) arg;
    char buf[256];
    int r = read(*fd, buf, 20);
    buf[r] = '\0';
    if(r == 18){
      printf(1,"Failed!\n");
    }else{
      printf(1,"Passed!\n");
    }
    exit();
}

void test_Files()
{
    int fd1 = open("test1.txt", O_CREATE | O_RDWR);
    char *stack1 = malloc(4096);
    stack1 += 4096;
    char *buf1;
    buf1 =  "Modified by tid1!\n";
    write(fd1, buf1, strlen(buf1));
    printf(1, "Testing for CLONE_FILES(flag not set): ");
    int tid1 = clone(func_files, (void *)stack1, 0, &fd1);
    join(tid1);
    close(fd1);
    free(stack1-4096);
    int fd2 = open("test2.txt", O_CREATE | O_RDWR);
    char *stack2 = malloc(4096);
    stack2 += 4096;
    char *buf2;
    buf2 =  "Modified by tid2!\n";
    write(fd2, buf2, strlen(buf2));
    printf(1, "Testing for CLONE_FILES(flag set): ");
    int tid2 = clone(func_files, (void *)stack2, CLONE_FILES, &fd2);
    join(tid2);
    free(stack2-4096);
    close(fd2);
    return;
}

int func_tkill(void *args)
{
    while(1);
    exit();
}

void test_tkill(void){
    char *stack;
    int tid;
    int child_tgid ;
    stack = malloc(4096);
    tid = clone(func_tkill, stack + 4096,CLONE_THREAD, &child_tgid);
    if (tid == -1) {
        return;
    }
    int ret =  tkill(tid);
    free(stack-4096);
    printf(1, "Testing for tkill: ");
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
    printf(1, "Testing for CLONE_FS(flag not set): ");
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

void Test_Exec1(void){
    char *stack;
    stack = malloc(4096);
    clone(func_exec, stack + 4096,0, 0);
    sleep(1);
    return;
}

void Test_Exec2(void){
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
    thread_max_test();
    thread_sort_test();

    printf(1,"\nstarting kernel Test...\n");
    test_Args();
    test_Multiple_Args();
    test_Files();
    test_tkill();
    test_FS();
    printf(1, "Testing for EXEC(flag not set): ");
    Test_Exec1();
    printf(1, "Testing for EXEC(flag set): ");
    Test_Exec2();
    printf(1,"This will not get printed\n");   //this will not get printed as CLONE_thread in exec2 is set so main gets killed
	exit();
}