// Snake.c
// Runs on LM4F120/TM4C123
// Domino Weir and Don Ton
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "draw.h"
#include "switch.h"
#include "random.h"
#include "SysTick.h"
#include "Timer0.h"
#include "Sound.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
// ************** Global Variables *******************
uint8_t loseCondition = 0;
uint32_t snakeLength = 3;
uint32_t score = 0;
//xcoor and y coor of the "head" of the snake (top left corner)
//current direction and new direction- matters when changing corners
uint32_t mov = 0x001; //one bit each for up-down-left-right (in that order)
uint32_t newdir = 0x0000;
//ateDot tells us if the snake succeeded in eating the dot
uint32_t ateDot = 0;
//this basically controls the speed 
uint32_t delay = 30;
uint32_t decrement = 1; //rate at which we decrement the above delay counter
void Init_All(void){
	TExaS_Init();  									
  Random_Init(1); 
	ADC_Init();
	Output_Init();
	Timer0_Init(75000);
	Sound_Init();  
  ST7735_InitR(INITR_REDTAB);
	Switch_Init();	
	Draw_StartScreen();
  Delay100ms(25);
	ST7735_FillScreen(0x0000);
	Select_Difficulty();
	ST7735_FillScreen(0x0000);
	Draw_Init();
	Draw_Dot();
}
int main(void){
	Init_All();
	while(1){
			Draw_ScoreScreen(score);
			Delay100ms(delay);
			Switch_In();
			Move_Snake(); 
			Switch_In();
			Dot_Eat();
		if (loseCondition == 1){
				break;
			}//endif
	}//endwhile
	//losing screens
	ST7735_FillScreen(0x0000);
	Draw_LoseScreen();
}//endmain
