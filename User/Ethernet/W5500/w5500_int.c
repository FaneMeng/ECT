#include "w5500_int.h"

/**
  * @brief  W5500_INT引脚的中断控制器配置
  * @param  无
  * @retval 无
  */
static void WIZ_INT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*配置NVIC为优先级组1*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*配置中断源为W5500_INT引脚*/
	NVIC_InitStructure.NVIC_IRQChannel = WIZ_INT_EXTI_IRQ;
	/*配置抢占优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/*配置子优先级为1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/*使能中断通道*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*调用初始化函数*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  W5500_INT引脚的中断配置
  * @param  无
  * @retval 无
  */
void W5500_INT_EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*开启AFIO的时钟*/
	RCC_APB2PeriphClockCmd(WIZ_INT_AFIO_CLK,ENABLE);
	/*配置NVIC中断*/
	WIZ_INT_NVIC_Config();
	/*选择EXTI的信号源*/
	GPIO_EXTILineConfig(WIZ_INT_PORTSOURCE,WIZ_INT_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = WIZ_INT_EXTI_LINE;
	/*配置EXTI为中断模式*/
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/*下降沿中断*/
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	/*使能中断*/
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	/*初始化*/
	EXTI_Init(&EXTI_InitStructure);
}

