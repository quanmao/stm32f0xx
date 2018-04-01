/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API中关于修改CPU频率,以及延时部分。
	*	1  通过修改不同的CPU频率，可以看到启动速度，以及us延时的影响
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
#include "ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 cpuClock example"
#define EXAMPLE_DATE	"2017-12-31"
#define DEMO_VER			"1.0"

// 串口，led
E_UART usart(USART1,UART1_TX,UART1_RX);
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
  usart.printf("* flash size : %d KB \r\n",cpu.flash_size);
  usart.printf("* core       : %d\r\n",clock.core);
  usart.printf("* hclk       : %d\r\n",clock.hclk);
  usart.printf("* pclk1      : %d\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}

E_GPIO led(PA_5);

#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_rcc.h"

// 通过不同的组合可以设置不同的工作频率，默认工作在48M,无需SystemClock_Config函数
#define	CPUCLOCK96M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK72M		LL_RCC_PLL_MUL_9 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK48M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2

#define CPUCLOCK			CPUCLOCK48M

void SystemClock_Config(void)
{
	/* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
	 Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
							 ((8MHz * 12)/ 2)                  = 48MHz             */
	LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {CPUCLOCK};
	/* Variable to store AHB and APB buses clock configuration */
	/* Settings to have HCLK set to 48MHz and APB to 48 MHz */
	LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

	/* Switch to PLL with HSI as clock source             */
	LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
}

void setup()
{
    ebox_init();
		usart.begin(115200);
		get_chip_info(&cpu);
		PrintfLogo();
    led.mode(OUTPUT_PP);
}

void testUs()
{
	uint8_t i = 255;
	while(i--){
				led.toggle();
			delay_us(1);
	}
}

void testMs()
{
	uint8_t i = 20;
	while(i--){
				led.toggle();
			delay_ms(1);
	}
}

int main(void){
		uint32_t t;
    setup();
		usart.printf("启动耗时：%d us,  %d ms \r\n",micros(),millis());
    while (1)
    {
			 t = micros();
			 testUs();
			 t = micros() - t;
			 usart.printf("testUs 耗时：%dus \r\n",t);
			 // 使用millis_seconds,减少调用消耗
			 t = millis_seconds;
			 testMs();
			 t = millis_seconds - t;
			 usart.printf("testMs 耗时：%dms \r\n",t);
    }
}
