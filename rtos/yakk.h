
#define NULL 0
#define IDLE_STACKSIZE 256

struct Task{
	int *taskSP;
	unsigned char taskPriority;
	unsigned int taskDelay;
	struct Task* next;
	struct Task* prev;
};

typedef struct{
	int value;
	char* string;
	struct Task* pendHead;
} YKSEM;

extern unsigned int YKTickCount;
extern unsigned int YKCtxSwCount;
extern unsigned int YKIdleCount;

YKSEM* YKSemCreate(int value);

void YKSemPost(YKSEM* sem);

void YKSemPend(YKSEM* sem);

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
void YKSaveSP();

void YKInsertSorted(struct Task* item, struct Task** head);

void YKRemoveSorted (struct Task* item, struct Task** head);

void YKRemoveBlocked(struct Task* item);

void YKInsertBlocked(struct Task* item);

void printStack(struct Task* item);

void printLists(void);


