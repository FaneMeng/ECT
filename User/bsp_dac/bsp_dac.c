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
#include "bsp_adc.h"

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
extern __IO uint8_t equilflag;
__IO uint16_t Max_A_X = max_value;  //水平路设置最大值
__IO uint16_t Min_A_X = min_value;    //水平路设置最小值
__IO uint16_t Max_B_Y = max_value;  //垂直路设置最大值
__IO uint16_t Min_B_Y = min_value;    //垂直路设置最小值

uint16_t A_set=2000;
uint16_t B_set=2000;


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


/*
	* @brief   平衡控制程序
	* @param   X：水平相位信号
             Y：垂直相位信号
	* @retval  无
*/
/*
void balance(unsigned short X, unsigned short Y)  //枚举
{
	uint16_t X_Value = X & 0xfff;
	uint16_t Y_Value = Y & 0xfff;
	uint16_t mid_A_X = (Max_A_X + Min_A_X) >>1;

	if (X_Value>0x7c1 && X_Value<0x83d && Y_Value>0x7c1 && Y_Value<0x83d)
	{

		 Max_A_X = max_value;
		 Min_A_X = min_value;
		 Max_B_Y = max_value;
		 Min_B_Y = min_value;
		//
		equilflag = 0;
		//
		return;
	}
	
	if(Max_A_X == Min_A_X)
	{
		Max_A_X = max_value;
		Min_A_X = min_value;
		
		if(Min_B_Y < max_value)
		{
			Min_B_Y++;
			DAC_Setdata(mid_A_X,Min_B_Y);
		}
		else
		{
			equilflag = 0;
			Min_B_Y=min_value;
		}
		return;
	}

	if (X_Value < 0x83d && Y_Value < 0x83d) Min_A_X = mid_A_X;
	else if (X_Value > 0x7c1 && Y_Value > 0x7c1) Max_A_X = mid_A_X;   //有出不去的风险
	else if ((X_Value<0x7c1 && Y_Value>0x83d) || (Y_Value<0x7c1 && X_Value>0x83d))
	{
		Max_A_X = max_value;
		Min_A_X = min_value;
		//Min_B_Y < Max_B_Y ? (Min_B_Y++) : (equilflag = 0);
		if(Min_B_Y < max_value)
		{
			Min_B_Y++;
			DAC_Setdata(mid_A_X,Min_B_Y);
		}
		else
		{
			equilflag = 0;
			Min_B_Y=min_value;
		}

		return;
	}

    DAC_Setdata(mid_A_X,Min_B_Y);
	//
	//将参数修改为mid_A_X、mid_B_Y
	//
}
*/
extern void(*p_Balance_FOR_XY)(unsigned short,unsigned short);
void balance_A_X(unsigned short X, unsigned short Y)
{
	uint16_t X_Value = X & 0xfff;
	uint16_t Y_Value = Y & 0xfff;
	if(X_Value<0x7eb)
	{
		A_set++;
		DAC_Setdata(A_set,B_set);
		p_Balance_FOR_XY=balance_B_Y;
		return;
	}
		if(X_Value>0x813)
	{
		A_set--;
		DAC_Setdata(A_set,B_set);
		p_Balance_FOR_XY=balance_B_Y;
		return;
	}
	if(X_Value>0x7eb && X_Value<0x813 && Y_Value>0x7eb && Y_Value<0x813)
	{
		equilflag = 0;
	}
}

void balance_B_Y(unsigned short X, unsigned short Y)
{
	uint16_t X_Value = X & 0xfff;
	uint16_t Y_Value = Y & 0xfff;
	if(Y_Value<0x7eb)
	{
		B_set++;
		DAC_Setdata(A_set,B_set);
		p_Balance_FOR_XY=balance_A_X;
		return;
	}
		if(Y_Value>0x813)
	{
		B_set--;
		DAC_Setdata(A_set,B_set);
		p_Balance_FOR_XY=balance_A_X;
		return;
	}
		if(X_Value>0x7eb && X_Value<0x813 && Y_Value>0x7eb && Y_Value<0x813)
	{
		equilflag = 0;
	}
}


























