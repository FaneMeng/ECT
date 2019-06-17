/**
  ******************************************************************************
  * @file    bsp_dac.c
  * @author  孟繁悦
  * @version V1.0
  * @date    2018-03-05
  * @brief   双路DAC设置
  ******************************************************************************
  */ 
  
#include "bsp_dac.h"


/**
  * @brief  使能DAC的时钟，初始化GPIO
  * @param  无
  * @retval 无
  */
void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* 使能GPIOA时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/* 使能DAC时钟 */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC的GPIO配置，模拟输入 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	


  /* 配置DAC 通道1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;						//使用软件触发
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不使用波形发生器
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	//使用DAC输出缓冲
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* 配置DAC 通道2 */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* 使能通道1 由PA4输出 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* 使能通道2 由PA5输出 */
  DAC_Cmd(DAC_Channel_2, ENABLE);
}

/*
	* @brief   设置DAC输出的数据
	* @param   DAC1Data：DAC1要输出的数据
             DAC1Data：DAC1要输出的数据
	* @retval  无
*/
void DAC_Setdata(uint16_t DAC1Data, uint16_t DAC2Data)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, DAC1Data);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_SetChannel2Data(DAC_Align_12b_R, DAC2Data);
	DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}































