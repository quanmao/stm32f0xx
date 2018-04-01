/**
 ******************************************************************************
 * @file    mem_test.cpp
 * @author  cat_li
 * @version V1.2
 * @date    2017/12/29
 * @brief   ebox mem 管理测试,ebox_free_block_print()功能不祥
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
#define EXAMPLE_NAME	"STM32F0 mem example"
#define EXAMPLE_DATE	"2017-12-29"
#define DEMO_VER			"1.0"

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

uint8_t *ptr;
uint8_t *ptx[100];

//extern uint8_t dl;
	void prin(const char *fmt, ...){
		usart.printf(fmt);
	}

void setup()
{
  ebox_init();
  usart.begin(115200);
  get_chip_info(&cpu);
  PrintfLogo();

//  usart.printf(" %d \r\n",dl);
  usart.printf("----------------mem test--------------------\r\n");
  usart.printf("可分配内存 :0x%x---0x%x(%dK%dbytes)\r\n",ebox_get_sram_start_addr(),ebox_get_sram_end_addr(),(ebox_get_free())/1024,(ebox_get_free())%1024);
  int     i = 0;

  delay_ms(100);
  usart.printf("ptr ID:\t\tADDRE\t\t|free \r\n");
  for ( i = 0; i < 100; i++)
  {
    // 分配长度为100的内存空间,实际分配空间为(100+7)/8 = 13  13*8 = 104  104+8(块管理) =112
    ptx[i] = (uint8_t *)ebox_malloc(100);
    if (ptx[i] == NULL)
      usart.printf("game over\r\n");
    else
    // 向划分的内存中写入16个字节0xff
      for (int j = 0; j < 16; j++)
      {
        ptx[i][j] = 0xFF;
      }

    usart.printf("ptr %03d:\t0X%X\t|%05d\r\n",i,ptx[i],ebox_get_free());
  }
//	  ebox_free_block_print();
  // 检查指定内存是否写入成功
  for ( i = 0; i < 100; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      if (ptx[i][j] != 0xFF)
        usart.printf("ptr err%03d:0X%X\r\n",i,ptx[i]);
    }
  }
  // 释放内存
  usart.printf("释放内存\r\n");
	usart.printf("ptr ID:\t\t ADDRE\t\t size\t free\r\n");
  for ( i = 0; i < 100;  i ++)
  {
    ebox_free(ptx[i]);
    usart.printf("ptr %03d:\t 0X%X\t |%d\t |%05d \r\n",i,ptx[i],ptx[i] - ptx[i -1],ebox_get_free());
  }

//  ebox_free_block_print();

  for ( i = 1; i < 80;  i +=   2)
  {
    ebox_free(ptx[i]);
  }
//  ebox_free_block_print();

}
int main(void)
{
  setup();
  while (1)
  {
    //uart1.print(".");
    delay_ms(1000);
  }

}


