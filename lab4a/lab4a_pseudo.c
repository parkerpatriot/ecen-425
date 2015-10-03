
/*
Title: LAB 4A PSEUDO CODE
Authors: JAMES PARKER & RICK LYON
Date: OCTOBER 1, 2015
*/


#define TASK_NUM 10
#define IDLE_STACKSIZE 256

unsigned int YKCtxSwCount; // Global variable tracking context switches
unsigned int YKIdleCount; // Global variable used by idle task
unsigned int YKTickNum; // Global variable incremented by tick handler

unsigned char task_count;
unsigned char YKRunFlag; // indicates if YKRun() has been called
unsigned char YKIsrDepth; // Same as YKTcxSwCount

struct Task* task_next; // determines/sets the list (blocked/ready) and priroity of the task
struct Task* task_prev; // determines/sets the list (blocked/ready) and priroity of the task

int IdleStk[IDLE_STACKSIZE];
void IdleTask(void); 

struct Task {
	unsigned char task_priority;
	int *task_sp;
	
	unsigned task_delay;
	
	struct Task* task_next;
	struct Task* task_prev;
} YKtasks[TASKNUM];

void YKInitialize(void){
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKTickNum = 0;
	task_count = 0;
	YKRunFlag = 0;
	YKIsrDepth = 0;
	
	//Create Idle Task with lowest priority
	YKNewTask(IdleTask, (void *) &IdleStk[IDLE_STACKSIZE], 100);
}

void YKNewTask(void (*task)(void), void *taskStack, unsigned char priority){
	/*
	Call function to add to sorted ready list--next and prev set
	if TCB with same priority already exists in list, print warning and call exit()
	*/
	
	Yktasks[task_count]->task_ip = task;
	Yktasks[task_count]->task_sp = taskStack;
	Yktasks[task_count]->task_priority = priority;
	Yktasks[task_count]->task_delay = 0;
	
	/*
	Call assembly routine to create initial stack frame with IP from the argument and the 
	flags and cs initialized correctly
	*/
	
	task_count++;
	
	if (YKRunFlag == 1 ){
		//save context
		YkScheduler(1);
	}
}

void YKRun(void){
	YKRunFlag = 1;
	YKScheduler(0);
}

void YKDelayTask(unsigned count){
	if (count == 0){
		return;
	}
	
	running_task->task_delay = count;
	
	/*
	remove task from ready list and put in unsorted blocked list
	*/
	
	//save context and switch tasks, pass 1 in to indicate to dispatcher to save context
	YKScheduler(1);
}

void YKTickHandler(void){
	
	YKTickNum++;
	
	/*
	decrement every task_delay in blocked list
	if any get to zero, then move that TCB to sorted ready list
	*/
}

void YKEnterMutex(void) {
	/*
	assembly routine: 
	cli
	ret
	*/
}

void YKExitMutex(void){
	/*
	assembly routine: 
	sti
	ret
	*/
}

//make sure assembly code for this function is at least 4 instructions
void YKIdleTask(void){
	while(1);
}

void YKEnterISR(void){
	YKIsrDepth++;
	if (YKIsrDepth == 1){
		/*
		call assembly routine to save value of sp to running task's TCB.
		*/
	}
}

void YKExitISR(void){
	YKIsrDepth--;
	if (YKIsrDepth == 0){
		//don't save context-- ISR already saved it
		YKScheduler(0);
	}
	
}

void YKScheduler(unsigned char save_context){
	
	if(running_task != ready_head){
		YKCtxSwCount++;
		YKDispatcher(save_context);
	}
	
}

//this function is written in assembly
void YKDispatcher(unsigned char save_context){
	/*
	if argument is 1, save context: 
	push all registers in correct order, bottom of the stack should look like: Return IP, CS, Flags, bp...
	
	Restore context:
	get sp from the TCB at the head of ready list. Pop all the registers off the corresponding stack. Change the running task pointer to
	point to TCB at the head of the ready list. Call iret.
	*/
}