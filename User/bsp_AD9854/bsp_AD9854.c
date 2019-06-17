 /**
  ******************************************************************************
  * @file    bsp_AD9854.c
  * @author  �Ϸ���
  * @version V1.0
  * @date    2019-02-28
  * @brief   ����AD9854����Ƶ�ʿɵ������Ҳ�
  ******************************************************************************
  **/ 

#include "bsp_AD9854.h"

uint8_t Adress[6]={0x04,0x05,0x06,0x07,0x08,0x09};//AD9854Ƶ�ʿ����ֵ�ַ
//AD9854Ƶ�ʿ�����
uint8_t FCW[15][6]={{0x00,0x00,0x14,0x21,0xF5,0xF4},//60
				 {0x00,0x00,0xA7,0xC5,0xAC,0x47},//500
				 {0x00,0x01,0x4F,0x8B,0x58,0x8E},//1k
				 {0x00,0x03,0xEE,0xA2,0x09,0xAB},//3k
				 {0x00,0x07,0xDD,0x44,0x13,0x55},//6k
				 {0x00,0x0D,0x1B,0x71,0x75,0x8E},//10k
				 {0x00,0x13,0xA9,0x2A,0x30,0x55},//15k
				 {0x00,0x1A,0x36,0xE2,0xEB,0x1C},//20k
				 {0x00,0x27,0x52,0x54,0x60,0xAA},//30k
				 {0x00,0x4E,0xA4,0xA8,0xC1,0x55},//60k
				 {0x00,0x83,0x12,0x6E,0x97,0x8D},//100k
				 {0x01,0x89,0x37,0x4B,0xC6,0xA8},//300k
				 {0x02,0x8F,0x5C,0x28,0xF5,0xC3},//500k
				 {0x04,0x18,0x93,0x74,0xBC,0x6A},//800k
				 {0x05,0x1E,0xB8,0x51,0x1B,0x85}};//1M

				 
/*
* @brief   ��ʼ������AD9854��GPIO��
* @param   ��
* @retval  ��
*/
void AD9854_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;  //����һ��GPIO��ʼ���ṹ��
	RCC_APB2PeriphClockCmd(AD9854_GPIO_CLK,ENABLE); //ʹ�����GPIO�˿ڵ�����ʱ��
	GPIO_Initstructure.GPIO_Pin=AD9854_GPIO_PIN;    //ѡ��Ҫ���Ƶ�����
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;  //����Ϊ�������ģʽ
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_2MHz; //������������Ϊ2MHz
	GPIO_Init(AD9854_GPIO_Port,&GPIO_Initstructure); //��ʼ��GPIO�˿�
	GPIO_ResetBits(AD9854_GPIO_Port,AD9854_GPIO_PIN); //����������0 
}	
				 
/*
* @brief   ������AD9854���ݿ�д�������
* @param   AD9854Ƶ�ʿ�����
* @retval  ��
*/
static void set_AD9854_DataBus(uint8_t AD9854_FCW)
{
	uint8_t i,j=0;
	for(i=0;i<8;i++)
	{
		j=(AD9854_FCW&(1<<i))>>i; //ȡ��AD9854Ƶ�ʿ����ֵ�ÿһλ
		if (j)
			AD9854_Data(i)=1;
		else
			AD9854_Data(i)=0;
	}
}

/*
* @brief   ������AD9854��ַ��
* @param   AD9854Ƶ�ʿ����ֵ�ַ
* @retval  ��
*/
static void set_AD9854_Addr(uint8_t AD9854_Addr)
{
	uint8_t i,j=0;
	for(i=0;i<6;i++)
	{
		j=(AD9854_Addr&(1<<i))>>i; //ȡ��AD9854Ƶ�ʿ����ֵ�ַ��ÿһλ
		if (j)
			AD9854_Addr(i)=1;
		else
			AD9854_Addr(i)=0;
	}
}

/*
	* @brief  ��AD9854д�벢������ 
	* @param  AD9854��ַ��AD9854������ 
	* @retval �� 
*/
static void AD9854_WR_Byte(uint8_t AD9854_Addr,uint8_t AD9854_FCW)
{
	set_AD9854_Addr(AD9854_Addr);
	set_AD9854_DataBus(AD9854_FCW);
	AD9854_WRB=0;
    AD9854_WRB=1;
}

/*
	* @brief   ����AD9854
	* @param   ����Ҫ���õ�Ƶ�ʵ����
	* @retval  ��
*/
void set_AD9854(uint8_t NumFre)
{
	uint8_t i=0;
	AD9854_MRESET=SET;
	AD9854_MRESET=RESET;  //����AD9854
	AD9854_WR_Byte(0x1d,0x10); //ʹ�ܵ���ģʽ���ر��ڲ��Ƚ���
	AD9854_WR_Byte(0x20,0x40); //ʹ���ڲ��˲���
	for(i=0;i<6;i++)
	 AD9854_WR_Byte(Adress[i],FCW[NumFre][i]);
}
