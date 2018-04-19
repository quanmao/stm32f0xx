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

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 sleep example"
#define EXAMPLE_DATE	"2018-04-07"
#define DEMO_VER			"1.0"

// 串口，led
E_UART usart(USART1,PA_9,PA_10);
E_GPIO led(PA_5);
E_EXTI buttn(PC_13);
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
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	usart.printf("* flash size : %d KB \r\n",cpu.chip_id[0],cpu.flash_size);
	usart.printf("* core       : %d Hz\r\n",clock.core);
  usart.printf("* hclk       : %d Hz\r\n",clock.hclk);
  usart.printf("* pclk1      : %d Hz\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}

void setup()
{
    ebox_init();
		led.mode(OUTPUT_PP);
		get_chip_info(&cpu);
    usart.begin(115200);
}


void test()
{
  static uint8_t i = 0;
  led.toggle();
  usart.read();   // 清空read中断
	usart.write(0x30+i++);
  if (i > 5){
    ebox_exitSleepOnExti();
    i = 0;
  }
}



int main(void)
{
  setup();
  PrintfLogo();

  usart.printf("ebox_core.cpp中debug定义为：1，可以输出更多调试信息\r\n\r\n");
  if (isWakeFromSB())
  {
    usart.printf("复位类型：0x%x ,standby模式唤醒,60s后进入主循环 \r\n",get_resetType());
    /* Clear Standby flag */
//    LL_PWR_ClearFlag_SB();
    delay_ms(60000);
  }else{
    // 绑定中断,用来唤醒MCU
    usart.attach(&test,RxIrq);
    usart.enable_irq(RxIrq);

    buttn.attach(&test,FALLING);
    buttn.ENABLE(FALLING);

    usart.printf("复位类型：0x%x ,30s后进入低功耗演示\r\n",get_resetType());

    // sleep mode test
    delay_ms(30000);
    usart.printf("sleep now模式,usart,user键(任意中断)唤醒\r\n");
    ebox_sleep(sleep_now);
    usart.printf("已经唤醒，30后进入sleep_on_exti模式\r\n");
    delay_ms(30000);
    usart.printf("sleep_on_exti模式sart,user键(任意中断)唤醒,唤醒后执行完中断任务继续进入休眠，5次中断后唤醒\r\n");
    ebox_sleep(sleep_on_exti);
		// stop mode test
    usart.printf("已经唤醒，30后进如stop模式\r\n");
    delay_ms(30000);
    usart.printf("stop模式，user(外部中断)键唤醒\r\n");
    ebox_stop(sleep_now);

		usart.printf("已经唤醒，30后进如standby模式\r\n");
    delay_ms(30000);
    usart.printf("standby模式，默认PA0唤醒\r\n");
    ebox_standby();
  }
  usart.printf("即将进入主循环,led闪烁\r\n");
  while (1)
  {
    delay_ms(500);
    led.toggle();
  }
}




