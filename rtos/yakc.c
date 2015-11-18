#include "clib.h"
#include "yakk.h"
#include "yaku.h"
#include "lab6defs.h"

#define LAB7
// #define DEBUG

#define INTERRUPTS_ENABLED	0x0200

int* YKsave;
int* YKrestore;

/////////// externs from user code /////////////

extern int KeyBuffer; 	// clib key buffer

#ifdef LAB5
	extern YKSEM *NSemPtr; 	// for Lab 5 application
#endif

#ifdef LAB6
	extern int GlobalFlag;
	extern YKQ *MsgQPtr; 
	extern struct msg MsgArray[];
	static int next = 0;
	static int data = 0;
#endif

/////////////////// YK Vars ////////////////////

int YKtaskCount; 	// how many tasks initialized
int YKRunFlag;		// are we running?
int YKIsrDepth;		// ISR nesting counter
int YKSemCount;
int YKQCount;
int YKEventCount;
volatile int YKIdleVar; 
unsigned int YKTickCount;
unsigned int YKCtxSwCount;
unsigned int YKIdleCount;

////////////// TASK POINTERS /////////////////

struct Task* readyHead;
struct Task* readyTail;
struct Task* blockedHead;
struct Task* blockedTail;
struct Task* YKRunningTask;
struct Task* pendHead;
struct Task* pendTail;

//////// PREALLOCATED MEMORY BLOCKS ////////

struct Task YKTasks[TASKNUM];
int YKIdleStk[STACKSIZE];
YKSEM YKSemaphors[MAXSEM];
YKQ YKQueues[MAXQ];
YKEVENT YKEvents[MAXEVENTS];

void printEventPendingTasks(YKEVENT* event);


////////////////// EVENTS /////////////////////


YKEVENT* YKEventCreate(unsigned init_val){
	YKEVENT* tempEvent;
	tempEvent = &YKEvents[YKEventCount];
	tempEvent->allEvents = init_val;
	tempEvent->blockedHead = NULL;
	YKEventCount++;
	return tempEvent;
}

unsigned YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode) {
	YKEnterMutex();
	// printString("pending on event...\n\r");
	if(!eventMask | event==NULL){
		printString("Error! No events specified!\n\r");
		YKExitMutex();
		return 0;
	}
	else{
		YKRunningTask->waitMode = waitMode;
		YKRunningTask->eventPend = eventMask;
		YKRemoveSorted(YKRunningTask, &readyHead);
		YKInsertSorted(YKRunningTask, &(event->blockedHead));
		YKScheduler(1);
		YKExitMutex();
		return event->allEvents;
	}
}

void YKEventSet(YKEVENT *event, unsigned eventMask) {
	struct Task* tempTask;
	struct Task* nextTask;
	int callScheduler;
	unsigned result;
	// callScheduler = 0;
	YKEnterMutex();
	// printString("posting....\n\r");
	if(!eventMask | event==NULL){
		printString("Error no events specified!\n\r");
		YKExitMutex();
	}
	else{
		event->allEvents = event->allEvents | eventMask;
		if(event->blockedHead != NULL){
			tempTask = event->blockedHead;
			do{
				nextTask = tempTask->next;
				if (tempTask->waitMode==EVENT_WAIT_ANY){
					result = tempTask->eventPend & event->allEvents;
					if(result){
						YKRemoveSorted(tempTask, &(event->blockedHead));
						YKInsertSorted(tempTask, &readyHead);
						// callScheduler = 1;
					}
				}
				else if(tempTask->waitMode==EVENT_WAIT_ALL){
					result = tempTask->eventPend & event->allEvents;
					if(result == tempTask->eventPend){
						YKRemoveSorted(tempTask, &(event->blockedHead));
						YKInsertSorted(tempTask, &readyHead);
						// callScheduler = 1;
					}
				}
				tempTask = nextTask;
			} while(nextTask != NULL);
			if (/*callScheduler && */YKIsrDepth==0){
				YKScheduler(1);
			}
		}
	}
	YKExitMutex();
}

