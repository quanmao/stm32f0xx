/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
  * @brief   ebox application example .
	*		2017/9/7 	���Ӷ����Ա��
	*		2017/9/10	���Ӻ�����
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
#define EXAMPLE_NAME	"STM32F0 TIMER example"
#define EXAMPLE_DATE	"2017-07-20"
#define DEMO_VER			"1.0"

// ���ڣ�led
//E_UART usart(USART1,PA_9,PA_10);
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

E_TIME time(TIM17);

class ld{
	public:
		void tet(void){
		led.toggle();
	}	
};

ld test;

void tset(){
	led.toggle();
}

void setup()
{
    ebox_init();
	led.mode(OUTPUT_PP);
    usart.begin(115200);
	get_chip_info(&cpu);
	PrintfLogo();
		
	//time.setFrequency(10);
	time.setUs(500000);
	//time.attach(&test,&ld::tet);
	time.attach(&tset);
	time.start();
}

int main(void)
{
	setup();
	while (1)
	{
//				time.setFrequency(frq++);
//				delay_ms(1000);
	}

}




