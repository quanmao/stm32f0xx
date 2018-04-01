/**
  ******************************************************************************
  * @file    ebox_tim.h
  * @author  cat_li
  * @brief
  *		2017/7/15 cat_li
		1	E_PWM ������ɣ�TIM3��֤OK
  *		2017/7/23 cat_li
		1 E_TIME,PWM,CAPTRUE�������
		2	���ע�ͣ��޸�captureżȻ����ռ�ձ�Ϊ0��bug
  *		2017/9/7 
		1 E_TIME ʵ�ֶ����Ա��
	*		2017/9/10
		1	E_CAPTURE �����Ա�󶨣�������
	*		2017/9/26
		1	E_PWM TIM1,TIM3,TIM14,TIM16,TIM17��֤ok
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
#ifndef	__EBOX_TIM_H_
#define	__EBOX_TIM_H_

/*
1.֧��TIM2��3��4��ch1,2,3,4.����12��ͨ��
2.֧�ֲ������ڡ�Ƶ�ʡ��߼��÷�֧�ֲ���ռ�ձ�
3.Ĭ�ϲ����½��أ���ͨ���������ò�������ģʽ
4.��ʱ�����������ֵΪ0xffff,Ϊ��ͻ��������ƣ�
    �ڱ������У�ʹ����update����жϵ���tx_overflow_times���Խ�������
    ��չ��2^32�������߲�����Χ������ʵ�����Ƶ�ʣ�1��Ƶ���������ڵ���120s���źš�
    ���ʹ��2��Ƶ���ɲ������ڵ���240s���źš��Դ����ơ�
5.���ڷ�Ƶϵ���������Ȳ����ļ����ϵ��Ҫ��ѭһ��ԭ���ڲ����������¾���ʹ�õͷ�Ƶϵ������TIMʱ�ӣ�ȥ������
6.����get_capture()�Ͳ���ʱ����ת���Ĺ�ϵ��
    ʱ��(us)=get_capture()/(72/prescaler);
    ʱ��(ms)=get_capture()/(72000/prescaler);
    ʱ��(s)=get_capture()/(72000000/prescaler);
    ���ֱ��ʹ��get_zone_time_us()�������ֱ�ӵõ�һ������õ�ֵ������ʡȥ�ֹ�����Ĺ��̡�
    �˴��ṩ�����ֻ�ȡ���ؿ�ȵķ�����һ���ǰ���ʱ����������һ���ǰ�ʱ�䵥λע��������


7.���ʹ��ĳ����ʱ��ͨ���������벶����ö�ʱ������ͨ�������������벶��ģʽ������������Ϊ��������ģʽ
�ص㣺
    �ڲ��õͷ�Ƶϵ����ʱ�򣬿��Ա�֤�������ȣ����ǻ�����ʱ�����Ƶ�ʣ���������cpu������
    �ڲ��ø߷�Ƶϵ����ʱ�򣬲������Ƚϵͣ����ǻή�Ͷ�ʱ�����Ƶ�ʣ���������cpu������
    stm32��72M��Ƶ�£���߿ɲ�160Khz���źš�����ٴ󣬽��޷�������
*/

#include "target_mcu.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"

// ����ָ��,ָ��LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) ����
typedef void (*pfun)(TIM_TypeDef *,uint32_t);
typedef uint32_t (*pGetFun)(TIM_TypeDef *);
typedef void (*pCCIT)(TIM_TypeDef *);

// �жϰ󶨺���ָ��
typedef void (*tim_irq_handler)(uint32_t id);


class E_base{
public:
	E_base(TIM_TypeDef *TIMx);
protected:	
	TIM_TypeDef  *_timx;		// TIM����
	uint8_t		 _tIndex;		// TIM����
	uint32_t 	 _period;		// ���� 0-65535
	uint32_t	 _prescaler;	// TIMԤ��Ƶֵ,��ֵ��Χ1-65536,�������ջ�תΪ0-65535
	
	void _calculate(uint32_t frq);
	void _setCountMode(uint32_t CounterMode);
	void _enableClock();
	void _start();
	void _setPerPsc();
	void _setMode(void);
	void _EnInterrupt();		// ʹ���ж�
};

class E_TIME:E_base{
public:
	E_TIME(TIM_TypeDef *TIMx):E_base(TIMx){};
	// ����time����,us
	void setUs(uint32_t us);
	void setMs(uint32_t ms);
	// ����timeƵ��,hz
	void setFrequency(uint32_t frq);
	void start();
	void stop();
	
	uint32_t GetMaxFrequency(void);
		
	void attach(void (*fptr)(void)){
		_pirq.attach(fptr);
	}
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void)){
			_pirq.attach(tptr, mptr);
	}
