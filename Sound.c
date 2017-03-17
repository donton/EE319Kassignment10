// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>
#include "Sound.h"
#include "DAC.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
uint32_t delay1;
void DAC_Init(void){
//PortE init- Port E pins 4-0 are DAC outputs
		SYSCTL_RCGC2_R |= 0x10;  								// 1) enable clock to E
		delay1 = SYSCTL_RCGC2_R;  								// 2) no need to unlock
		GPIO_PORTE_LOCK_R |= 0x4C4F434B;
		GPIO_PORTE_CR_R 	|= 0xF8;							// allow changes to PE4-0
		GPIO_PORTE_DIR_R	|= 0xF8;							// PE4-0 are outputs
		GPIO_PORTE_DEN_R	|= 0xF8;							// digital enable
}

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: none
// Output: none
void Sound_Init(void){
	DAC_Init();
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
	NVIC_ST_CTRL_R = 0x07;
}
// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTE_DATA_R = (data<<3);	
}

const unsigned short preciseWave[64] = {
	  32,35,38,41,44,47,49,52,54,56,58,
	  59,61,62,62,63,63,63,62,62,61,59,
		58,56,54,52,49,47,44,41,38,35,
	  32,29,26,23,20,17,15,12,10,8,
	  6,5,3,2,2,1,1,1,2,2,3,
		5,6,8,10,12,15,17,20,23,26,29};
int Index = 0;

void SysTick_Handler(void){
		Index = (Index+1)&0x3F;      // 4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3,... 
		DAC_Out(preciseWave[Index]);    // output one value each interrupt
}

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
// Output: none
void Sound_Play(uint32_t freq){
	NVIC_ST_RELOAD_R = freq;
}


