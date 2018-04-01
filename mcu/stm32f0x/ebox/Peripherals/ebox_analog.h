/**
  ******************************************************************************
  * @file    ebox_analog.h
  * @author  cat_li
  * @brief   
	*		2017/7/31	 LQM
			1	��ͨ��ADCת��ģʽ
	*		2017/8/2		 LQM
			1	��ͨ����������
			2	��ͨ�����β���,��������
			2017/8/28
			1	��ͨ��ģʽ��ͨ��ָ��������ȣ���������������ָ����������
	* ��֪bug
			1 �ڲ��ο���ѹ��һ�ζ�ȡ��ȷ��֮��ÿ�ζ�ȡ��Ҫ���800ms�����ܱ�֤�ο���ѹ��ȷ
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

#ifndef __EBOX_ANALOG_H_
#define __EBOX_ANALOG_H_

#include "target_mcu.h"
#include "ebox_config.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"

#ifdef	USE_TIMEOUT
	#define TIMEOUT		1000	// Ĭ�ϳ�ʱʱ��Ϊ1000us
#endif




extern void ADC1_init(void);
extern int get_channel(E_PinID pin);
extern uint16_t analogin_read(uint32_t *channel);
extern uint16_t analogin_read_voltage(uint16_t *channel);
extern void DMA_configuration(void);
extern uint16_t analog_read_temperature(void);

typedef enum 
{
	temperature_ch = 0, 
#if defined(ADC_CCR_VBATEN)
	battery_ch
#endif
}ADC_CH;

class E_ADC{
public:
	E_ADC(E_PinID id,uint16_t refVoltage = 3300);
	E_ADC(ADC_CH ch);
	
	void setRefVoltage(uint16_t refVoltage);

	uint16_t read();
	uint16_t getVoltage();
	float 	 getTemperature();
private:
	uint32_t _channel;		// adcͨ��	
	void _setInterChannel(void);
};


class E_AdcDMA{
public:
	E_AdcDMA(E_PinID id);
	E_AdcDMA(uint8_t chNum,uint8_t deep = 0);
	E_AdcDMA(uint32_t channel,uint8_t chNum,uint8_t deep = 0);

	void addChannel(E_PinID id);

	void dmaConfig();
	void dmaStart();
	void update();

	uint16_t *r_buffer;		// ת�����
private:
	uint32_t _channel;
	uint8_t  _deep;
	static uint8_t  _channelNum;
};

#endif
