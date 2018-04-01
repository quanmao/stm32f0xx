/**
  ******************************************************************************
  * @file    ebox_core.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ���ļ�Ϊebox�����ļ�,�ṩ�����ǳ���Ҫ�Ľӿ�,���Ա���׼��c�ļ����á�
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



/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CORE_H
#define __CORE_H


#ifdef __cplusplus
extern "C" {
#endif
// ebox ���
#include "ebox_define.h"
#include "ebox_config.h"
#include "ebox_mem.h"
// Ŀ��cpu���
#include "target_mcu.h"
	

	
typedef struct
{
	uint32_t core;		// SYSCLK ϵͳʱ��
	uint32_t hclk;		// AHB����ʱ�ӣ���ϵͳʱ�ӷ�Ƶ�õ���һ�㲻��Ƶ������ϵͳʱ��
	uint32_t pclk1;		// APB1����ʱ��
}cpu_clock_t; 

typedef struct
{
//	uint32_t    ability;      //cpu calculate plus per second;
//	cpu_clock_t	clock;
	uint32_t    chip_id[3];   //оƬid
	uint16_t    flash_size;   //flash��С
}cpu_t;

extern cpu_clock_t clock;

///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint32_t millis_seconds;					//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���

// ��ʼ��
extern void ebox_init(void);                  //eboxϵͳ��ʼ��
extern void SystemClock_Config(void);       	//�ⲿʵ�ָú����������޸�ʱ��Դ�Լ�ʱ��Ƶ�ʡ�Ĭ�Ϲ�����48Mhz

// ʱ����ؽӿ�
extern uint32_t micros(void);                 //��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
uint32_t millis(void);   											//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
extern void delay_ms(uint32_t ms);            //��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ��������������1ms��
extern void delay_us(uint16_t us);            //��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ������48M��Ƶ��,��������1us��

// оƬ��Ϣ
extern void get_system_clock(void);						//��ȡCPUʱ��,���������ȫ�ֱ���clock��
extern void get_chip_info(cpu_t *cpu);				//����cpu��Ϣ,���������cpu��

// systick�жϻص�����
extern E_STATE SystickCallBackRegister(fun_noPara_t fun);                       // ���жϻص�������ʹ��Ĭ������ 1ms
extern E_STATE SystickCallBackMultipleReg(fun_noPara_t fun,uint16_t multiple); // ���жϻص������������������� = multiple * 1ms; ���65535


#ifdef __cplusplus
}
#endif
// ���²��ֻ�δ�Ż�
class eBox{
public:
	eBox(int i){};
	void sleep(void);
	void stop(void);
	void standby(void);
};


#endif
