/**
******************************************************************************
* @file    			main.c
* @author  			�Ϸ���
* @version 			V1.0
* @date    			2018-11-11
* @brief   			ADת�����ڴ������
* @attention  	ʵ��ƽ̨��Ұ��stm32������
*
*               Ĭ��ʹ�ÿ������SPI2�ӿ�
*							
*               �������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ
*               ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP
******************************************************************************
*/ 
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "w5500.h"
#include "w5500_conf.h"
#include "w5500_int.h"
#include "socket.h"
#include "utility.h"
#include "tcp_demo.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_AD9854.h"
#include "bsp_dac.h"
#include "bsp_pwm.h"



int main(void)
{ 	
	systick_init(72);										/*��ʼ��Systick����ʱ��*/
	GPIO_AFIODeInit();                  /*���ù��ܻ�ԭΪĬ��ֵ*/
	USART1_Config();										/*��ʼ������ͨ��:115200@8-n-1*/
	gpio_for_w5500_config();						/*��ʼ��MCUy��W5500��ص�����*/
	/*��ʼ����AD9854��ص�GPIO�˿�*/
	AD9854_GPIO_Config();
	/*��ʼ��DAC*/
	DAC_Config();
	/*pwm��ʼ��*/
	PWM_TIM_Init(15,8); // PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
	/*����AD9854������Ҳ���Ƶ��*/
	set_AD9854(2);
	/*����ռ�ձ�*/
	PWM_DutyRatio(PWM_TIM,6); //ռ�ձ�ΪCCR/(ARR+1)
  /*����DAC���������*/
	DAC_Setdata(4095, 1023); 
	reset_w5500();											/*Ӳ��λW5500*/
	set_w5500_mac();										/*����MAC��ַ*/
	set_w5500_ip();											/*����IP��ַ*/
	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	  while(getSn_SR(SOCK_TCPS) != SOCK_ESTABLISHED)
	{
		if(getSn_SR(SOCK_TCPS) == SOCK_CLOSED)
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);
		if(getSn_SR(SOCK_TCPS) == SOCK_INIT)
			listen(SOCK_TCPS);
	}
	ADCx_Init();                  /*ADCģʽ��ʼ��*/
	W5500_INT_EXTI_Config();
	setSIMR(0x01);
	setSn_IMR(SOCK_TCPS,Sn_IR_RECV);
	
	while(1)                            /*ѭ��ִ�еĺ���*/ 
	
	{
	  
	}
} 


