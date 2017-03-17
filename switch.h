// Piano.h
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 10/5/2014 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections

// Header files contain the prototypes for public functions
// this file explains what the module does
#include <stdint.h>
// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Switch_Init(void);

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
void Switch_In(void);
