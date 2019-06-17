/**
  ******************************************************************************
  * @file    bsp_dac.c
  * @author  �Ϸ���
  * @version V1.0
  * @date    2018-03-05
  * @brief   ˫·DAC����
  ******************************************************************************
  */ 
  
#include "bsp_dac.h"


/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	


  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;						//ʹ���������
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	//ʹ��DAC�������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* ����DAC ͨ��2 */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* ʹ��ͨ��1 ��PA4��� */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* ʹ��ͨ��2 ��PA5��� */
  DAC_Cmd(DAC_Channel_2, ENABLE);
}

/*
	* @brief   ����DAC���������
	* @param   DAC1Data��DAC1Ҫ���������
             DAC1Data��DAC1Ҫ���������
	* @retval  ��
*/
void DAC_Setdata(uint16_t DAC1Data, uint16_t DAC2Data)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, DAC1Data);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_SetChannel2Data(DAC_Align_12b_R, DAC2Data);
	DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}































