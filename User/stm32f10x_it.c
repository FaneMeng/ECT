/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_adc.h"
#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "w5500_int.h"
#include "bsp_AD9854.h"
#include "bsp_pwm.h"
#include "bsp_dac.h"

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
__IO uint8_t equilflag = 0;
uint8_t buffer[4]; 
uint16_t len,m;        //控制子长度
uint16_t i,j,k = 0;
uint8_t CtrlWord[2]={3,3}; //接收上位机传来的控制字
uint16_t DAC1Val,DAC2Val = 2048;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
			 	
}

/**
  * @brief  This function handles DMA1_CH1 interrupt request.
  * @param  None
  * @retval None
  */
void(*p_Balance_FOR_XY)(unsigned short,unsigned short)=balance_A_X;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)  //确认产生了中断
	{
	
		buffer[1] = (uint8_t)((ADC_ConvertedValue[0])&(0x00ff));
		buffer[0] = (uint8_t)((((ADC_ConvertedValue[0])&(0xff00))>>8)+0x10);
		buffer[3] = (uint8_t)((ADC_ConvertedValue[1])&(0x00ff));
		buffer[2] = (uint8_t)((((ADC_ConvertedValue[1])&(0xff00))>>8)+0x20);				
		send(SOCK_TCPS,buffer,4);
		
		if(equilflag == 1)
		{
			//balance(ADC_ConvertedValue[0],ADC_ConvertedValue[1]);
			//void(*p_Balance_FOR_XY)(unsigned short,unsigned short);
			//p_Balance_FOR_XY=balance_A_X;
			(*p_Balance_FOR_XY)(ADC_ConvertedValue[0],ADC_ConvertedValue[1]);
		}

		DMA_ClearITPendingBit(DMA1_IT_TC1);
		
	}
}

void W5500_IRQHandler(void)
{
	if(EXTI_GetITStatus(WIZ_INT_EXTI_LINE) != RESET)
	{
		  k = getSn_IR(SOCK_TCPS);
		    setSn_IR(SOCK_TCPS,k&0xef);
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_RECV|Sn_IR_CON);								          /*清除接收中断标志位*/
				//set_AD9854(4);
			}
			len=getSn_RX_RSR(SOCK_TCPS);									            /*定义len为已接收数据的长度*/
			if(len>0)
			{
				recv(SOCK_TCPS,CtrlWord,len); 				/*接收来自Client的数据*/
		  }
			if(CtrlWord[0]==0)
			{
				i=CtrlWord[1];
				set_AD9854(i);
				CtrlWord[0]=3;
			}
			else if(CtrlWord[0]==1)
			{
				j=CtrlWord[1];
				PWM_DutyRatio(PWM_TIM,j);
				CtrlWord[0]=3;
			}
		  else if(CtrlWord[0] == 2)
			{
				equilflag = 1;
				CtrlWord[0] = 3;
			}
			else
			{
				i = CtrlWord[0]>>4;
				if(i == 4)
				{
					DAC1Val = (uint16_t)((CtrlWord[0] & 0x0f)<<8) + CtrlWord[1];
					DAC_Setdata(DAC1Val,DAC2Val);
				}
				else if(i == 5)
				{
					DAC2Val = (uint16_t)((CtrlWord[0] & 0x0f)<<8) + CtrlWord[1];
					DAC_Setdata(DAC1Val,DAC2Val);
				}
				CtrlWord[0] = 3;
			}

			EXTI_ClearITPendingBit(WIZ_INT_EXTI_LINE); 
	}
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
