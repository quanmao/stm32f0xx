/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
  * @brief   ebox application example .
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
#include "ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 sleep,RTC wakeup example"
#define EXAMPLE_DATE	"2018-04-021"
#define DEMO_VER			"1.0"

// ���ڣ�led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);
// ע��E_RTCû����ʾ���캯����Ĭ��Ϊ�޲ι��죬����rtc���治������
E_RTC rtc;
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
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	usart.printf("* flash size : %d KB \r\n",cpu.chip_id[0],cpu.flash_size);
	usart.printf("* core       : %d Hz\r\n",clock.core);
  usart.printf("* hclk       : %d Hz\r\n",clock.hclk);
  usart.printf("* pclk1      : %d Hz\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("*************************************************************\n\r");
}

void test()
{
  static uint8_t i = 0;
  led.toggle();
  usart.read();   // ���read�ж�
	usart.write(0x30+i++);
  if (i >= 5){
    ebox_exitSleepOnExti();
    i = 0;
  }
}

void setup()
{
  Time_T time = {LL_RTC_TIME_FORMAT_AM_OR_24, 22, 56, 1};

  ebox_init();
  led.mode(OUTPUT_PP);
  get_chip_info(&cpu);
  usart.begin(115200);
  PrintfLogo();

  // EOK,��ʼ���ɹ�������RTCʱ�������У�����Ҫ�������ڣ�ʱ�䡣������Ҫ����
  if (rtc.begin() != E_OK)
  {
    rtc.setTime(time);
  }
  // ��������
  rtc.getTime(&time);
  time.Minutes += 1;
  time.Seconds += 0;
  
  rtc.setAlarm(time,eRtc_MaskDateWeekDay|eRtc_MaskHours|eRtc_MaskMinutes|eRtc_MaskSeconds);
  usart.printf("\n\r ����ÿ��һ�� ");
  rtc.attach_alarm_interrupt(&test);
  rtc.alarmOnOff(ENABLE);
}

int main(void)
{
  setup();


  usart.printf("ebox_core.cpp��debug����Ϊ��1������������������Ϣ\r\n\r\n");
  if (isWakeFromSB())
  {
    rtc.alarmOnOff(DISABLE);
    usart.printf("\r\n��λ���ͣ�0x%x ,standbyģʽ����,60s�������ѭ�� \r\n",get_resetType());
    delay_ms(60000);
  }else{
    // ���ж�,��������MCU
    usart.attach(&test,RxIrq);
    usart.enable_irq(RxIrq);

    usart.printf("\r\n��λ���ͣ�0x%x ,30s�����͹�����ʾ\r\n",get_resetType());

    // sleep mode test
    delay_ms(30000);
    usart.printf("\r\n sleep nowģʽ,usart,user��(�����ж�)����\r\n");
    ebox_sleep(sleep_now);
    usart.printf("\r\n �Ѿ����ѣ�30�����sleep_on_extiģʽ\r\n");
    delay_ms(30000);
    usart.printf("\r\n sleep_on_extiģʽ�usart,user��(�����ж�)����,���Ѻ�ִ�����ж���������������ߣ�5���жϺ���\r\n");
    ebox_sleep(sleep_on_exti);
    // stop mode test
    usart.printf("\r\n �Ѿ����ѣ�30�����stopģʽ\r\n");
    delay_ms(30000);
    usart.printf("\r\n stopģʽ��user(�ⲿ�ж�)������\r\n");
    ebox_stop(sleep_now);

    usart.printf("\r\n �Ѿ����ѣ�30�����standbyģʽ\r\n");
    delay_ms(30000);
    usart.printf("\r\n standbyģʽ��Ĭ��PA0����\r\n");
    ebox_standby();
  }
  usart.printf("\r\n ����������ѭ��,led��˸\r\n");
  while (1)
  {
    delay_ms(500);
    led.toggle();
  }
}




