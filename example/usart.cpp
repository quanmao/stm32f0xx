 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2017/07/07
  * @brief   uart例程，包含中断绑定 .
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
#define EXAMPLE_NAME	"STM32F0 USART example"
#define EXAMPLE_DATE	"2017-07-07"
#define DEMO_VER			"1.0"

E_GPIO PA5(PA_5);
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

// 接收中断回调函数
void rxirq(void){
	PA5 = 1;
	
	usart.printf("hello World ! %d \r\n",usart.read());
}

void setup()
{
	ebox_init();
	PA5.mode(OUTPUT_PP);
	usart.begin(115200);
	get_chip_info(&cpu);
	// 绑定中断回调函数，可以绑定对象成员，也可以绑定静态函数
	usart.attach(&PA5,&E_GPIO::reset,TxIrq);
//	usart.attach(&rxirq,RxIrq);
	// 开中断
	usart.enable_irq(TxIrq);
//	usart.enable_irq(RxIrq);
}

int main(void)
{
		E_STATE s;
		uint16_t r;
    setup();
		PrintfLogo();
    while(1)
    {
			if(usart.read(&r) == E_OK)
			{
				usart.printf("recived is %c \r\n",r);
			}
    }
}




