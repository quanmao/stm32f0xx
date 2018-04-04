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

#include "ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 ANALOG DMA example"
#define EXAMPLE_DATE	"2017-08-02"
#define DEMO_VER			"1.0"

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
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	usart.printf("* flash size : %d KB \r\n",cpu.chip_id[0],cpu.flash_size);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}

E_AdcDMA adcs(2);


// E_AnalogDMA adcs(new E_PinBase(PA_0));

void setup()
{
    ebox_init();
    usart.begin(115200);
		get_chip_info(&cpu);
		PrintfLogo();
		adcs.addChannel(PA_0);
		adcs.addChannel(PA_2);
		adcs.dmaConfig();
		
}

int main(void)
{
    setup();
	adcs.dmaStart();
    while(1)
    {
		adcs.update();
		usart.printf("ch1 = %d mv | ",3317*adcs.r_buffer[0]/4095);
		usart.printf("ch2 = %d mv \r\n",3317*adcs.r_buffer[1]/4095);
    delay_ms(2000);
		 }
}
