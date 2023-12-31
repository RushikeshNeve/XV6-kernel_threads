typedef struct RSThread{
  struct RSThread *next;
  void *arg;
  char *stack;
  int kernel_tid;
}RSThread;

RSThread *Head;

struct tickLock{
  int tick;
};

void tickLock_init(struct tickLock *lock);
void tickLock_acquire(struct tickLock *lock);
void tickLock_release(struct tickLock *lock);

int thread_create(int (*fn) (void *), void *arg);
int thread_join(int tid);
int thread_exit(int tid);