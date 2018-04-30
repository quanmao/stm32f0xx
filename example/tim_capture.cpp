/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ebox application example .
  *	 	2017/7/15	cat_li
		1	��ֲ��ebox stm32f0ƽ̨
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
#define EXAMPLE_NAME	"STM32F0 CAPTURE example"
#define EXAMPLE_DATE	"2017-07-22"
#define DEMO_VER			"1.0"

// ���ڣ�led
//E_UART usart(USART1,PA_9,PA_10);
E_UART usart(USART2,UART2_TX,UART2_RX);
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

E_PWM pwm1(TIM3CH1);
E_CAPTURE cap(TIM1CH2);

void setup()
{
	ebox_init();
	get_chip_info(&cpu);
	usart.begin(115200);
	PrintfLogo();
}

int main(void)
{
	uint32_t i;
	setup();

	pwm1.begin(1000,500);
	delay_ms(5000);
	
	cap.attach(&cap,&E_CAPTURE::simple_event);
	cap.begin();
	

	while (1)
	{
		for (i=800;i<=1740;){
			if (cap.available()){
				usart.printf("capture frq is                     %.2f Hz \r\n",cap.get_wave_frq());
				usart.printf("capture period is %.2f us \r\n",cap.get_wave_peroid());
//				usart.printf("high_duty = %0.2f%%\r\n", cap.get_wave_high_duty());
//				usart.printf("low duty  = %0.2f%%\r\n\r\n", cap.get_wave_low_duty());
				i+=1;
				pwm1.SetFrequency(i);
				usart.printf("\r\n");
			}
			delay_ms(5000);			
		}
		i = 730;
	}
}




