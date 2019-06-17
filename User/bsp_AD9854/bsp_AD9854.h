#ifndef __BSP_AD9854_H
#define	__BSP_AD9854_H

#include "stm32f10x.h"

/*�������AD9854��Ҫ�õ���GPIO�˿�*/
#define AD9854_GPIO_Port GPIOD
#define AD9854_GPIO_CLK  RCC_APB2Periph_GPIOD
#define AD9854_GPIO_PIN  GPIO_Pin_All
#define AD9854_GPIO_ODR_Addr (GPIOD_BASE+12)  //0x4001140C

/*����λ��������Ҫ�õ��ĵ�ַ*/
//���Ĵ�����ַת����Ӱ�ӼĴ�����ַ
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
// ��һ����ַת����һ��ָ��
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
// ��Ӱ�ӼĴ����ĵ�ַת����ָ��
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

/*�������AD9854��Ҫ�õ������λ����*/
#define AD9854_Data(n) BIT_ADDR(AD9854_GPIO_ODR_Addr,(15-n)) //AD9854�ϵ�D0~D7�Խ�STM32��PD15~PD8
#define AD9854_Addr(n) BIT_ADDR(AD9854_GPIO_ODR_Addr,(5-n))  //AD9854�ϵ�A0~A5�Խ�STM32��PD5~PD0
#define AD9854_WRB     BIT_ADDR(AD9854_GPIO_ODR_Addr,7)     //AD9854�ϵ�WRB�Խ�STM32��PD7
#define AD9854_MRESET  BIT_ADDR(AD9854_GPIO_ODR_Addr,6)     //AD9854�ϵ�MRESET�Խ�STM32��PD6

/*��������*/
void AD9854_GPIO_Config(void);
static void set_AD9854_DataBus(uint8_t AD9854_FCW);
static void set_AD9854_Addr(uint8_t AD9854_Addr);
static void AD9854_WR_Byte(uint8_t AD9854_Addr,uint8_t AD9854_FCW);
void set_AD9854(uint8_t NumFre);
static void set_AD9854_Addr(uint8_t AD9854_Addr);

#endif /*__BSP_AD9854_H*/
