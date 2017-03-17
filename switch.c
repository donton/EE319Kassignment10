// Piano.connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "switch.h"
#include "Sound.h"
volatile uint32_t del;
extern uint32_t mov;
extern uint32_t newdir;
volatile uint32_t RisingEdges = 0;

// **************Switch_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: non
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware ce
void Switch_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x02;  								// 1) enable clock to B
		while((SYSCTL_RCGCGPIO_R&0x02)==0x00){} 
		RisingEdges = 0;  											// 2) no need to unlock
		GPIO_PORTB_LOCK_R |= 0x4C4F434B;
		GPIO_PORTB_CR_R 	|= 0x3C;							// allow changes to PB5,PB4-PB2
		GPIO_PORTB_DIR_R	&= 0xC3;							// PB5-PB2 are intputs
		GPIO_PORTB_DEN_R	|= 0x3C;							// digital enable	
}//endinit

// **************Switch_In*********************
// Input from button inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
void Switch_In(void){
	Sound_Play(500);
	uint32_t inp;
	inp = GPIO_PORTB_DATA_R; 
	Sound_Play(0);
	//because the given input is in PB5-2
	inp = inp >> 2;														
	if (inp == 0x01){
		inp = 0x0001;
	}
	else if (inp == 0x02){
		inp = 0x0010;
	}
	else if (inp == 0x04){
		inp = 0x0100;
	}
	else if (inp == 0x08){
		inp = 0x1000;
	}
	else {
		inp = mov;
	}
	newdir = inp;
}//endhalnder
