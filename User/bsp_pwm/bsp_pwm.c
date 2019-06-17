 /**
  ******************************************************************************
  * @file    bsp_pwm.c
  * @author  �Ϸ���
  * @version V1.0
  * @date    2019-03-02
  * @brief   ���PWM������ѹ������ 
  ******************************************************************************
  **/ 

#include "bsp_pwm.h"

/*
	* @brief   ���PWM��ʼ��  
	* @param   arr����װ������
	           psc: Ԥ��Ƶ����
	* @retval  ��
*/
void PWM_TIM_Init(uint16_t arr,uint16_t psc) 
{
  GPIO_InitTypeDef GPIO_InitStructure;              //GPIO��ʼ���ṹ��
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //ʱ���ṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;           //����ȽϽṹ��
 
	TIM_DeInit(PWM_TIM);                                   //���й�TIM�ļĴ�����ԭ��Ĭ��ֵ
	PWM_TIM_APBxClock_FUN(PWM_TIM_CLK, ENABLE);	           //ʹ�ܶ�ʱ��ʱ��
 	RCC_APB2PeriphClockCmd(PWM_TIM_GPIO_CLK, ENABLE);  //ʹ��GPIO����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //AFIO���ù���ģ��ʱ��
 
	GPIO_PinRemapConfig(PWM_TIM_GPIO_REMAP, ENABLE);       //TIMȫ����ӳ��

  /*����Ƚ�ͨ�� GPIO ��ʼ��*/
	RCC_APB2PeriphClockCmd(PWM_TIM_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  PWM_TIM_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PWM_TIM_GPIO_PORT, &GPIO_InitStructure);
	
	/*ʱ���ṹ���ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ�װ�ؼĴ�����ֵ��ÿ��arr+1������һ�����»��ж�
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����ʱ��Ԥ��Ƶֵ����ʱ��Ƶ��Ϊ72M/(psc+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //�ظ���������ֵ���˴�����
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //���ú�����ʼ��ʱ���ṹ��
	
	/*����ȽϽṹ���ʼ��*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);               //���ú�����ʼ��ͨ��1����ȽϽṹ��
  TIM_OC1PreloadConfig(PWM_TIM,TIM_OCPreload_Enable);       //ʹ��CCR1��Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(PWM_TIM,ENABLE);                     //ʹ��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(PWM_TIM,ENABLE);                                  //ʹ�ܼ�����
	TIM_CtrlPWMOutputs(PWM_TIM,ENABLE);                       //�����ʹ��
}

