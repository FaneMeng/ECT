#include "stm32f10x.h"
#include "w5500_conf.h"
#ifndef _W5500_INT_H
#define _W5500_INT_H

//引脚定义 (关于W5500_INT引脚的定义参见 w5500_conf.h,这里只定义与设置中断有关的参数)
#define WIZ_INT_AFIO_CLK     RCC_APB2Periph_AFIO
#define WIZ_INT_PORTSOURCE   GPIO_PortSourceGPIOG
#define WIZ_INT_PINSOURCE    GPIO_PinSource8
#define WIZ_INT_EXTI_LINE    EXTI_Line8
#define WIZ_INT_EXTI_IRQ     EXTI9_5_IRQn
#define W5500_IRQHandler     EXTI9_5_IRQHandler

//函数声明
void W5500_INT_EXTI_Config(void);

#endif /*_W5500_INT_H*/

