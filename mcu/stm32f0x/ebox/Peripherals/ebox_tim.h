/**
  ******************************************************************************
  * @file    ebox_tim.h
  * @author  cat_li
  * @brief
  *		2017/7/15 cat_li
		1	E_PWM 基本完成，TIM3验证OK
  *		2017/7/23 cat_li
		1 E_TIME,PWM,CAPTRUE基本完成
		2	添加注释，修复capture偶然出现占空比为0的bug
  *		2017/9/7 
		1 E_TIME 实现对象成员绑定
	*		2017/9/10
		1	E_CAPTURE 对象成员绑定，函数绑定
	*		2017/9/26
		1	E_PWM TIM1,TIM3,TIM14,TIM16,TIM17验证ok
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
1.支持TIM2，3，4的ch1,2,3,4.共计12个通道
2.支持测量周期、频率、高级用法支持测量占空比
3.默认测量下降沿，可通过函数设置测量边沿模式
4.定时器计数器最大值为0xffff,为了突破这个限制，
    在本例程中，使用了update溢出中断调用tx_overflow_times可以将计数器
    拓展至2^32。大大提高测量范围，可以实现最高频率（1分频）测量周期低于120s的信号。
    如果使用2分频，可测量周期低于240s的信号。以此类推。
5.关于分频系数和脉冲宽度测量的计算关系，要遵循一个原则：在不溢出的情况下尽量使用低分频系数（高TIM时钟）去检测对象
6.关于get_capture()和测量时间结果转换的关系；
    时间(us)=get_capture()/(72/prescaler);
    时间(ms)=get_capture()/(72000/prescaler);
    时间(s)=get_capture()/(72000000/prescaler);
    如果直接使用get_zone_time_us()方法则可直接得到一个计算好的值。可以省去手工计算的过程。
    此处提供了两种获取边沿宽度的方法，一种是按定时器脉冲数，一种是按时间单位注意其区别。


7.如果使用某个定时器通道用于输入捕获，则该定时器所有通道都必须是输入捕获模式，不能再设置为其他工作模式
重点：
    在采用低分频系数的时候，可以保证测量精度，但是会增大定时器溢出频率，进而增大cpu开销，
    在采用高分频系数的时候，测量精度较低，但是会降低定时器溢出频率，进而降低cpu开销，
    stm32在72M主频下，最高可测160Khz的信号。如果再大，将无法测量。
*/

#include "target_mcu.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"

// 函数指针,指向LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) 函数
typedef void (*pfun)(TIM_TypeDef *,uint32_t);
typedef uint32_t (*pGetFun)(TIM_TypeDef *);
typedef void (*pCCIT)(TIM_TypeDef *);

// 中断绑定函数指针
typedef void (*tim_irq_handler)(uint32_t id);


class E_base{
public:
	E_base(TIM_TypeDef *TIMx);
protected:	
	TIM_TypeDef  *_timx;		// TIM外设
	uint8_t		 _tIndex;		// TIM索引
	uint32_t 	 _period;		// 周期 0-65535
	uint32_t	 _prescaler;	// TIM预分频值,赋值范围1-65536,程序最终会转为0-65535
	
	void _calculate(uint32_t frq);
	void _setCountMode(uint32_t CounterMode);
	void _enableClock();
	void _start();
	void _setPerPsc();
	void _setMode(void);
	void _EnInterrupt();		// 使能中断
};

class E_TIME:E_base{
public:
	E_TIME(TIM_TypeDef *TIMx):E_base(TIMx){};
	// 设置time周期,us
	void setUs(uint32_t us);
	void setMs(uint32_t ms);
	// 设置time频率,hz
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
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
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
	uint16_t _duty;		// 占空比
	uint8_t	 _accuracy; // 精度

	pfun  _OCsetCompare;

	void _setMode(void);
};


extern uint16_t t1_overflow_times ;
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;

/**
  *	1 在不做溢出处理的情况下:最低捕获频率 FRQmin = TIM_CLK/((ARR+1)*(PSC+1)) = 48000000/(65535+1)*1 = 732.42hz
  *	2 超过138k,因为频繁进入中断，导致cpu卡死，无法处理其他任务
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


	//波形的基本的测量工具
	void        complex_event();//适用于要求测量占空比的情况，但是最短脉宽不能低于4us****
	void        simple_event();//适用于所有情况，执行效率高，最高可测180K,但是不能测量占空比
	float       get_wave_frq();///<波形的频率
	float       get_wave_peroid();///<波形的周期
	float       get_wave_high_duty();///<波形的高电平占空比
	float       get_wave_low_duty();///<波形的低电平占空比
	float       get_wave_high_time();///<波形的高电平时间
	float       get_wave_low_time();///<波形的低电平时间
	bool        available();///<波形的测量完成

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
	uint32_t 		_channel;	// 通道
	uint16_t 		_duty;		// 占空比
	uint8_t	 		_accuracy; 	// 精度
	__IO uint8_t	_porlicy;	// 极性

	uint16_t   		*_overflow_times;
	__IO uint32_t	_last_value;	//最后值
	__IO uint32_t   _capture;		//捕获值
	__IO bool	   	_available;		//是否有效
	__IO uint32_t   _high_capture;	//高电平捕获
	__IO uint32_t   _low_capture;	//低电平捕获

	uint32_t   _timeClock;			//time时钟

	pGetFun  _ICgetCompare;			//捕获函数
	pCCIT	 _CCEnableIT;			//使能捕获中断

	void _setMode(void);

	FunctionPointer _pirq;
	static void _irq_handler(uint32_t id);
};

#endif