void YKEventReset(YKEVENT *event, unsigned eventMask){
	event->allEvents &= (~eventMask);
}







//////////////// YAK METHODS ///////////////////



void YKIdleTask(void){
	while(1){
		YKIdleCount++;
		if(YKIdleVar){
			YKIdleVar++;
		}
	}
}

void YKInitialize(void){
	YKEnterMutex();
	YKsave = NULL;
	YKrestore = NULL;
	YKTickCount = 0;
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKRunFlag = 0;
	YKtaskCount = 0;
	readyHead = NULL;
	readyTail = NULL;
	blockedHead = NULL;
	blockedTail = NULL;
	YKRunningTask = NULL;
	YKIsrDepth = 0;
	YKSemCount = 0;
	YKNewTask(&YKIdleTask, &YKIdleStk[IDLE_STACKSIZE], 100);
}

void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority){
	struct Task* tempTask;
	int *tempSP;
	int k;
	YKEnterMutex();
	//set up inital stack frame
	tempSP = taskStack-1;
	*tempSP = INTERRUPTS_ENABLED; 	//initial value for flags (0x0200)
	--tempSP;
	*tempSP = 0; 					//initial value for CS
	--tempSP;
	*tempSP = (int)task;		 	//initial value for IP
	--tempSP;
	*tempSP = (int)(taskStack-1);	//initial value for BP
	--tempSP;
	// allocated space for the general purpose registers
	for(k = 0; k <=7; k++){
		*tempSP = 0;	// initialize to zero
		--tempSP;
	}
	
	//initialize TCB
	tempTask = &YKTasks[YKtaskCount];
	tempTask->taskSP = tempSP+1;
	tempTask->taskPriority = priority;
	tempTask->taskDelay = 0;
	
	YKtaskCount++;
	//insert into ready list 
	YKInsertSorted(tempTask, &readyHead);
	#ifdef DEBUG
		printStack(tempTask);
		printLists();
	#endif
	if (YKRunFlag == 1){
		YKScheduler(1);
		YKExitMutex();
	}
}

// start running tasks
void YKRun(void) {
	if (readyHead != NULL){
		YKRunFlag = 1;
		YKrestore = readyHead->taskSP;
		YKRunningTask = readyHead;
		YKCtxSwCount++;
		YKDispatcher(0);
	}
	else {
		printString("YKRun Failed: readyHead is Null");
	}
}

// delay task by given number of ticks
void YKDelayTask(unsigned count){
	struct Task* item;
	if (count != 0){
		YKEnterMutex();
		item = YKRunningTask;
		item->taskDelay = count;
		YKRemoveSorted(item, &readyHead);
		YKInsertBlocked(item);
		#ifdef DEBUG
			printLists();
		#endif
		YKScheduler(1); 
		YKExitMutex();
	}
}

void YKExitISR(void){
	YKIsrDepth--;
	if(YKIsrDepth == 0){
		YKScheduler(0);
	}
}

void YKEnterISR(void){
	YKIsrDepth++;
	if (YKIsrDepth == 1){
		YKsave = (int*)&(YKRunningTask->taskSP);
		YKSaveSP();
	}
}

void YKScheduler(int saveContext){
	if (YKRunningTask != readyHead){
		YKsave = (int*)&(YKRunningTask->taskSP);
		YKrestore = readyHead->taskSP;
		YKRunningTask = readyHead;
		YKCtxSwCount++;
		YKDispatcher(saveContext);
	}
}

/////////////////// ISR HANDLERS ///////////////////

