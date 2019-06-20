#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"


void DAC_Config(void);
void DAC_Setdata(uint16_t DAC1Data, uint16_t DAC2DATA);
void balance(uint16_t X, uint16_t Y);
void balance_A_X(unsigned short, unsigned short);  //声明函数名
void balance_B_Y(unsigned short, unsigned short);  //声明函数名
//void(*p_Balance_FOR_XY)(unsigned short,unsigned short);

#endif /* __DAC_H */

