#ifndef __BSP_PWM_H
#define	__BSP_PWM_H

#include "stm32f10x.h"

//��ʱ����������
#define            PWM_TIM                   TIM1
#define            PWM_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            PWM_TIM_CLK               RCC_APB2Periph_TIM1

// TIM1 ����Ƚ�ͨ��CH1��ӳ��
#define            PWM_TIM_GPIO_CLK           RCC_APB2Periph_GPIOE
#define            PWM_TIM_GPIO_PORT          GPIOE
#define            PWM_TIM_GPIO_PIN           GPIO_Pin_9
#define            PWM_TIM_GPIO_REMAP         GPIO_FullRemap_TIM1

//ռ�ձ�����
#define            PWM_DutyRatio              TIM_SetCompare1

//��������
void PWM_TIM_Init(uint16_t arr,uint16_t psc);

#endif  /*__BSP_PWM_H*/
