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
  2018/4/22
    1  修复LSE模式下闹铃无法响应问题
    2  添加闹铃响应字段定义
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

#define eRtc_NoMask                         LL_RTC_ALMA_MASK_NONE          // 不屏蔽,date,hour,minutes,seconds均参与匹配
#define eRtc_MaskDateWeekDay                LL_RTC_ALMA_MASK_DATEWEEKDAY   // 不匹配年月日
#define eRtc_MaskHours                      LL_RTC_ALMA_MASK_HOURS         // 不匹配小时
#define eRtc_MaskMinutes                    LL_RTC_ALMA_MASK_MINUTES       // 不匹配分钟
#define eRtc_MaskSeconds                    LL_RTC_ALMA_MASK_SECONDS       // 不匹配秒
#define eRtc_MaskAll                        (RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1) //屏蔽所有位,此时闹铃每秒触发一次

class E_RTC
{
public:
//	E_RTC(void);  //1:LSE;0:LSI如果使用外部晶振
	
	/**
		* @brief  初始化RTC,默认LSE,如果LSE启动失败,会自动转为LSI
		* @param  ClockS clock 时钟源，clock_lse,clock_lsi
		* @retval E_TIMEOUT 时钟配置失败,E_OK 时钟配置成功,无需设置时钟，E_NG 时钟配置成功，需要设置时钟
		*/
  int begin(ClockS clock = clock_lse);
  // 设置闹铃，默认不匹配日期
  void setAlarm(Time_T time,uint32_t mask = eRtc_MaskDateWeekDay);	
  void attach_alarm_interrupt(void (*cb_fun)(void));
	void alarmOnOff(FunctionalState state);
  
  // 设置日期，时间
	void setDate(Date_T date);
	void setTime(Time_T time);
	// 读取日期，时间	
	void getDateTime(date_time_t *datetime);
	void getTime(Time_T *time);
	void getDate(Date_T *date);  


private:
	int    	_config(ClockS clock);
  void    _setFormat(uint32_t formant);
	uint8_t _getTimeFlag(void);
	void    _setTimeFlag(void);
	void    _nvic(void);
};

#endif

