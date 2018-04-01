/**
  ******************************************************************************
  * @file    ebox_wdg.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/16
  * @brief   STM32F0,基于HAL库LL层
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ebox_wdg.h"
#include "stm32f0xx_ll_rcc.h"

/**
 * 初始化独立看门狗
   基本计算方法
 * pr:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^pr.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
   本函数内部已经做了相关计算；
    输入参数为ms；1000代表1000ms；请在1s内喂一次狗。否则将会复位
 */

const uint16_t PR[7] = {4,8,16,32,64,128,256};

/**
 *@brief    启动看门狗,最长延时26208ms
 *@param    ms 延时时长，单位ms
 *@retval   NONE
*/
void E_WDG::begin(uint16_t ms)
{
	uint8_t pr  = 0;	//分频数 0-7
	uint16_t rlr = 0;	//重载寄存器值，12位有效位，最大值4095
	// 计算重载值
	for (pr = 0; pr < 7; pr++)
	{
		rlr = ms * 40 / PR[pr];
		if (rlr <= 0x0fff) break;
	}
	// 当计算出的重载值大于0xfff时，取最大值rlr = 4095, pr = 6;
	// 此时延时Tout=(256*4095)/40 (ms)
	if ( rlr > 0x0fff || pr >= 7 )
	{
		pr = 6;
		rlr = 0xfff;
	}

	LL_RCC_LSI_Enable();
	while (LL_RCC_LSI_IsReady() != 1){}

	/* Configure the IWDG with window option disabled */
	/* ------------------------------------------------------- */
	/* (1) Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register */
	/* (2) Enable register access by writing 0x0000 5555 in the IWDG_KR register */
	/* (3) Write the IWDG prescaler by programming IWDG_PR from 0 to 7 - LL_IWDG_PRESCALER_4 (0) is lowest divider*/
	/* (4) Write the reload register (IWDG_RLR) */
	LL_IWDG_Enable(IWDG);                             /* (1) */
	LL_IWDG_EnableWriteAccess(IWDG);               /* (2) */
	LL_IWDG_SetPrescaler(IWDG, pr);                 /* (3) */
	LL_IWDG_SetReloadCounter(IWDG, rlr);           /* (4) */
}

/**
 *@brief    喂狗
 *@param    none
 *@retval   NONE
*/
void E_WDG::feed(void)
{
      /*reload*/
	LL_IWDG_ReloadCounter(IWDG);
}
