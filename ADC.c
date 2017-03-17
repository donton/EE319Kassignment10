// put implementations for functions, explain how it works
// put your names here, date
// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "ST7735.h"
#include "SysTick.h"
extern uint32_t decrement;
// ADC initialization function 
// Input: none
// Output: none 
void ADC_Init(void){ 
	int delay;
	//1. ENABLE PORT E CLOCK
	SYSCTL_RCGCGPIO_R |= 0x08;
	//2. WAIT
	while ((SYSCTL_RCGCGPIO_R & 0x08) == 0){};
//	delay = SYSCTL_RCGCGPIO_R;
	//3. SET PIN 2 AS AN INPUT
	GPIO_PORTD_DIR_R &= ~0x04;
	//4. TURN ON ALTERNATOR
	GPIO_PORTD_AFSEL_R |= 0x04;
	//5. DIGITAL ENABLE
	GPIO_PORTD_DEN_R &= ~0x04;
	//6. ENABLE ANALOG FUNCTION
	GPIO_PORTD_AMSEL_R |= 0x04;
	//7. ENABLE THE ADC CLOCK
	SYSCTL_RCGCADC_R |= 0x01;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	//8. SPECIFY A MAXIMUM SAMPLING RATE OF 125 KHZ
	ADC0_PC_R = 0x01;
	//9. SET THE PRIORITY OF EACH SEQUENCER (WE ARE USING 3, THE SIMPLEST ONE)
	ADC0_SSPRI_R = 0x0123; // CAUSING HARDFAULT
	//10. DISABLE THE SEQUENCER BEFORE CONFIGURING IT
	ADC0_ACTSS_R &= ~0x0008;
	//11. SPECIFY A SOFTWARE TRIGGER
	ADC0_EMUX_R &= ~0xF000;
	//12. SAMPLE CHANNEL ONE
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0)+5;
	//13. ENSURE THAT THE END BIT IS SET (PREVENTS UNPREDICTABLE BEHAVIOR), TURN OFF TEMPERATURE BIT, 
	//TURN ON RIS FLAG SETTING (TELLS US WHEN DONE SAMPLING), TURN OFF DIFFERENTIAL MODE 
	ADC0_SSCTL3_R = 0x0006;
	//14. DISABLE SS3 INTERRUPTS
	ADC0_IM_R &= ~0x0008;
	//15. ENABLE SAMPLE SEQUENCER LOGIC (TURN BACK ON NOW THAT WE'RE DONE CONFIGURING STUFF)
	ADC0_ACTSS_R |= 0x0008;
}
char hard[7] = "hard  ";
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	uint32_t data; 
	ADC0_PSSI_R = 0x0008;
	while ((ADC0_RIS_R & 0x0008) == 0) {};
	data = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R = 0x0008;
  return data;
}
char easy[7] = "easy  ";
//------------Convert------------
// Input: number to be converted
// Output: converted number
uint32_t Convert(uint32_t input){
  uint32_t value = ((488*input)/1000)+(1480/100000);  
	return value;
}
//------------Select_Difficulty------------
// Input: none
// Output: none
char str[14] = "Select Level: ";
void Select_Difficulty(void){
	ST7735_SetTextColor(0xFF);
	ST7735_OutString(str);
	uint32_t data;
	ST7735_SetCursor(14,0);
	while (!(GPIO_PORTB_DATA_R)){
		data = ADC_In();
		data = Convert(data);
		if (data >= 1000){
				decrement = 2;
				ST7735_SetCursor(14,0);
				ST7735_OutString(hard);
			}
		if (data < 1000){
				decrement = 1;
				ST7735_SetCursor(14,0);
				ST7735_OutString(easy);
			}
		}	
}