private:
	FunctionPointer _pirq;
	void _setMode(void);
	static void _irq_handler(uint32_t id);
};
/**
 *@brief    ����Pin_id��ȡ��Ӧ��������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(E_PinID pin_id,uint32_t periph,const AF_FUN_S *emap)
{
	uint8_t i = 0;
	while (!((0xffffff00 & (emap+i)->_periph_OR_ch) == periph) || !((emap+i)->_pin_id == pin_id))
	{
		if ((emap+i)->_pin_id == P_NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}
class E_PWM:E_base{
public:
	E_PWM(TIM_TypeDef *TIMx,E_PinID id):E_base(TIMx){
		uint8_t _index;
		uint32_t t = (uint32_t)TIMx;
		E_PinBase *_pin;
		_pin = new E_PinBase(id);
		_index = getIndex(id,t,TIM_MAP);
		_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
		_timx = TIMx;
		
		t = (TIM_MAP[_index]._periph_OR_ch) - (uint32_t)_timx;
		switch (t)
		{
		case TIMxCH1:
			_channel = LL_TIM_CHANNEL_CH1;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH1;
			break;
		case TIMxCH2:
			_channel = LL_TIM_CHANNEL_CH2;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH2;
			break;
		case TIMxCH3:
			_channel = LL_TIM_CHANNEL_CH3;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH3;
			break;
		case TIMxCH4:
			_channel = LL_TIM_CHANNEL_CH4;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH4;
			break;
		}
	}
	void begin(uint32_t frq,uint16_t duty);

	void SetPorlicy(uint8_t porlicy);
	void SetFrequency(uint32_t frq);
	void SetDutyCycle(uint16_t duty);

	uint32_t GetMaxFrequency(void);
private:
	uint32_t _channel;
	uint16_t _duty;		// ռ�ձ�
	uint8_t	 _accuracy; // ����

	pfun  _OCsetCompare;

	void _setMode(void);
};


extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;

/**
  *	1 �ڲ����������������:��Ͳ���Ƶ�� FRQmin = TIM_CLK/((ARR+1)*(PSC+1)) = 48000000/(65535+1)*1 = 732.42hz
  *	2 ����138k,��ΪƵ�������жϣ�����cpu�������޷�������������
  */

class E_CAPTURE:E_base{
public:
	E_CAPTURE(TIM_TypeDef *TIMx,E_PinID id):E_base(TIMx){
		uint8_t _index;
		uint32_t t = (uint32_t)TIMx;
		E_PinBase *_pin;
		_pin = new E_PinBase(id);
		_index = getIndex(id,t,TIM_MAP);
		_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
		_timx = TIMx;

		_overflow_times = &t1_overflow_times;
		_last_value 		= 0;

		t = (TIM_MAP[_index]._periph_OR_ch) - (uint32_t)_timx;
		switch (t)
		{
		case TIMxCH1:
			_channel = LL_TIM_CHANNEL_CH1;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH1;
			_CCEnableIT = &LL_TIM_EnableIT_CC1;
			break;
		case TIMxCH2:
			_channel = LL_TIM_CHANNEL_CH2;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH2;
			_CCEnableIT = &LL_TIM_EnableIT_CC2;
			break;
		case TIMxCH3:
			_channel = LL_TIM_CHANNEL_CH3;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH3;
			_CCEnableIT = &LL_TIM_EnableIT_CC3;
			break;
		case TIMxCH4:
			_channel = LL_TIM_CHANNEL_CH4;
			_ICgetCompare = &LL_TIM_IC_GetCaptureCH4;
			_CCEnableIT = &LL_TIM_EnableIT_CC4;
			break;
		}
	}



	void SetPorlicy(uint8_t porlicy);
	void begin();

	void        set_count(uint16_t count);
	void        set_polarity_falling();
	void        set_polarity_rising();


	//���εĻ����Ĳ�������
	void        complex_event();//������Ҫ�����ռ�ձȵ������������������ܵ���4us****
	void        simple_event();//���������������ִ��Ч�ʸߣ���߿ɲ�180K,���ǲ��ܲ���ռ�ձ�
	float       get_wave_frq();///<���ε�Ƶ��
	float       get_wave_peroid();///<���ε�����
	float       get_wave_high_duty();///<���εĸߵ�ƽռ�ձ�
	float       get_wave_low_duty();///<���εĵ͵�ƽռ�ձ�
	float       get_wave_high_time();///<���εĸߵ�ƽʱ��
	float       get_wave_low_time();///<���εĵ͵�ƽʱ��
	bool        available();///<���εĲ������

	uint32_t GetDetectMaxFrq(void);
	uint32_t GetDetectMinFrq(void);

	void attach(void (*fptr)(void)){
		_pirq.attach(fptr);
	}
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void)){
		_pirq.attach(tptr, mptr);
	}

private:
	uint32_t 		_channel;	// ͨ��
	uint16_t 		_duty;		// ռ�ձ�
	uint8_t	 		_accuracy; 	// ����
	__IO uint8_t	_porlicy;	// ����

	uint16_t   		*_overflow_times;
	__IO uint32_t	_last_value;	//���ֵ
	__IO uint32_t   _capture;		//����ֵ
	__IO bool	   	_available;		//�Ƿ���Ч
	__IO uint32_t   _high_capture;	//�ߵ�ƽ����
	__IO uint32_t   _low_capture;	//�͵�ƽ����

	uint32_t   _timeClock;			//timeʱ��

	pGetFun  _ICgetCompare;			//������
	pCCIT	 _CCEnableIT;			//ʹ�ܲ����ж�

	void _setMode(void);

	FunctionPointer _pirq;
	static void _irq_handler(uint32_t id);
};

#endif
