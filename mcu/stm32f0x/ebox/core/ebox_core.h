/**
  ******************************************************************************
  * @file    ebox_core.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   此文件为ebox核心文件,提供了诸多非常重要的接口,可以被标准的c文件调用。
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

#ifndef __CORE_H
#define __CORE_H


#ifdef __cplusplus
extern "C" {
#endif
// ebox 组件
#include "ebox_define.h"
#include "ebox_config.h"
#include "ebox_mem.h"
// 目标cpu组件
#include "target_mcu.h"
	

	
typedef struct
{
	uint32_t core;		// SYSCLK 系统时钟
	uint32_t hclk;		// AHB总线时钟，有系统时钟分频得到，一般不分频，等于系统时钟
	uint32_t pclk1;		// APB1外设时钟
}cpu_clock_t; 

typedef struct
{
//	uint32_t    ability;      //cpu calculate plus per second;
//	cpu_clock_t	clock;
	uint32_t    chip_id[3];     //芯片id
	uint16_t    flash_size;     //flash大小
}cpu_t;

extern cpu_clock_t clock;

///////全局变量、函数///////////////////////////////////////////////
extern __IO uint32_t millis_seconds;					//提供一个mills()等效的全局变量。降低cpu调用开销

// 初始化
extern void ebox_init(void);                  //ebox系统初始化
extern void SystemClock_Config(void);       	//外部实现该函数，可以修改时钟源以及时钟频率。默认工作在48Mhz

// 时间相关接口
extern uint32_t micros(void);                 //获取系统当前运行了多长时间。单位：us
uint32_t millis(void);   											//获取系统当前运行了多长时间。单位：ms
extern void delay_ms(uint32_t ms);            //延时n毫秒。可在任意情况下使用，包括禁止所有中断的情况。精度正负1ms。
extern void delay_us(uint16_t us);            //延时n微秒。可在任意情况下使用，包括禁止所有中断的情况。48M主频下,精度正负1us。

// 芯片信息
extern void get_system_clock(void);						//获取CPU时钟,结果保存在全局变量clock中
extern void get_chip_info(cpu_t *cpu);				//返回cpu信息,结果保存在cpu中

// systick中断回调函数
extern E_STATE SystickCallBackRegister(fun_noPara_t fun);                       // 绑定中断回调函数，使用默认周期 1ms
extern E_STATE SystickCallBackMultipleReg(fun_noPara_t fun,uint16_t multiple); // 绑定中断回调函数，允许则定义周期 = multiple * 1ms; 最大65535
// power manage
extern void ebox_sleep(void);									//仅关闭CPU时钟，所有外设仍运行，可以通过任何中断唤醒
extern void ebox_stop();
extern void ebox_standby(void);								//wkup pin拉高,外部复位,rtc alarm,内部看门够可唤醒

#define RST_LOWPOWER	0x80
#define RST_WWDOG			0x40
#define RST_IWDOG			0x20
#define RST_SOFT			0x10
#define RST_POWER			0x08
#define RST_NRST			0x04
#define RST_OBL				0x02

// 默认返回值为0x0c,即RST_POWER|RST_NRST
__STATIC_INLINE uint8_t get_resetType(){
		return (RCC->CSR)>>24;
}

// 软件重启，执行后复位寄存器返回值为0x1c
__STATIC_INLINE void ebox_rst(){
		NVIC_SystemReset();
}
#ifdef __cplusplus
}
#endif
// 以下部分还未优化
class eBox{
public:
	eBox(int i){};
	void sleep(void);
	void stop(void);
	void standby(void);
};


#endif
