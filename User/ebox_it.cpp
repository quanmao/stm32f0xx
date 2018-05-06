/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_RTC/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    27-May-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ebox_core.h"

#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_adc.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_STANDBY_RTC
  * @{
  */


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/



extern "C" {
	
__IO uint16_t t1_overflow_times = 0;
__IO uint16_t t2_overflow_times = 0;
__IO uint16_t t3_overflow_times = 0;
__IO uint16_t t4_overflow_times = 0;

void DMA1_Channel1_IRQHandler(void){


}

void ADC1_COMP_IRQHandler(void)
{
	
//	  /* Check whether ADC group regular overrun caused the ADC interruption */
//  if(LL_ADC_IsActiveFlag_OVR(ADC1) != 0)
//  {
//    /* Clear flag ADC group regular overrun */
//    LL_ADC_ClearFlag_OVR(ADC1);
//    
//    /* Call interruption treatment function */
//  }

}
//#include "ebox_gpio.h"
//extern E_GPIO led;
//void TIM2_IRQHandler(void)
//{
//  /* Check whether update interrupt is pending */
//  if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
//  {
//    /* Clear the update interrupt flag*/
//		
//    LL_TIM_ClearFlag_UPDATE(TIM2);
//		led.toggle();

////	irq_handler(tim_irq_ids[0]);
//  }
//  
//  /* TIM2 update interrupt processing */

//}
#if !defined(STM32F030x6)
//void TIM6_DAC_IRQHandler(void)
//{
//	LL_TIM_ClearFlag_UPDATE(TIM6);
//}
#endif

//void TIM2_IRQHandler(void)
//{
//	uint8_t i;
//	
//	LL_TIM_ClearFlag_UPDATE(TIM2);
//	i=i;
//}
extern fun_noPara_t irq_handler;

//void TIM3_IRQHandler(void)
//{
////	if (LL_TIM_IsActiveFlag_UPDATE(TIM3))
////	{
////		t3_overflow_times++;
////		//tim_irq_callback(TIM3_IT_Update);
////		LL_TIM_ClearFlag_UPDATE(TIM3);
////	}
////	if (LL_TIM_IsActiveFlag_CC1(TIM3) == 1)
////	{			
////		LL_TIM_ClearFlag_CC1(TIM3);
////		tim_irq_callback(TIM3_IT_CC1);
////	}
////	if (LL_TIM_IsActiveFlag_CC2(TIM3) == 1)
////	{
////		tim_irq_callback(TIM3_IT_CC2);
////		LL_TIM_ClearFlag_CC2(TIM3);
////	}
////	if (LL_TIM_IsActiveFlag_CC3(TIM3) == 1)
////	{
////		tim_irq_callback(TIM3_IT_CC3);
////		LL_TIM_ClearFlag_CC3(TIM3);
////	}
////	if (LL_TIM_IsActiveFlag_CC4(TIM3) == 1)
////	{
////		tim_irq_callback(TIM3_IT_CC4);
////		LL_TIM_ClearFlag_CC4(TIM3);
////	}
//}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	if (LL_TIM_IsActiveFlag_UPDATE(TIM1))
	{
		LL_TIM_ClearFlag_UPDATE(TIM1);
		t1_overflow_times++;
	}	
}

/**
  * @brief   This function handles NMI exception.
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
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
extern void ebox_SysTick_Handler(void);  // 具体的实现在ebox_core.cpp中
void SysTick_Handler(void)
{	
		ebox_SysTick_Handler();
}
}

/******************************************************************************/
/*                  STM32F0xx Peripherals Interrupt Handlers                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles RTC Auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{

}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
