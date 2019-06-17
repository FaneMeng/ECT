#include "stm32f10x.h"
#include "w5500_conf.h"
#ifndef _W5500_INT_H
#define _W5500_INT_H

//���Ŷ��� (����W5500_INT���ŵĶ���μ� w5500_conf.h,����ֻ�����������ж��йصĲ���)
#define WIZ_INT_AFIO_CLK     RCC_APB2Periph_AFIO
#define WIZ_INT_PORTSOURCE   GPIO_PortSourceGPIOG
#define WIZ_INT_PINSOURCE    GPIO_PinSource8
#define WIZ_INT_EXTI_LINE    EXTI_Line8
#define WIZ_INT_EXTI_IRQ     EXTI9_5_IRQn
#define W5500_IRQHandler     EXTI9_5_IRQHandler

//��������
void W5500_INT_EXTI_Config(void);

#endif /*_W5500_INT_H*/

