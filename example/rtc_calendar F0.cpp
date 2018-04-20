/**
  ******************************************************************************
  * @file    rtc_calendar.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   rtc 例程
	*     STM32F0RTC例程，注意，和F1系列不通用
  *    1 创建rtc对象
  * 	 2 调用begin()初始化时钟。1 LSE  0 LSI
  *	   3 设置时钟
  *	   4 设置闹铃，附加闹铃中断回调函数
	*		 5 时钟从LSI改为LSE时，需要断电重启一次LSE才生效
	
	已知bug：LSE时闹铃无效
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

#include "ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 RTC example"
#define EXAMPLE_DATE	"2017-07-18"
#define DEMO_VER			"1.0"

E_RTC rtc(clock_lse);
//E_RTC rtc(clock_lse);
// 串口，led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);

cpu_t	cpu;

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	usart.printf("\n\r");
	usart.printf("*************************************************************\n\r");
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	usart.printf("* 例程版本   : %s\r\n", DEMO_VER);			/* 打印例程版本 */
	usart.printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f0xx.h文件中 */
	usart.printf("* CMSIS版本  : V%d.%d.%d (STM32 HAL lib)\r\n", __STM32F0_DEVICE_VERSION_MAIN,
			__STM32F0_DEVICE_VERSION_SUB1,__STM32F0_DEVICE_VERSION_SUB2);
	usart.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
			usart.printf("* CPUID = %08X %08X %08X ,flash size = %d KB \n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0],cpu.flash_size);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}



// 每分钟输出一次闹铃
void exit()
{
	Time_T time;
	usart.printf("\n\r\n\r");
	usart.printf("#######################  闹铃  ##############################\n\r");
	rtc.getTime(&time);
	usart.printf("#####                 %2d:%02d:%02d                       ######",time.Hours,time.Minutes,time.Seconds);
	time.Minutes += 1;
	rtc.setAlarm(time);
	usart.printf("\r\n");
	usart.printf("#############################################################\n\r");
}

void setup()
{
	Date_T date = {LL_RTC_WEEKDAY_WEDNESDAY, 13, LL_RTC_MONTH_SEPTEMBER, 16};
	Time_T time = {LL_RTC_TIME_FORMAT_AM_OR_24, 22, 56, 1};

	ebox_init();
	get_chip_info(&cpu);
	usart.begin(115200);

	PrintfLogo();
// EOK,初始化成功，并且RTC时间在运行，不需要设置日期，时间。否则需要设置
	if (rtc.begin() != E_OK)
	{
		rtc.setDate(date);
		rtc.setTime(time);
	}
	// 设置闹铃
	rtc.getTime(&time);
	time.Minutes += 1;
	time.Seconds += 0;
	rtc.setAlarm(time);
	usart.printf("\n\r 闹铃时间设定为：%2d:%02d:%02d秒",time.Hours,time.Minutes,time.Seconds);
	rtc.attach_alarm_interrupt(&exit);
  rtc.alarmOnOff(ENABLE);
}

uint8_t Week[7][3] = {{"一"},{"二"},{"三"},{"四"},{"五"},{"六"},{"日"}};

int main(void)
{
	// date_time_t 声明在common.h中，包含年月日时分秒星期信息
	date_time_t dtime;
	setup();
	while (1)
	{
		// 每30s读取一次时间
		rtc.getDateTime(&dtime);
		usart.printf("\n\r %2d:%02d:%02d秒",dtime.hour,dtime.min,dtime.sec);
		usart.printf("\n\r 20%2d年%02d月%2d日 星期%02d",dtime.year,dtime.min,dtime.date,dtime.week);
		delay_ms(30000);
	}
}


