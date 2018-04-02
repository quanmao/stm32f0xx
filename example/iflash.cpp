/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   iflash ����
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

// ���û�����ʼ��ַ����һ�����棬Ĭ��Ϊ1 page,��С����1*pagesize
E_Flash  Fh(0);
// ���û�����ʼ��ַ+2page��������һ�����棬3 page,��С����3*pagesize
E_Flash	 fl(2,3);

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 USART_POLLING example"
#define EXAMPLE_DATE	"2018-02-07"
#define DEMO_VER			"1.0"

E_GPIO PA5(PA_5);
E_UART usart(USART1,UART1_TX,UART1_RX);

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
	usart.printf("* flash size : %d KB \r\n",cpu.flash_size);
	usart.printf("* core       : %d\r\n",clock.core);
  usart.printf("* hclk       : %d\r\n",clock.hclk);
  usart.printf("* pclk1      : %d\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
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
	// �ӵ�ַ0����ȡ1���ֽڣ����������rec��
	Fh.read(0,(uint8_t*)rec,1);
	usart.printf("flash test; �� %d �ο����� \r\n",rec[0]);
	rec[0]++;
	// ����ȡ����rec[0]�ԼӺ�����д��iflash�У���Ϊ��������
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




