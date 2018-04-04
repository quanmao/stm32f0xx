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
/**
 * -LQM (2017/7/12)
 *     注意：此部分和F1系列不通用，提供了2中ADC模式，一种是单通单次采集，不适用DMA
 *  一种连续采集（指定采样次数，单通道或多通道） 
 *  单通道单次采集,参考电压定义在ebox_analog.h中 VDDA_APPLI 
 *    1  声明对象   E_Analog adc1(new E_PinBase(PA_0));
 *    2  提供read()和getVoltage()方法
 */

#include "ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 ANALOG example"
#define EXAMPLE_DATE	"2017-07-24"
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
	usart.printf("* core       : %d Hz\r\n",clock.core);
  usart.printf("* hclk       : %d Hz\r\n",clock.hclk);
  usart.printf("* pclk1      : %d Hz\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* 打印一行空格 */
	usart.printf("*************************************************************\n\r");
}

// 注意,ADC端口电压不能超过3.3v,超过会造成温度和vbat读数异常
E_ADC adc1(PA_0);
E_ADC tem(temperature_ch);
#if defined(ADC_CCR_VBATEN)
E_ADC vbat(battery_ch);
#endif
E_ADC ad2(PA_1,3317);


// E_AnalogDMA adcs(new E_PinBase(PA_0));

void setup()
{
    ebox_init();
    usart.begin(115200);
		get_chip_info(&cpu);
		PrintfLogo();
}

int main(void)
{
    setup();
    while(1)
    {
		usart.printf("ch1 = %d mv ",adc1.getVoltage());
		delay_ms(800);
#if defined(ADC_CCR_VBATEN)
		usart.printf("Vbat = %d mv",vbat.getVoltage()*2);
		delay_ms(800);
#endif
		usart.printf("ch2 = %.2f℃ \r\n",tem.getTemperature());
    delay_ms(2000);
		 }
}
