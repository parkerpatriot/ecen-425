// #define TICK "TICK"
// #define KEY "KEYPRESS %c IGNORED"
#include "clib.h"

// program messages (strings)
const char TICK[8] = "\n\rTICK ";
#define TICK_LEN 8
const char KEYPRESS[13] = "\n\rKEYPRESS (";
#define KEY_LEN 13
const char IGNORED[12] = ") IGNORED\n\r";
#define IG_LEN 12
const char START_DELAY[22] = "\n\rDELAY KEY PRESSED\n\r";
#define SDEL_LEN 22
const char END_DELAY[19] = "\n\rDELAY COMPLETE\n\r";
#define EDEL_LEN 19

// static counter for couting number of tick interrupts
static int tick_counter = 0;

// key buffer for logging key presses
extern int KeyBuffer;

void tickHandler(){
	tick_counter++;
	print(TICK, TICK_LEN);
	printInt(tick_counter);
	printNewLine();
}

void resetHandler(){
	exit(0);
}

void nothing_loop(){
	int counter = 0;
	while(counter<5000){
		counter++;
	}
}

void keyHandler(){
	char cast_char;
	cast_char = (char) KeyBuffer;
	if(cast_char=='d'){
		print(START_DELAY, SDEL_LEN);
		nothing_loop();
		print(END_DELAY, EDEL_LEN);
	}
	else{
		print(KEYPRESS, KEY_LEN);
		printChar(cast_char);
		print(IGNORED, IG_LEN);
	}
}