//assuming tick isr is highest priority
void YKTickHandler(void){
	struct Task* temp;
	struct Task* temp_next;
	#if !defined(LAB6) && !defined(LAB7)
		printString("TICK ");
		printInt(YKTickCount);
		printString("\n\r\n\r");
	#endif
	YKTickCount++;

	////////////////// LAB 6 CODE ///////////////////
	#ifdef LAB6
	    /* create a message with tick (sequence #) and pseudo-random data */
	    MsgArray[next].tick = YKTickCount;
	    data = (data + 89) % 100;
	    MsgArray[next].data = data;
	    if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0)
		printString("  TickISR: queue overflow! \n");
	    else if (++next >= MSGARRAYSIZE)
			next = 0;
	#endif
	////////////// END OF LAB 6 CODE //////////////
	
	temp = blockedHead;
	//printLists();
	while(temp != NULL){
		temp_next = temp->next;
		(temp->taskDelay)--;
		if (temp->taskDelay <= 0){
			YKRemoveBlocked(temp);
			YKInsertSorted(temp, &readyHead);
		}
		if (temp_next != NULL){
			temp = temp_next;
		}
		else break;
	}
}

void YKKeypressHandler(void){
	#ifdef LAB6
		GlobalFlag = 1;

	#elif defined(LAB7)
		#include "lab7defs.h"
		if(KeyBuffer == 'a') YKEventSet(charEvent, EVENT_A_KEY);
	    else if(KeyBuffer == 'b') YKEventSet(charEvent, EVENT_B_KEY);
	    else if(KeyBuffer == 'c') YKEventSet(charEvent, EVENT_C_KEY);
	    else if(KeyBuffer == 'd') YKEventSet(charEvent, EVENT_A_KEY | EVENT_B_KEY | EVENT_C_KEY);
	    else if(KeyBuffer == '1') YKEventSet(numEvent, EVENT_1_KEY);
	    else if(KeyBuffer == '2') YKEventSet(numEvent, EVENT_2_KEY);
	    else if(KeyBuffer == '3') YKEventSet(numEvent, EVENT_3_KEY);
	    else {
	        print("\nKEYPRESS (", 11);
	        printChar(KeyBuffer);
	        print(") IGNORED\n", 10);
	    }

	#else
		int k = 0;
		int m = 0;
		if (=='d'){
			printNewLine();
			printString("DELAY KEY PRESSED");
			printNewLine();
			for (k=0; k<=5000; k++){
				m++;		
			}
			printString("DELAY COMPLETE");
			printNewLine();		
		}

		///////////// LAB 5 CODE ////////////
		#ifdef LAB5
		else if(KeyBuffer=='p'){
			YKSemPost(NSemPtr);
		}
		#endif
		///////// END OF LAB 5 CODE ////////

		else {
			 printNewLine();
			 printString("KEYPRESS (");
			 printChar(KeyBuffer);
			 printString(") IGNORED");
			 printNewLine();
		}
	#endif
}

void YKResetHandler(){
	exit(0);
}

//////////////////  SEMAPHORE CODE ////////////////////////


YKSEM* YKSemCreate(int val){
	YKSEM* sem;
	if(val < 0){
		printString("Error! Invalid semaphor value!\n\r");
		return NULL;
	}
	else{
		// printString("Creating semaphor!\n\r");
	}
	sem = &YKSemaphors[YKSemCount];
	sem->value = val;
	sem->pendHead = NULL;
	sem->string = NULL;
	YKSemCount++;
	return sem;
}

void YKSemPend(YKSEM* sem){
	YKEnterMutex();
	// printString("pending...\n\r");
	if((sem->value)-- > 0){
		YKExitMutex();
		return;
	}
	else{
		YKRemoveSorted(YKRunningTask, &readyHead);
		YKInsertSorted(YKRunningTask, &(sem->pendHead));
		YKScheduler(1);
		YKExitMutex();
		return;
	}
}

