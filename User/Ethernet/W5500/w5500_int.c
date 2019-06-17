#include "w5500_int.h"

/**
  * @brief  W5500_INT���ŵ��жϿ���������
  * @param  ��
  * @retval ��
  */
static void WIZ_INT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*����NVICΪ���ȼ���1*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*�����ж�ԴΪW5500_INT����*/
	NVIC_InitStructure.NVIC_IRQChannel = WIZ_INT_EXTI_IRQ;
	/*������ռ���ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/*���������ȼ�Ϊ1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/*ʹ���ж�ͨ��*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*���ó�ʼ������*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  W5500_INT���ŵ��ж�����
  * @param  ��
  * @retval ��
  */
void W5500_INT_EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	/*����AFIO��ʱ��*/
	RCC_APB2PeriphClockCmd(WIZ_INT_AFIO_CLK,ENABLE);
	/*����NVIC�ж�*/
	WIZ_INT_NVIC_Config();
	/*ѡ��EXTI���ź�Դ*/
	GPIO_EXTILineConfig(WIZ_INT_PORTSOURCE,WIZ_INT_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = WIZ_INT_EXTI_LINE;
	/*����EXTIΪ�ж�ģʽ*/
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/*�½����ж�*/
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	/*ʹ���ж�*/
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	/*��ʼ��*/
	EXTI_Init(&EXTI_InitStructure);
}

