 /**
  ******************************************************************************
  * @file    bsp_pwm.c
  * @author  孟繁悦
  * @version V1.0
  * @date    2019-03-02
  * @brief   输出PWM波用于压控增益 
  ******************************************************************************
  **/ 

#include "bsp_pwm.h"

/*
	* @brief   输出PWM初始化  
	* @param   arr：重装载因子
	           psc: 预分频因子
	* @retval  无
*/
void PWM_TIM_Init(uint16_t arr,uint16_t psc) 
{
  GPIO_InitTypeDef GPIO_InitStructure;              //GPIO初始化结构体
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //时基结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;           //输出比较结构体
 
	TIM_DeInit(PWM_TIM);                                   //将有关TIM的寄存器还原到默认值
	PWM_TIM_APBxClock_FUN(PWM_TIM_CLK, ENABLE);	           //使能定时器时钟
 	RCC_APB2PeriphClockCmd(PWM_TIM_GPIO_CLK, ENABLE);  //使能GPIO外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //AFIO复用功能模块时钟
 
	GPIO_PinRemapConfig(PWM_TIM_GPIO_REMAP, ENABLE);       //TIM全部重映射

  /*输出比较通道 GPIO 初始化*/
	RCC_APB2PeriphClockCmd(PWM_TIM_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  PWM_TIM_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PWM_TIM_GPIO_PORT, &GPIO_InitStructure);
	
	/*时基结构体初始化*/
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动装载寄存器的值，每（arr+1）产生一个更新或中断
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置时钟预分频值，定时器频率为72M/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //重复计数器的值，此处不用
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //调用函数初始化时基结构体
	
	/*输出比较结构体初始化*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);               //调用函数初始化通道1输出比较结构体
  TIM_OC1PreloadConfig(PWM_TIM,TIM_OCPreload_Enable);       //使能CCR1上预装载寄存器
	
	TIM_ARRPreloadConfig(PWM_TIM,ENABLE);                     //使能ARR上的预装载寄存器
	TIM_Cmd(PWM_TIM,ENABLE);                                  //使能计数器
	TIM_CtrlPWMOutputs(PWM_TIM,ENABLE);                       //主输出使能
}

