#include "clib.h"
#include "yakk.h"
#include "yaku.h"

int* YKsave;
int* YKrestore;

extern int KeyBuffer;

int YKtaskCount;
int YKRunFlag;
int YKIsrDepth;
volatile int YKIdleVar;
unsigned int YKTickCount;
unsigned int YKCtxSwCount;
unsigned int YKIdleCount;

struct Task* readyHead;
struct Task* readyTail;
struct Task* blockedHead;
struct Task* blockedTail;
struct Task* YKRunningTask;

struct Task {
	int *taskSP;
	unsigned char taskPriority;
	unsigned int taskDelay;
	struct Task* next;
	struct Task* prev;
} YKtasks[TASKNUM];

int YKIdleStk[STACKSIZE];

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
	YKNewTask(&YKIdleTask, &YKIdleStk[IDLE_STACKSIZE], 100);
}

void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority){
	struct Task* tempTask;
	int *tempSP;
	int k;
	YKEnterMutex();
	//set up inital stack frame
	tempSP = taskStack-1;
	*tempSP = 0x0200; 	//initial value for flags (0x0200)
	--tempSP;
	*tempSP = 0; 		//initial value for CS
	--tempSP;
	*tempSP = (int)task; 	//initial value for IP
	--tempSP;
	*tempSP = (int)(taskStack-1);	//initial value for BP
	--tempSP;
	for(k = 0; k <=7; k++){
		*tempSP = 0;
		--tempSP;
	}
	
	//initialize TCB
	tempTask = &YKtasks[YKtaskCount];
	tempTask->taskSP = tempSP+1;
	tempTask->taskPriority = priority;
	tempTask->taskDelay = 0;
	
	YKtaskCount++;
	//insert into ready list 
	YKinsertReady(tempTask);
	printStack(tempTask);
	printLists();
	if (YKRunFlag == 1){
		YKScheduler(1);
		YKExitMutex();
	}
}

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

void YKDelayTask(unsigned count){
	struct Task* item;
	if (count != 0){
		YKEnterMutex();
		item = YKRunningTask;
		item->taskDelay = count;
		YKremoveReady(item);
		YKinsertBlocked(item);
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
		YKsave = &(YKRunningTask->taskSP);
		YKsaveSP();
	}
}

void YKScheduler(int saveContext){
	if (YKRunningTask != readyHead){
		YKsave = &(YKRunningTask->taskSP);
		YKrestore = readyHead->taskSP;
		YKRunningTask = readyHead;
		YKCtxSwCount++;
		YKDispatcher(saveContext);
	}
}

//assuming tick isr is highest priority
void YKTickHandler(void){
	struct Task* temp;
	struct Task* temp_next;
	printString("TICK ");
	printInt(YKTickCount);
	printNewLine();
	YKTickCount++;
	temp = blockedHead;
	//printLists();
	while(temp != NULL){
		temp_next = temp->next;
		(temp->taskDelay)--;
		if (temp->taskDelay <= 0){
			YKremoveBlocked(temp);
			YKinsertReady(temp);
		}
		if (temp_next != NULL){
			temp = temp_next;
		}
		else break;
	}
}

void YKkeypress(void){
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

	else {
		 printNewLine();
		 printString("KEYPRESS (");
		 printChar(KeyBuffer);
		 printString(") IGNORED");
		 printNewLine();
	}
}

void YKinsertReady(struct Task* item){
	struct Task* temp;
	if (item != NULL) {
		//add to empty list
		if (readyHead == NULL){
			readyHead = item;
			readyTail = item;
			item->next = NULL;
			item->prev = NULL;
		}
	
		else {
			temp = readyHead;
			while(temp != NULL){
				//put in front of temp if item priority is higher
				if ((temp->taskPriority) > (item->taskPriority)){
					//put at front of list if temp is in front
					if (temp->prev == NULL){
						item->next = temp;
						readyHead = item;
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
			temp = temp->next;
			}
			//item is lowest priority-- add at tail
			readyTail->next = item;
			item->prev = readyTail;
			readyTail = item;
			item->next = NULL;
		}
	}
}

void YKremoveReady (struct Task* item){
	if (item != NULL) {
		if (item->prev != NULL) {
			(item->prev)->next = item->next;
		}
		else {
			readyHead = item->next;
		}
		if (item->next != NULL) {
			(item->next)->prev = item->prev;
		}
		else {
			readyTail = item->prev;
		}
	}
}

void YKinsertBlocked(struct Task* item){
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

void YKremoveBlocked(struct Task* item){
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
