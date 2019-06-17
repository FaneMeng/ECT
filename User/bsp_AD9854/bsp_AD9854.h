#ifndef __BSP_AD9854_H
#define	__BSP_AD9854_H

#include "stm32f10x.h"

/*定义控制AD9854所要用到的GPIO端口*/
#define AD9854_GPIO_Port GPIOD
#define AD9854_GPIO_CLK  RCC_APB2Periph_GPIOD
#define AD9854_GPIO_PIN  GPIO_Pin_All
#define AD9854_GPIO_ODR_Addr (GPIOD_BASE+12)  //0x4001140C

/*定义位带操作所要用到的地址*/
//将寄存器地址转换到影子寄存器地址
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
// 把影子寄存器的地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

/*定义控制AD9854所要用到的输出位操作*/
#define AD9854_Data(n) BIT_ADDR(AD9854_GPIO_ODR_Addr,(15-n)) //AD9854上的D0~D7对接STM32的PD15~PD8
#define AD9854_Addr(n) BIT_ADDR(AD9854_GPIO_ODR_Addr,(5-n))  //AD9854上的A0~A5对接STM32的PD5~PD0
#define AD9854_WRB     BIT_ADDR(AD9854_GPIO_ODR_Addr,7)     //AD9854上的WRB对接STM32的PD7
#define AD9854_MRESET  BIT_ADDR(AD9854_GPIO_ODR_Addr,6)     //AD9854上的MRESET对接STM32的PD6

/*函数声明*/
void AD9854_GPIO_Config(void);
static void set_AD9854_DataBus(uint8_t AD9854_FCW);
static void set_AD9854_Addr(uint8_t AD9854_Addr);
static void AD9854_WR_Byte(uint8_t AD9854_Addr,uint8_t AD9854_FCW);
void set_AD9854(uint8_t NumFre);
static void set_AD9854_Addr(uint8_t AD9854_Addr);

#endif /*__BSP_AD9854_H*/
