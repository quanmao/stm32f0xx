/**
  ******************************************************************************
  * @file    ebox_wdg.h
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/16
  * @brief   STM32F0,����HAL��LL��
		��������core.cpp�ļ�
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
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_WDG_H_
#define __EBOX_WDG_H_
#include "ebox_core.h"
#include "stm32f0xx_ll_iwdg.h"


class E_WDG
{
public:
	E_WDG() {};
	/**
	*@brief    �������Ź�,���ʱ26208ms
	*@param    ms ��ʱʱ������λms
	*@retval   NONE
	*/
	void begin(uint16_t ms);
	/**
	*@brief    ι��,�����Ź�ȥ�26208ʱ������ι�������Ҫ����25s
	*@param    none
	*@retval   NONE
	*/
	void feed();
};

#endif
