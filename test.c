#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"
int x = 20;
int f(void *arg) {
	int p = *(int *)arg;
	printf(1,"%d\n",p);
	exit();
}
int main() {
	th_t tid[3];
	int args[3],k;
		
	for(k = 0; k < 3; k++) {
		args[k] = 10 + k;
		thread_create(&tid[k], f, &args[k]);
		sleep(1);
	}
	for(k = 0; k < 3; k++) {
		thread_join(tid[k]);
	}
	exit();
}