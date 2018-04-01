/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API�й����޸�CPUƵ��,�Լ���ʱ���֡�
	*	1  ͨ���޸Ĳ�ͬ��CPUƵ�ʣ����Կ��������ٶȣ��Լ�us��ʱ��Ӱ��
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
#include "ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 cpuClock example"
#define EXAMPLE_DATE	"2017-12-31"
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

E_GPIO led(PA_5);

#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_rcc.h"

// ͨ����ͬ����Ͽ������ò�ͬ�Ĺ���Ƶ�ʣ�Ĭ�Ϲ�����48M,����SystemClock_Config����
#define	CPUCLOCK96M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK72M		LL_RCC_PLL_MUL_9 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK48M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2

#define CPUCLOCK			CPUCLOCK48M

void SystemClock_Config(void)
{
	/* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
	 Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
							 ((8MHz * 12)/ 2)                  = 48MHz             */
	LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {CPUCLOCK};
	/* Variable to store AHB and APB buses clock configuration */
	/* Settings to have HCLK set to 48MHz and APB to 48 MHz */
	LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

	/* Switch to PLL with HSI as clock source             */
	LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
}

void setup()
{
    ebox_init();
		usart.begin(115200);
		get_chip_info(&cpu);
		PrintfLogo();
    led.mode(OUTPUT_PP);
}

void testUs()
{
	uint8_t i = 255;
	while(i--){
				led.toggle();
			delay_us(1);
	}
}

void testMs()
{
	uint8_t i = 20;
	while(i--){
				led.toggle();
			delay_ms(1);
	}
}

int main(void){
		uint32_t t;
    setup();
		usart.printf("������ʱ��%d us,  %d ms \r\n",micros(),millis());
    while (1)
    {
			 t = micros();
			 testUs();
			 t = micros() - t;
			 usart.printf("testUs ��ʱ��%dus \r\n",t);
			 // ʹ��millis_seconds,���ٵ�������
			 t = millis_seconds;
			 testMs();
			 t = millis_seconds - t;
			 usart.printf("testMs ��ʱ��%dms \r\n",t);
    }
}
