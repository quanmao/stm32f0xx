/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
一个独立看门狗的示例程序
*/
#include "ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 iwdg example"
#define EXAMPLE_DATE	"2018-04-04"
#define DEMO_VER			"1.0"
E_WDG dog;
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


void setup()
{
	ebox_init();
	usart.begin(115200);
	get_chip_info(&cpu);
	PrintfLogo();
	usart.printf("reset !!!\r\n");
	dog.begin(26208);
}

int main(void)
{

	setup();

	while (1)
	{
//	dog.feed();			//应该在定时器中断中定时喂狗,如果不按时喂狗，设备会重启
		usart.printf("running!\r\n");
//        delay_ms(1000);
//        delay_ms(1900);
		delay_ms(25000);
		//        delay_ms(2000);
		//        delay_ms(3000);
	}
}




