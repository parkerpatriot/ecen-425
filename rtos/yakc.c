#include "clib.h"
#include "yakk.h"
#include "yaku.h"

#define INTERRUPTS_ENABLED	0x0200

int* YKsave;
int* YKrestore;

extern int KeyBuffer; 	// clib key buffer
extern YKSEM *NSemPtr; 	// for Lab 5 application

int YKtaskCount; 	// how many tasks initialized
int YKRunFlag;		// are we running?
int YKIsrDepth;		// ISR nesting counter

volatile int YKIdleVar; 
unsigned int YKTickCount;
unsigned int YKCtxSwCount;
unsigned int YKIdleCount;

struct Task* readyHead;
struct Task* readyTail;
struct Task* blockedHead;
struct Task* blockedTail;
struct Task* YKRunningTask;

struct Task* pendHead;
struct Task* pendTail;

struct Task YKTasks[TASKNUM];

int YKIdleStk[STACKSIZE];

// semaphor stuff
YKSEM YKSemaphors[MAXSEM];

volatile unsigned int semCount;

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
	semCount = 0;
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
	printStack(tempTask);
	printLists();
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
		//printLists();
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
	printString("TICK ");
	printInt(YKTickCount);
	printString("\n\r\n\r");
	YKTickCount++;
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
	int k = 0;
	int m = 0;
	if (KeyBuffer=='d'){
		printNewLine();
		printString("DELAY KEY PRESSED");
		printNewLine();
		for (k=0; k<=5000; k++){
			m++;		
		}
		printString("DELAY COMPLETE");
		printNewLine();		
	}	
	else if(KeyBuffer=='p'){
		YKSemPost(NSemPtr);
	}

	else {
		 printNewLine();
		 printString("KEYPRESS (");
		 printChar(KeyBuffer);
		 printString(") IGNORED");
		 printNewLine();
	}
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
	sem = &YKSemaphors[semCount];
	sem->value = val;
	sem->pendHead = NULL;
	sem->string = NULL;
	semCount++;
	return sem;
}

void YKSemPend(YKSEM* sem){
	YKEnterMutex();
	// printString("pedingin...\n\r");
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
