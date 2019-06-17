#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0};

/**
  * @brief  ����ADC�ɼ�Ƶ�ʵ�TIM��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//������ʱ��ʱ�ӣ����ڲ�ʱ��72M
	ADC_TIM_APBxClock_FUN(ADC_TIM_CLK,ENABLE);
	//ʱ�ӷ�Ƶ��������ʱ��û�У����ù�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//����ģʽΪ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //ʱ��Ԥ��Ƶ��Ϊ71�����������Ϊ1M
	TIM_TimeBaseStructure.TIM_Prescaler = ADC_TIM_Prescaler;
	//1ms��װ��һ��
  TIM_TimeBaseStructure.TIM_Period = ADC_TIM_Period;
  //������ʱ��û�У����ù�
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
  //��ʼ����ʱ��
  TIM_TimeBaseInit(ADC_TIM,&TIM_TimeBaseStructure);	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//������ϸ˵�� 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable; 
  TIM_OCInitStructure.TIM_Pulse = 1; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�����PWM1ҪΪLow��PWM2��ΪHigh 
  TIM_OC2Init(TIM2, & TIM_OCInitStructure);   
  TIM_InternalClockConfig(TIM2);
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 
  TIM_UpdateDisableConfig(TIM2, DISABLE);
	TIM_Cmd(ADC_TIM,ENABLE);

}

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN1|
																		ADC_PIN2;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	DMA_ITConfig  ( ADC_DMA_CHANNEL, DMA_IT_TC,  ENABLE) ;

	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ������
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// ��ʼ��ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_1Cycles5);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x));
	ADC_ExternalTrigConvCmd  (ADC_x,ENABLE); 

	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	//ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}
static void DMA_NVIC_Config(void)
{
	//NVIC��ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//����NVICΪ������1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//�����ж�ԴΪDMAͨ��1
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	//ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//��ɳ�ʼ��
	NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADC_TIM_Config();
	DMA_NVIC_Config();
	ADCx_Mode_Config();
}
/*********************************************END OF FILE**********************/
