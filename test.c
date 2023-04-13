#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"
int func_thread(void * args){
	exit();
}

void Test_thread_create(void){
	th_t tid[3];
	int args[3],k;
	for(k = 0; k < 3; k++) {
		printf(1,"thread creation test ");
		args[k] = 10 + k;
		th_t thid = thread_create(&tid[k], func_thread, &args[k]);
		if(thid > 0){
			printf(1,"%d - passed!\n",k+1);
		}
		else{
			printf(1,"%d - failed!\n",k+1);
		}
		sleep(1);
	}

	for(int l = 0 ; l < 3 ; l++){
		printf(1,"thread exit test ");
		if(l == 1){
			thread_kill(tid[l]);
		}
		int reval = thread_join(tid[l]);
		if(reval > 0){
			printf(1,"%d - passed!\n",l+1);
		}
		else{
			printf(1,"%d - failed!\n",l+1);
		}
	}
	return ;
}

int main(){
	Test_thread_create();
	exit();
}