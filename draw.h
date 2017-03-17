//draw.h
//Created by Don Ton and Domino Weir
//Function prototypes for draw on LCD subroutines
//Includes routines for both the snake and dot sprites
#include <stdint.h>
void Draw_StartScreen(void);
void Move_Right(void);
void Move_Down(void);
void Move_Left(void);
void Move_Up(void);
void Draw_Dot(void);
void Clear_Dot(void);
void Turn_Corner(void);
void Move_Snake(void);
void Dot_Eat(void);
void Draw_LoseScreen(void);
void Draw_ScoreScreen(uint32_t);
void Draw_Init(void);
void ADelay1ms(uint32_t);
