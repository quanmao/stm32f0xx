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

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 I2C Search Add example"
#define EXAMPLE_DATE	"2017-07-24"
#define DEMO_VER			"1.0"

// ���ڣ�led
E_UART usart(USART1,UART1_TX,UART1_RX);
E_GPIO led(PA_5);

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
	usart.printf("* flash size : %d KB \r\n",cpu.chip_id[0],cpu.flash_size);
	usart.printf("* core       : %d Hz\r\n",clock.core);
  usart.printf("* hclk       : %d Hz\r\n",clock.hclk);
  usart.printf("* pclk1      : %d Hz\r\n",clock.pclk1);
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
	usart.printf("*************************************************************\n\r");
}

//#define IC1  I2C1,PB_8,PB_9
#define IC1  I2C1,I2C1_SCL,I2C1_SDA
//#define	IC21314	I2C2,PB_13,PB_14
E_I2C i2c(IC1);

void setup()
{
	ebox_init();
	usart.begin(115200);
	get_chip_info(&cpu);
	PrintfLogo();
	i2c.begin(100);
}

int main(void)
{
	uint8_t add = 1;
	setup();
	
	
	i2c.write(0x4e,0x00);
	delay_ms(6000);

	usart.printf("I2C address scanner, Scaning.......\r\n");
	while (1)
	{
		// 7λ��ַ,0-127
		for (;add<=127;add++)
		{
			usart.printf("test address is 0x%2x  ",add);
			if (i2c.waitAck(add,300)==E_OK)
			{
				usart.printf("success! \r\n");
				i2c.write(add,0x08);
				delay_ms(600);
				//break;				
			} else{
				usart.printf("fail! \r\n");
			}
		}
		if (add>127)
		{
			usart.printf("�������,��ȷ���豸ok! \r\n");
		}
		delay_ms(6000);
		i2c.write(0x4f,0x00);
		add = 1;
	}

}




