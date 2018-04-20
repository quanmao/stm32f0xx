/**
  ******************************************************************************
  * @file    rtc_calendar.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   rtc ����
	*     STM32F0RTC���̣�ע�⣬��F1ϵ�в�ͨ��
  *    1 ����rtc����
  * 	 2 ����begin()��ʼ��ʱ�ӡ�1 LSE  0 LSI
  *	   3 ����ʱ��
  *	   4 �������壬���������жϻص�����
	*		 5 ʱ�Ӵ�LSI��ΪLSEʱ����Ҫ�ϵ�����һ��LSE����Ч
	
	��֪bug��LSEʱ������Ч
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

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 RTC example"
#define EXAMPLE_DATE	"2017-07-18"
#define DEMO_VER			"1.0"

E_RTC rtc(clock_lse);
//E_RTC rtc(clock_lse);
// ���ڣ�led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);

cpu_t	cpu;

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	usart.printf("\n\r");
	usart.printf("*************************************************************\n\r");
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	usart.printf("* ���̰汾   : %s\r\n", DEMO_VER);			/* ��ӡ���̰汾 */
	usart.printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f0xx.h�ļ��� */
	usart.printf("* CMSIS�汾  : V%d.%d.%d (STM32 HAL lib)\r\n", __STM32F0_DEVICE_VERSION_MAIN,
			__STM32F0_DEVICE_VERSION_SUB1,__STM32F0_DEVICE_VERSION_SUB2);
	usart.printf("* EBOX��汾 : %s (ebox)\r\n", EBOX_VERSION);
			usart.printf("* CPUID = %08X %08X %08X ,flash size = %d KB \n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0],cpu.flash_size);
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("*************************************************************\n\r");
}



// ÿ�������һ������
void exit()
{
	Time_T time;
	usart.printf("\n\r\n\r");
	usart.printf("#######################  ����  ##############################\n\r");
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
// EOK,��ʼ���ɹ�������RTCʱ�������У�����Ҫ�������ڣ�ʱ�䡣������Ҫ����
	if (rtc.begin() != E_OK)
	{
		rtc.setDate(date);
		rtc.setTime(time);
	}
	// ��������
	rtc.getTime(&time);
	time.Minutes += 1;
	time.Seconds += 0;
	rtc.setAlarm(time);
	usart.printf("\n\r ����ʱ���趨Ϊ��%2d:%02d:%02d��",time.Hours,time.Minutes,time.Seconds);
	rtc.attach_alarm_interrupt(&exit);
  rtc.alarmOnOff(ENABLE);
}

uint8_t Week[7][3] = {{"һ"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"}};

int main(void)
{
	// date_time_t ������common.h�У�����������ʱ����������Ϣ
	date_time_t dtime;
	setup();
	while (1)
	{
		// ÿ30s��ȡһ��ʱ��
		rtc.getDateTime(&dtime);
		usart.printf("\n\r %2d:%02d:%02d��",dtime.hour,dtime.min,dtime.sec);
		usart.printf("\n\r 20%2d��%02d��%2d�� ����%02d",dtime.year,dtime.min,dtime.date,dtime.week);
		delay_ms(30000);
	}
}


