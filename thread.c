#include "thread.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "clonecntl.h"

void tickLock_init(struct tickLock *lock){
      lock->tick = 0;
}

static inline int fetch_and_add(int* variable, int value)
{
  __asm__ volatile("lock; xaddl %0, %1"
    : "+r" (value), "+m" (*variable) // input + output
    : // No input-only
    : "memory"
  );
  return value;
}

void tickLock_acquire(struct tickLock *lock)
{
	while(fetch_and_add(&lock->tick, 1) != 0);
	__sync_synchronize();
}

void
tickLock_release(struct tickLock *lock)
{
	__sync_synchronize();
	lock->tick = 0;
}

void insert_ThreadList(RSThread * thr){

  if(thr == 0)
    return;
  if(Head == 0){
	  Head = thr;
  }
  else{
	  RSThread *temp = Head;
    while(temp->next != 0){
      temp = temp->next;
    }
    temp->next = thr;
  }
  return;
}

RSThread *searchInThreadList(int tid){

  RSThread *current = Head;
  while (current != 0) {
	if (current->kernel_tid == tid) return current;
	current = current->next;
  }
  return 0;
}

void deleteThread(int tid){
  RSThread *temp=Head , *prev=0;
  while(temp->kernel_tid!=tid && temp->next){
    prev=temp;
    temp=temp->next;
  }
  if(temp==Head){
    Head=temp->next;
    free(temp->stack-4096);
    free(temp);
  }
  else{
    prev->next=temp->next;
    free(temp->stack-4096);
    free(temp);
  }
}

int thread_create(int (*fn) (void *), void *arg) {
	char *stack = malloc(4096)+4096;
	RSThread *thread = (RSThread *)malloc(sizeof(RSThread));
	thread->arg = arg;
	thread->stack = stack;
	thread->next = 0;
	thread->kernel_tid = clone(fn, stack, CLONE_VM | CLONE_THREAD | CLONE_FILES, arg);
	insert_ThreadList(thread);
  sleep(2);
	return thread->kernel_tid;
}

int thread_join(int tid) {
	RSThread *thread = searchInThreadList(tid);
	int jid = join(thread->kernel_tid);
	return jid;
}

int thread_exit(int tid){
	int retval = tkill(tid);
	deleteThread(tid);
	return retval;
}