typedef int th_t;
int thread_create(th_t *tid, int (*fn) (void *), void *arg);
int thread_join(th_t tid);