#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"

int global = 10;

int func_thread(void * args){
	exit();
}

int func_kill(void * args){
	while(1);
	exit();
}

int func_args(void * args){
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
		tid[k] = thread_create( func_thread, &args[k]);
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
	tid = thread_create(func_thread,&arg);
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

void Test_args(void){
	int tid;
	int arg  = global;
	tid = thread_create(func_args,&arg);
	thread_join(tid);
	return;
}

int main(){
	Test_thread_create();
	Test_multiple_thread_create();
	Test_thread_kill();
	Test_args();
	exit();
}