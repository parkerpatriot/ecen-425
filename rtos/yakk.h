
#ifndef YAKK_H_   /* Include guard */
#define YAKK_H_

#define NULL 0
#define IDLE_STACKSIZE 256

#define EVENT_WAIT_ANY	1
#define EVENT_WAIT_ALL 	2

struct Task{
	int *taskSP;
	unsigned char taskPriority;
	unsigned int taskDelay;
	struct Task* next;
	struct Task* prev;
	int eventPend;
	int waitMode;
};

typedef struct{
	int value;
	char* string;
	struct Task* pendHead;
} YKSEM;

typedef struct{
	unsigned int size;
	unsigned int count;
	int *start;
	int *end;
	int *head;
	int *next;
	struct Task* blockedHead;
} YKQ;

typedef struct{
	unsigned allEvents;
	struct Task* blockedHead;
} YKEVENT;

extern unsigned int YKTickCount;
extern unsigned int YKCtxSwCount;
extern unsigned int YKIdleCount;

YKEVENT* YKEventCreate(unsigned init_val);

unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode);

void YKEventSet(YKEVENT *event, unsigned eventMask);

void YKEventReset(YKEVENT *event, unsigned eventMask);

YKQ* YKQCreate(void **start, unsigned int size);

void* YKQPend(YKQ *queue);

int YKQPost(YKQ *queue, void *msg);

void printYKQ(YKQ *queue);

void printMsgQueue(YKQ *queue);


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


#endif // YAKK_H_