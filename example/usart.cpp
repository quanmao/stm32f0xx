 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2017/07/07
  * @brief   uart���̣������жϰ� .
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

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 USART example"
#define EXAMPLE_DATE	"2017-07-07"
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

// �����жϻص�����
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
	// ���жϻص����������԰󶨶����Ա��Ҳ���԰󶨾�̬����
	usart.attach(&PA5,&E_GPIO::reset,TxIrq);
//	usart.attach(&rxirq,RxIrq);
	// ���ж�
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




