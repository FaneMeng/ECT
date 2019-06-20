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
#include "bsp_adc.h"

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
extern __IO uint8_t equilflag;
__IO uint16_t Max_A_X = max_value;  //ˮƽ·�������ֵ
__IO uint16_t Min_A_X = min_value;    //ˮƽ·������Сֵ
__IO uint16_t Max_B_Y = max_value;  //��ֱ·�������ֵ
__IO uint16_t Min_B_Y = min_value;    //��ֱ·������Сֵ

uint16_t A_set=2000;
uint16_t B_set=2000;


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


/*
	* @brief   ƽ����Ƴ���
	* @param   X��ˮƽ��λ�ź�
             Y����ֱ��λ�ź�
	* @retval  ��
*/
/*
void balance(unsigned short X, unsigned short Y)  //ö��
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
	else if (X_Value > 0x7c1 && Y_Value > 0x7c1) Max_A_X = mid_A_X;   //�г���ȥ�ķ���
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
	//�������޸�Ϊmid_A_X��mid_B_Y
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


























