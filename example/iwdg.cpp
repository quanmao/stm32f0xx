/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
һ���������Ź���ʾ������
*/
#include "ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 iwdg example"
#define EXAMPLE_DATE	"2018-04-04"
#define DEMO_VER			"1.0"
E_WDG dog;
// ���ڣ�led
E_UART usart(USART1,PA_9,PA_10);
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
			usart.printf("* CPUID = %08X %08X %08X ,flash size = %d KB \n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0],cpu.flash_size);
	usart.printf("* \r\n");	/* ��ӡһ�пո� */
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
//	dog.feed();			//Ӧ���ڶ�ʱ���ж��ж�ʱι��,�������ʱι�����豸������
		usart.printf("running!\r\n");
//        delay_ms(1000);
//        delay_ms(1900);
		delay_ms(25000);
		//        delay_ms(2000);
		//        delay_ms(3000);
	}
}




