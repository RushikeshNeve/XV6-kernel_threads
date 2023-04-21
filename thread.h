typedef struct RSThread{
  struct RSThread *next;
  void *arg;
  char *stack;
  int kernel_tid;
}RSThread;

RSThread *Head;

int thread_create(int (*fn) (void *), void *arg);
int thread_join(int tid);
int thread_exit(int tid);