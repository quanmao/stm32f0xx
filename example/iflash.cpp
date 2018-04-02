/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   iflash 例程
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

// 从用户区起始地址创建一个闪存，默认为1 page,大小等于1*pagesize
E_Flash  Fh(0);
// 从用户区起始地址+2page处，创建一个闪存，3 page,大小等于3*pagesize
E_Flash	 fl(2,3);

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F0 USART_POLLING example"
#define EXAMPLE_DATE	"2018-02-07"
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

void setup()
{
	ebox_init();
	usart.begin(115200);
	get_chip_info(&cpu);
	PrintfLogo();
	usart.printf("E_FLASH test \r\n");
}

uint8_t buf[] = "flash test -  ";
char rec[20];

int main(void)
{
	int i = 20,j=0;
	setup();
	// 从地址0处读取1个字节，结果保存在rec中
	Fh.read(0,(uint8_t*)rec,1);
	usart.printf("flash test; 第 %d 次开机！ \r\n",rec[0]);
	rec[0]++;
	// 将读取到的rec[0]自加后重新写入iflash中，作为开机次数
	Fh.write(0,(uint8_t*)rec,1);
	while (1)
	{	
			while(j<2){
				j++;
				buf[13] = j+0x30;
				i = fl.write(0,buf,sizeof(buf)/sizeof(buf[0]));
				delay_ms(10);
				if(i>0) {
				fl.read(0,(uint8_t*)rec,i);
				usart.write(rec,i-1);
				usart.printf("\r\n");
				}
			 delay_ms(2000);
			}
	}
}




