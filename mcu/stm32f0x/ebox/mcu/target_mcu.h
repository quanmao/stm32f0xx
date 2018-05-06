/******************************************************************************
程 序 名： target_mcu.h　
编 写 人： cat_li
编写时间： 2018年05月06日
相关平台： STM32F1xx, ebox
修改日志：　　
	NO.1-  初始版本
******************************************************************************/

#ifndef __MCU_SELECT_H_
#define __MCU_SELECT_H_

#include "ebox_config.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#if 	defined(STM32F103xB)
	#include "stm32f030_define.h"
#elif defined(STM32F103xE)
	#include "stm32f072_define.h"	
#else
	#error 	 "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
	#define STM32_SRAM_SIZE					16		//内存大小，ebox_mem调用
#endif
	 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