void YKSemPost(YKSEM* sem){
	struct Task* pending_task;
	YKEnterMutex();
	// printString("posting....\n\r");
	if((sem->value)++ >= 0){
		YKExitMutex();
		return;
	}
	else{
		pending_task = (sem->pendHead);
		YKRemoveSorted(pending_task, &(sem->pendHead));
		YKInsertSorted(pending_task, &readyHead);
		YKScheduler(1);
		return;
	}
}

///////////////////// QUEUE CODE /////////////////////////


YKQ *YKQCreate(void **start, unsigned int size){
	YKQ *tempQ;
	YKEnterMutex();
	tempQ = &YKQueues[YKQCount];
	tempQ->size = size;
	tempQ->count = 0;
	tempQ->start = (int*)start;
	tempQ->end = ((int*)start) + (size-1);
	tempQ->head = (int*)start;
	tempQ->next = (int*)start;
	tempQ->blockedHead = NULL;
	YKQCount++;
	if(YKRunFlag==1){
		YKExitMutex();
	}
	#ifdef DEUBG
		printYKQ(tempQ);
	#endif
	return tempQ;
}

void* YKQPend(YKQ *queue){
	void* tmpMsg;
	struct Task* tmpTask;
	YKEnterMutex();
	if (queue->count == 0){
		tmpTask = readyHead;
		YKRemoveSorted(tmpTask, &readyHead);
		YKInsertSorted(tmpTask, &(queue->blockedHead));
		YKScheduler(1);
	}
	(queue->count)--;
	tmpMsg = (void*) *(queue->head);
	if (++(queue->head) > queue->end){
		queue->head = queue->start;
	}
	#ifdef DEBUG
		printString("Pend!\n");
		printMsgQueue(queue);
	#endif
	YKExitMutex();
	return tmpMsg;
}

int YKQPost(YKQ *queue, void *msg){
	struct Task* tmpTask;
	YKEnterMutex();
	if(queue->size == queue->count){
		YKExitMutex();
		return 0;
	}
	(queue->count)++;
	*(queue->next) = (int) msg;
	if (++(queue->next) > queue->end){
		queue->next = queue->start;
	}

	if(queue->blockedHead != NULL){
		tmpTask = queue->blockedHead;
		YKRemoveSorted(tmpTask, &(queue->blockedHead));
		YKInsertSorted(tmpTask, &readyHead);
		if (YKIsrDepth==0){
			YKScheduler(1);
		}
	}
	#ifdef DEBUG
		printString("POST!\n");
		printMsgQueue(queue);
	#endif
	YKExitMutex();
	return 1;
}


//////////////////// LIST MANGERS ////////////////////////

void YKInsertSorted(struct Task* item, struct Task** head){
	struct Task* temp;
	struct Task* temp_prev;
	if (item != NULL) {
		//add to empty list
		if (*head == NULL){
			*head = item;
			// tail = item;
			item->next = NULL;
			item->prev = NULL;
		}
	
		else {
			temp = *head;
			temp_prev = *head;
			while(temp != NULL){
				//put in front of temp if item priority is higher
				if ((temp->taskPriority) > (item->taskPriority)){
					//put at front of list if temp is in front
					if (temp->prev == NULL){
						item->next = temp;
						*head = item;
						temp->prev = item;
						item->prev = NULL;
					}
					//put item in between temp's prev and temp
					else {
						(temp->prev)->next = item;
						item->prev = temp->prev;
						temp->prev = item;
						item->next = temp;	
					}
					return;
				}
			temp_prev = temp;
			temp = temp->next;
			}
			//item is lowest priority-- add at tail
			temp_prev->next = item;
			item->prev = temp_prev;
			// temp_prev = item;
			item->next = NULL;
		}
	}
}

void YKRemoveSorted (struct Task* item, struct Task** head){
	if (item != NULL) {
		if (item->prev != NULL) {
			(item->prev)->next = item->next;
		}
		else {
			*head = item->next;
		}
		if (item->next != NULL) {
			(item->next)->prev = item->prev;
		}
		else {
			// tail = item->prev;
		}
	}
}

