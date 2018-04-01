 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2017/07/13
  * @brief   ebox exti example, ����stm32nucleo(072)ƽ̨��֤	
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
#define EXAMPLE_NAME	"STM32F0 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"
#define DEMO_VER			"1.0"

// ���ڣ�led
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

uint32_t xx;

// led
E_GPIO led(PA_5);
// ���ַ�ʽ����exti����
E_GPIO	PA5(PC_13);
E_EXTI userbtn(&PA5);
E_EXTI userbt1(PA_0,INPUT_PU);

/**
 *@brief    ��̬�ص�����
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
 *@brief    �����࣬�����������Ա��
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
		// �����أ��½��ؾ�����
		userbtn.attach(fallrise,FALL_RISING);
		userbtn.ENABLE(FALL_RISING);
		// �����أ��½��ص��ò�ͬ�Ļص�����
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




