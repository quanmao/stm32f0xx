 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2017/07/13
  * @brief   ebox exti example, 基于stm32nucleo(072)平台验证	
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
#define EXAMPLE_NAME	"STM32F0 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"
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

uint32_t xx;

// led
E_GPIO led(PA_5);
// 两种方式创建exti对象
E_GPIO	PA5(PC_13);
E_EXTI userbtn(&PA5);
E_EXTI userbt1(PA_0,INPUT_PU);

/**
 *@brief    静态回调函数
 *@param    none
 *@retval   none
*/
void fall()
{
    xx--;
    usart.printf("\r\n falling,xx = %d", xx);
}

void rise()
{
    xx++;
    usart.printf("\r\n rising, xx = %d", xx);
}

void fallrise(){
		xx++;
		usart.printf("\r\n fallrise, xx = %d", xx);
}

/**
 *@brief    测试类，用来测试类成员绑定
 *@param    none
 *@retval   none
*/
class Test 
{
    public:
    void event() 
    {
        led.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    usart.begin(115200);
	  get_chip_info(&cpu);
		PrintfLogo();
	
    led.mode(OUTPUT_PP);
		// 上升沿，下降沿均触发
		userbtn.attach(fallrise,FALL_RISING);
		userbtn.ENABLE(FALL_RISING);
		// 上升沿，下降沿调用不同的回调函数
    userbt1.attach(rise,RISING);
		userbt1.attach(&test,&Test::event,FALLING);
		userbt1.ENABLE(FALL_RISING);	

}


int main(void)
{
    setup();
    while(1)
    {
        ;
    }
}




