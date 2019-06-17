/**
******************************************************************************
* @file    			main.c
* @author  			孟繁悦
* @version 			V1.0
* @date    			2018-11-11
* @brief   			AD转换网口传输测试
* @attention  	实验平台：野火stm32开发板
*
*               默认使用开发板的SPI2接口
*							
*               内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
*               如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
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
	systick_init(72);										/*初始化Systick工作时钟*/
	GPIO_AFIODeInit();                  /*复用功能还原为默认值*/
	USART1_Config();										/*初始化串口通信:115200@8-n-1*/
	gpio_for_w5500_config();						/*初始化MCUy与W5500相关的引脚*/
	/*初始化与AD9854相关的GPIO端口*/
	AD9854_GPIO_Config();
	/*初始化DAC*/
	DAC_Config();
	/*pwm初始化*/
	PWM_TIM_Init(15,8); // PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
	/*设置AD9854输出正弦波的频率*/
	set_AD9854(2);
	/*设置占空比*/
	PWM_DutyRatio(PWM_TIM,6); //占空比为CCR/(ARR+1)
  /*设置DAC输出的数据*/
	DAC_Setdata(4095, 1023); 
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	  while(getSn_SR(SOCK_TCPS) != SOCK_ESTABLISHED)
	{
		if(getSn_SR(SOCK_TCPS) == SOCK_CLOSED)
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);
		if(getSn_SR(SOCK_TCPS) == SOCK_INIT)
			listen(SOCK_TCPS);
	}
	ADCx_Init();                  /*ADC模式初始化*/
	W5500_INT_EXTI_Config();
	setSIMR(0x01);
	setSn_IMR(SOCK_TCPS,Sn_IR_RECV);
	
	while(1)                            /*循环执行的函数*/ 
	
	{
	  
	}
} 