void YKInsertBlocked(struct Task* item){
	if (item != NULL) {
		if (blockedTail == NULL){
			blockedHead = item;
			blockedTail = item;
			item->next = NULL;
			item->prev = NULL;
		}
		else {
			blockedTail->next = item;
			item->prev = blockedTail;
			blockedTail = item;
			item->next = NULL;
		}
	}
}

void YKRemoveBlocked(struct Task* item){
	if (item != NULL) {
		if (item->prev != NULL) {
			(item->prev)->next = item->next;
		}
		else {
			blockedHead = item->next;
		}
		if (item->next != NULL) {
			(item->next)->prev = item->prev;
		}
		else {
			blockedTail = item->prev;
		}
	}
}


/////////////////// PRINT FUNCTIONS /////////////////////

void printStack(struct Task* item){
	int k;
	int *tempSP;
	k = 0;
	if (item != NULL){
		tempSP = item->taskSP;
		printString("\n\rPrinting Stack:\n\r");
		for (k; k<12; k++){
			printWord((int)tempSP);
			printString(": [");
			printWord(*tempSP);
			printString("]\n\r");
			tempSP = tempSP+1;
		}
	}
	else {
		printString("\n\rPrintStack: item is NULL");

	}	
}

void printLists(void){
	struct Task* temp;
	printString("\n\rReady List:\n\r");
	temp = readyHead;
	while(temp != NULL){
		printString("[0x");
		printByte(temp->taskPriority);
		printString(", ");
		printWord(temp->taskDelay);
		printString(", ");
		printWord((int)temp->taskSP);
		printString("] ");
		temp = temp->next;
	}
	printNewLine();
	temp = blockedHead;
	printString("Blocked List:\n\r");
	while(temp != NULL){
		printString("[0x");
		printByte(temp->taskPriority);
		printString(", ");
		printWord(temp->taskDelay);
		printString(", ");
		printWord((int)temp->taskSP);
		printString("] ");
		temp = temp->next;
	}
	printString("\n\r");
}

void printYKQ(YKQ *queue){
	// YKEnterMutex();
	printString("Printing YKQ:\n\r");
	printString("NextEmpty: "); 
	printWord((int)queue->next);
	printString(", head: ");
	printWord((int)queue->head);
	printString(", size: ");
	printInt(queue->size);
	printString(", count: ");
	printInt(queue->count);
	printString(", qStart: "); 
	printWord((int)queue->start);
	printString(", qEnd: ");
	printWord((int)queue->end);
	printNewLine();
	// YKExitMutex();
}

void printMsgQueue(YKQ *queue){
	int *tempMsg;
	int k;
	YKEnterMutex();
	printString("PrintingQueue:\n\r");
	k = 0;
	tempMsg = (int*)queue->head;
	if (tempMsg != NULL) {
		for (k; k < (queue->count); k++){
			printString("[");
			printWord((int) tempMsg);
			printString("]: "); 
			printInt((int)*tempMsg);
			printString(": ");
			printInt(*((int*)*tempMsg));
			printString(", ");
			tempMsg++;
			if (((int*) tempMsg) > (queue->end)){
				tempMsg = queue->start;
			}
		}
	printNewLine();
	}
	YKExitMutex();
}


void printEventPendingTasks(YKEVENT *event){
	struct Task* tempTask;
	struct Task* nextTask;
	YKEnterMutex();
	printString("Printing Event-blocked Tasks:\n\r");
	tempTask = event->blockedHead;
	if(tempTask == NULL) return;
	do {
		nextTask = tempTask->next;
		printString("[0x");
		printByte(tempTask->taskPriority);
		printString(", ");
		printWord(tempTask->taskDelay);
		printString(", ");
		printWord((int)tempTask->taskSP);
		printString("] ");
		tempTask = nextTask;
		} while(nextTask!=NULL);
	printNewLine();
	YKExitMutex();
}