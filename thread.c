#include "thread.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "clonecntl.h"

th_t thread_create(th_t *tid, int (*fn) (void *), void *arg) {
	int tid1;
	char *stack = malloc(4096);
	tid1 = clone(fn, stack + 4096, CLONE_THREAD, arg);
	*tid = tid1;
	return tid1;
}

int thread_join(th_t tid) {
	int jid = join(tid);
	return jid;
}

int thread_kill(th_t tid){
	return tkill(tid);
}