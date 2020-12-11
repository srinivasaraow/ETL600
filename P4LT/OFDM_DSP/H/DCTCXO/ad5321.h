#ifndef AD5321_H
#define AD5321_H

#include "AD5321_defs.h"

void AD5321_Init();
void AD5321_Set_Reg(unsigned int Adr, unsigned int Data);
void AD5321_Write(unsigned int word24);
void Timer_Set(unsigned int);	
void Timer_Enable();
void AD5321_Delay(int);
void AD5321_Wait();
int AD5321_Convert_ppm2code(float ppm);

#endif
