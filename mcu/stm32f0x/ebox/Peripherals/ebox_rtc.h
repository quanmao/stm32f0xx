/**
  ******************************************************************************
  * @file    ebox_rtc.h
  * @author  cat_li
  * @brief   STM32F0 @ HAL lib
			todo list
			1	中断绑定
	2017/7/19
		1  修复无法启动LSE作为时钟源的bug
		2  修复无法重复设置闹铃的bug
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

#ifndef __EBOX_RTC_H_
#define __EBOX_RTC_H_
#include "ebox_core.h"
#include "stm32f0xx_ll_rtc.h"


/**
 * 说明:
 * -LQM (2016/9/12)
 *  1 LSE为时钟，当Vbat维持，VDD掉电，RTC仍然继续工作
 *  2 LSI为时钟，VDD掉电后，RTC状态不定
 *  3 HSE/32为时钟，VDD掉电或内部调压器（V1.8）掉电，RTC状态不定
 */

/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=40 kHz RC */
#define LSI_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSI_SYNCH_PREDIV           ((uint32_t)0x137)

/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define LSE_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSE_SYNCH_PREDIV           ((uint32_t)0x00FF)


typedef struct{
	uint32_t Format12_24;	//
	uint32_t Hours;			// 0-12 or 0-23
	uint32_t Minutes;		// 0-59
	uint32_t Seconds;		// 0-59
}Time_T;

typedef struct{
	Time_T time;
	uint32_t Day;
	uint32_t WeekDaySel;
}Alarm_T;

typedef struct{
	uint32_t WeekDay;	// 1-7
	uint32_t Day;		// 1-31
	uint32_t Month;		// 1-12
	uint32_t Year;		// 0-99
}Date_T;

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
}date_time_t;

typedef enum 
{
	clock_lsi = 0,
	clock_lse
}ClockS;

class E_RTC
{
public:
	E_RTC(ClockS clock){_clocks = clock;};  //1:LSE;0:LSI如果使用外部晶振
	int begin(void);
	//正常返回EOK
	//失败后会返回一个ETIMEOUT错误,并自动转为内部晶振。

//	void attach_overflow_interrupt(void (*cb_fun)(void));
	void attach_alarm_interrupt(void (*cb_fun)(void));
//	void attach_sec_interrupt(void (*cb_fun)(void));

//	void overflow_interrupt(FunctionalState state);
	void alarmOnOff(FunctionalState state);
//	void sec_interrupt(FunctionalState state);


	void setDate(Date_T date);
	void setTime(Time_T time);
	
	void setAlarm(Time_T time,uint32_t mask = LL_RTC_ALMA_MASK_DATEWEEKDAY);
	
	void getDateTime(date_time_t *datetime);
	void getTime(Time_T *time);
	void getDate(Date_T *date);  


private:
	ClockS	_clocks;
	int    	_config(ClockS clock);
  void    _setFormat(uint32_t formant);
	uint8_t _getTimeFlag(void);
	void    _setTimeFlag(void);
	void    _nvic(void);
};

#endif

