/**
  ******************************************************************************
  * @file   : gpio_write.cpp
  * @author : cat_li
  * @version: V2.0
  * @brief   F0��GPIO����
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


#include "ebox.h"
#include <stdio.h>

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 GPIO example"
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


/**
 * 1  ����GPIO��������E_GPIO, ����ΪPIN_ID; ������PA_5��ʽ
 * 2	����GPIOģʽ����Ϊͨ��GPIOʹ��ʱ��������af���ܡ�����������stm32_define.h�У��ο�ö�ٱ��� PIN_MODE
 * 3  ��д����
*/

// ���嵥���˿�
E_GPIO PA5(PA_5);
E_GPIO *LED = &PA5;
// ����ͬһ��port�����ɲ������˿�, PA0,PA3��������ʹ�����ַ�ʽ
E_PORT P_PORT(E_PORTA,0x09);
//����ͬһ��port�����������˿�, PA1-PA2������ʹ�����ַ�ʽ
E_PORT P_PORTS(E_PORTA,2,1);
// ����ͬport���ɶ˿ڴ������Ϊһ��������в���;�ʺϲ�ͬport��IO��ͬһport��IO���ʹ��
// ����������� IO˳��
E_BUS P_BUS(PB_0,PB_3,PA_6,PA_7);


void setup()
{
    ebox_init();
		usart.begin(115200);
	  get_chip_info(&cpu);
		PrintfLogo();
    PA5.mode(OUTPUT_PP);
		LED->mode(OUTPUT_PP);
    P_PORT.mode(OUTPUT_PP);
    P_PORTS.mode(OUTPUT_PP);
    P_BUS.mode(OUTPUT_PP);

}

void operationPort(void){
    uint16_t r,s;
    // ��һ��������һ��
    P_PORT.resetAll();
    P_PORTS.resetAll();

    /** write��ʱ����������������ǲ�����λ��ʽ���������pin����дʱ����Ҫ����
    	* δ���Pin��Ӱ�졣�������mask����ģ���Ҫ����δ���Pin��Ӱ��
    	*/
    P_PORT.write(0x01);  //PA0 = 1
    P_PORTS.write(0x01); //PA1 = 1
    // ������һ����
    P_PORT.write(0x03);  //PA0=1
    P_PORTS.write(0x03); //PA1 = 1, PA2 = 1

    /** read��ʱ��ͬ������������ǲ�����λ��ʽ���������pin����дʱ����Ҫ����
      	* δ���Pin��Ӱ�졣�������mask����ģ���Ҫ����δ���Pin��Ӱ��
      	*/
    P_PORT.setAll();
    P_PORTS.setAll();
    r = P_PORT.read();  // r = 0x09
    s = P_PORTS.read(); // s = 0x03
    r = s|r;
    P_PORT = 0;//PA3 = 0, PA0 = 0
}

void operationBus(){
    uint16_t r;
    r = P_BUS.read();
    P_BUS[0].set(); // PB0 = 1
    r = P_BUS.read();
    P_BUS.resetAll(); // PB_0,PB_3,PA_6,PA_7 = 0
    P_BUS = 0x02; // pb3 =1
    r = P_BUS;
    P_BUS.write(0x0f); //    PB_0,PB_3,PA_6,PA_7 = 1
    P_BUS = r;
}

int main(void)
{
    setup();
		operationBus();
    operationPort();
    while (1)
    {
//        delay_ms(500);
//        PA5.toggle();
//				P_PORTS.resetAll();
//				P_PORTS.setAll();
					P_PORTS.write(0xff);
					P_PORTS.write(0x00);
    }
}




