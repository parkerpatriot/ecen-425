
#define NULL 0
#define IDLE_STACKSIZE 256
#define TASKNUM 10

extern struct Task;
extern int YKtaskCount;
extern int YKRunFlag;
extern int YKIsrDepth;
extern int YKTickCount;
extern int YKCtxSwCount;
extern int YKIdleCount;

extern struct Task* readyHead;
extern struct Task* readyTail;
extern struct Task* blockedHead;
extern struct Task* blockedTail;
extern struct Task* YKRunningTask;

void YKEnterMutex(void);

void YKExitMutex(void);

void YKInitialize(void);

void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority);

void YKRun(void);

void YKDispatcher(int saveContext);

void YKScheduler(int saveContext);

void YKTickHandler(void);

void YKEnterISR(void);

void YKExitISR(void);

void YKDelayTask(unsigned count);

//assembly routine called in YKEnterISR
//saves task context stack pointer to TCB of running task
void YKsaveSP();

void YKinsertReady(struct Task* item);

void YKremoveReady (struct Task* item);

void YKremoveBlocked(struct Task* item);

void YKinsertBlocked(struct Task* item);

void printStack(struct Task* item);

void printLists(void);


