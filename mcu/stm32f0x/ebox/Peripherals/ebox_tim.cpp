/**
  ******************************************************************************
  * @file    ebox_tim.cpp
  * @author  cat_li
  * @brief
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
#include "ebox_tim.h"
#include "ebox_core.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rcc.h"
#include "ebox_debug.h"




#define TIM_IRQ_ID_NUM 5

static tim_irq_handler irq_handler;	   			// 声明函数指针变量，指向类的静态成员
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];	// 保存对象地址，供静态成员识别对象，并访问对象的普通成员

void tim_irq_init(uint8_t index,tim_irq_handler handler,uint32_t id)
{
	tim_irq_ids[index] = id;
	irq_handler =  handler;
}

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
__INLINE uint32_t GetClock(void){
	// 计算TIM时钟频率
	return LL_RCC_GetAPB1Prescaler() == 0 ? clock.pclk1 : (clock.pclk1*2);
}

//void E_TIMBase1::calculate(uint32_t frq){
//	uint32_t timer_clock = GetSourceClock();

//	for (prescaler = 1; prescaler <= 0xffff; prescaler++)
//	{
//		// 计数器频率 = 时钟/分频; 计数个数 = 计数器频率/需要的频率
//		period = timer_clock / (prescaler) / frq;
//		if ((0xffff >= period) && (period >= 1))
//		{
//			prescaler--;
//			break;
//		}
//	}
//}

//E_TIMBase::E_TIMBase(TIM_TypeDef *TIMx,E_PinID id){

//	_pin = new E_PinBase(id);
//	_index = getIndex(id,TIM_MAP);
//	_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
//	_timx = TIMx;
//	// 初始化通道信息
//	switch (TIM_MAP[_index]._periph_base)
//	{
//	case TIMxCH1:
//		_ch = LL_TIM_CHANNEL_CH1;
//		_OCsetCompare = &LL_TIM_OC_SetCompareCH1;
//		break;
//	case TIMxCH2:
//		_ch = LL_TIM_CHANNEL_CH2;
//		_OCsetCompare = &LL_TIM_OC_SetCompareCH2;
//		break;
//	case TIMxCH3:
//		_ch = LL_TIM_CHANNEL_CH3;
//		_OCsetCompare = &LL_TIM_OC_SetCompareCH3;
//		break;
//	case TIMxCH4:
//		_ch = LL_TIM_CHANNEL_CH4;
//		_OCsetCompare = &LL_TIM_OC_SetCompareCH4;
//		break;
//	}
//}

//void E_TIMBase::init(uint16_t period, uint16_t prescaler){
//
//	_index = getPeriphIndex1((uint32_t)_timx,TIM_INFO);
//	TIM_INFO[_index]._EnableClock(TIM_INFO[_index]._rcc);
//	// 以下代码来自pwm.cpp
//	LL_TIM_SetClockSource(_timx,LL_TIM_CLOCKSOURCE_INTERNAL);
//	LL_TIM_EnableARRPreload(_timx);
//	/* Reset value is LL_TIM_OCPOLARITY_HIGH */
//	//LL_TIM_OC_SetPolarity(TIMx, TIM_Channel, oc_polarity);
//	//LL_TIM_OC_EnablePreload(_timx, _ch);
//	// PWM end
//	LL_TIM_SetCounterMode(_timx, LL_TIM_COUNTERMODE_UP);
//	LL_TIM_SetPrescaler(_timx, prescaler);
//	LL_TIM_SetAutoReload(_timx, period);
//}

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/


/**
 *@brief    基类构造函数
 *@param    NONE
 *@retval   none
*/
E_base::E_base(TIM_TypeDef *TIMx){
	_timx = TIMx;
}

/**
 *@brief    使能TIM时钟
 *@param    NONE
 *@retval   none
*/
void E_base::_enableClock(){
	/* Enable the timer peripheral clock */
	_tIndex = getPeriphIndex1((uint32_t)_timx,TIM_INFO);
	TIM_INFO[_tIndex]._EnableClock(TIM_INFO[_tIndex]._rcc);

	//	tim_irq_init(0,(&E_base::_irq_handler),this);
	/* Set counter mode */
	/* Reset value is LL_TIM_COUNTERMODE_UP */
	//LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
}

void E_base::_EnInterrupt(){
	/* Configure the NVIC to TIMx update interrupt */
	NVIC_SetPriority(TIM_INFO[_tIndex]._irq, 0);
	NVIC_EnableIRQ(TIM_INFO[_tIndex]._irq);
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
}

/**
 *@brief    设置TIM Prescaler & Period
 *@param    NONE
 *@retval   none
*/
void E_base::_setPerPsc(void){
	/* Set the pre-scaler value */
	LL_TIM_SetPrescaler(_timx, _prescaler-1);
	/* Set the auto-reload value*/
	LL_TIM_SetAutoReload(_timx, _period);
}

/**
 *@name     E_base::calculate(uint32_t frq)
 *@brief    根据给定频率，计算period,prescaler参数
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void E_base::_calculate(uint32_t frq)
{
	uint32_t period  = 0;		// 周期
	uint32_t prescaler = 1;		// 预分频
	uint32_t ii = GetClock();

	DBG("TIM clock : %d , Timer frq : %d \r\n",GetClock(),frq);

	for (; prescaler <= 0xffff; prescaler++)
	{
		period = ii / prescaler / frq;
		if (0xffff >= period)
		{
			//DBG("calculate success! period = %d , prescaler = %d  \r\n",period,prescaler);
			break;
		}
	}
	_period = period;
	_prescaler = prescaler;
}

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
// uint32_t E_base::GetSourceClock(void){
	// return GetClock();
// }

/**
 *@brief    启动定时器
 *@param    NONE
 *@retval   NONE
*/
void E_base::_start(void){
	/* Enable counter */
	LL_TIM_EnableCounter(_timx);
	/* Force update generation 强制更新 */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    设置计数模式
 *@param    uint32_t CounterMode
 *         @arg @ref LL_TIM_COUNTERMODE_UP
 *         @arg @ref LL_TIM_COUNTERMODE_DOWN
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_UP
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_DOWN
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_UP_DOWN
 *@retval   NONE
*/
void E_base::_setCountMode(uint32_t CounterMode){
	LL_TIM_SetCounterMode(_timx, CounterMode);
}

/**
 *@brief    设定TIMx工作模式
 *@param    NONE
 *@retval   NONE
*/
void E_base::_setMode(void){
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
	/* Configure the NVIC to handle TIM2 update interrupt */
	NVIC_SetPriority(TIM_INFO[_tIndex]._irq, 0);
	NVIC_EnableIRQ(TIM_INFO[_tIndex]._irq);
}

/*********************************  E_TIME  *****************************************/
/**
 *@brief    设置定时器时间
 *@param    uint32_t us  
 *@retval   NONE
*/
void E_TIME::setUs(uint32_t us){
	uint32_t frq = 1000000/us;
	setFrequency(frq);
}

void E_TIME::setMs(uint32_t ms){
	uint32_t frq = 1000/ms;
	setFrequency(frq);
}

/**
 *@brief    设置定时器频率
 *@param    uint32_t frq  0-TIM时钟频率
 *@retval   NONE
*/
void E_TIME::setFrequency(uint32_t frq){
	if (frq >= GetClock())//控制频率，保证其有1%精度
		frq = GetClock();
	DBG("frq %d , max frq %d \r\n",frq,GetClock());
	_calculate(frq);
	_enableClock();
	_setPerPsc();
	tim_irq_init(TIM_INFO[_tIndex]._irqIndex,(&E_TIME::_irq_handler),(uint32_t)this);
}

/**
 *@brief    启动定时器
 *@param    NONE
 *@retval   NONE
*/
void E_TIME::start(void){
	//_setMode();
	_EnInterrupt();
	_start();
}

/**
 *@brief    关闭定时器
 *@param    NONE
 *@retval   NONE
*/
void E_TIME::stop(void){
	LL_TIM_DisableCounter(_timx);
	
}

void E_TIME::_setMode(void){
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
	/* Configure the NVIC to handle TIM2 update interrupt */
	NVIC_SetPriority(TIM_INFO[_tIndex]._irq, 0);
	NVIC_EnableIRQ(TIM_INFO[_tIndex]._irq);
}

/**
 *@brief    获取定时器最大频率
 *@param    NONE
 *@retval   NONE
*/
uint32_t E_TIME::GetMaxFrequency(void){
	return GetClock();
}

void E_TIME::_irq_handler(uint32_t id){
	E_TIME *hander = (E_TIME*)id;
	hander->_pirq.call();
}


/*********************************  E_PWM  ****************************************/

/**
 *@brief    设置TIM为PWM模式
 *@param    NONE
 *@retval   NONE
*/
void E_PWM::_setMode(void){
	LL_TIM_EnableARRPreload(_timx);
	/*********************************/
	/* Output waveform configuration */
	/*********************************/
	/* Set output mode */
	/* Reset value is LL_TIM_OCMODE_FROZEN */
	LL_TIM_OC_SetMode(_timx, _channel, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_EnablePreload(_timx, _channel);
	/* Enable output channel 1 */
	LL_TIM_CC_EnableChannel(_timx, _channel);
}

/**
 *@brief    启动PWM输出
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *		   duty 占空比 0-1000
 *@retval   NONE
*/
void E_PWM::begin(uint32_t frq,uint16_t duty){
	_enableClock();
	_setMode();
	SetFrequency(frq);
	SetDutyCycle(duty);	
	if (IS_TIM_BREAK_INSTANCE(_timx))
	{
		LL_TIM_EnableAllOutputs(_timx);
	}
	_start();
}

/**
 *@brief    设置频率,并同步计算更新占空比;最大频率可通过GetMaxFrequency()获得
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void E_PWM::SetFrequency(uint32_t frq)
{
	if (frq >= GetMaxFrequency())//控制频率，保证其有1%精度
		frq = GetMaxFrequency();

	_calculate(frq);

	_accuracy = ((_period >= 100) && (_period < 1000))?1:0;
	_setPerPsc();
	SetDutyCycle(_duty);
}

/**
 *@brief    设置PWM占空比.频率小于等于十分之一最大频率时，分辨率为千分之一
 *		   频率在十分之一最大频率和最大频率之间，分辨率为百分之一
 *@param    duty 0-1000 对应0%-100%
 *@retval   none
*/
void E_PWM::SetDutyCycle(uint16_t duty)
{
	uint16_t pulse = 0;
	float percent;		// 百分比
	
	_duty = duty>1000?1000:duty;
	// 百分之一精度
	if(_accuracy){
		_duty = (_duty<10 && _duty!=0)?10:duty;
	}

	percent = (_duty/1000.0);
	//DBG("DutyCycle is : %f \r\n",percent);

	pulse = (uint16_t) (( percent  * _period ));
	//DBG("pulse is : %d \r\n",pulse);

	_OCsetCompare(_timx, pulse);
		/* Force update generation 强制更新 */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    设置极性
 *@param    flag:  1 HIGH  0 LOW
 *@retval   None
*/
void E_PWM::SetPorlicy(uint8_t porlicy)
{
	LL_TIM_OC_SetPolarity(_timx,_channel,(porlicy == 1)?(LL_TIM_OCPOLARITY_HIGH):(LL_TIM_OCPOLARITY_LOW));
	/* Force update generation 强制更新 */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    获取PWM最大频率
 *@param    none
 *@retval   最大频率
*/
uint32_t E_PWM::GetMaxFrequency(void)
{
	return GetClock()/100;
}

/*********************************  E_CAPTURE  ****************************************/

/**
 *@brief    启动捕获
 *@param    none
 *@retval   最大频率
*/
void E_CAPTURE::begin(){
	_period = 0xffff;
	_prescaler = 1;
	_timeClock 	= GetClock()/_prescaler;
	_porlicy = LL_TIM_IC_POLARITY_RISING;
	
	_enableClock();
	_setPerPsc();
	//E_base::_setMode();
	_EnInterrupt();
	
	NVIC_SetPriority(TIM1_CC_IRQn, 0);
	NVIC_EnableIRQ(TIM1_CC_IRQn);
	
	tim_irq_init(TIM_INFO[_tIndex]._irqIndex,(&E_CAPTURE::_irq_handler),(uint32_t)this);
	_setMode();
	_start();
}

/**
 *@brief    获取高电平时间
 *@param    none
 *@retval   float 单位us
*/
float E_CAPTURE::get_wave_high_time()
{
    _available = false;
    if(_capture == 0)
        return  ((_high_capture )*1000000.0/(_timeClock));
    else
        return  (_capture*1000000.0/(_timeClock));
}

/**
 *@brief    获取低电平时间
 *@param    none
 *@retval   float 单位us
*/
float E_CAPTURE::get_wave_low_time()
{
    _available = false;
    if(_capture == 0)
        return  ((_low_capture)*1000000.0/(_timeClock));
    else
        return  (_capture*1000000.0/(_timeClock));
}

/**
 *@brief    获取高电平占比
 *@param    none
 *@retval   float 0-100
*/
float E_CAPTURE::get_wave_high_duty()
{
	    if(_capture == 0){
			 
       return (_high_capture*100.0/(_high_capture + _low_capture));
		}
    else
		{
			
       return 0;
		}
}

/**
 *@brief    获取低电平占比
 *@param    none
 *@retval   float 0-100
*/
float E_CAPTURE::get_wave_low_duty()
{
    _available = false;
    if(_capture == 0)
        return  (_low_capture*100.0/(_high_capture + _low_capture));
    else
        return  0;
}

/**
 *@brief    数据有效
 *@param    none
 *@retval   1 捕获结束, 0 正在捕获
*/
bool E_CAPTURE::available(){
	return _available;
}

/**
 *@brief    获取波形频率
 *@param    none
 *@retval   频率
*/
float E_CAPTURE::get_wave_frq()
{
    _available = false;
    if(_capture == 0)
        return  (_timeClock/((_high_capture + _low_capture)));
    else
        return (_timeClock*1.0/_capture);
}

/**
 *@brief    获取波形周期
 *@param    none
 *@retval   频率
*/
float E_CAPTURE::get_wave_peroid()
{
    _available = false;
    if(_capture == 0)
        return  ((_high_capture + _low_capture)*1000000.0/(_timeClock));
    else
        return  (_capture/(_timeClock/1000000.0));
}

/**
 *@brief    设置为捕获模式
 *@param    none
 *@retval   频率
*/
void E_CAPTURE::_setMode(){
   /* Input capture mode configuration */
  /************************************/
  /* Select the active input: IC1 = TI1FP1 */
  LL_TIM_IC_SetActiveInput(_timx, _channel, LL_TIM_ACTIVEINPUT_DIRECTTI);  
  /* Configure the input filter duration: no filter needed */
  LL_TIM_IC_SetFilter(_timx, _channel, LL_TIM_IC_FILTER_FDIV1);
  /* Set input prescaler: prescaler is disabled */
  LL_TIM_IC_SetPrescaler(_timx, _channel, LL_TIM_ICPSC_DIV1);
  /* Select the edge of the active transition on the TI1 channel: rising edge */
  LL_TIM_IC_SetPolarity(_timx, _channel, _porlicy);
	
  /**************************/
  /* TIM1 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1 */
  LL_TIM_EnableIT_CC2(_timx);  
  /***********************/
  /* Start input capture */
  /***********************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(_timx, _channel);  
}

/**
 *@brief    设置采样极性
 *@param    none
 *@retval   频率
*/
void E_CAPTURE::SetPorlicy(uint8_t porlicy){
	_porlicy = porlicy;
	LL_TIM_IC_SetPolarity(_timx,_channel,_period);
}

/**
 *@brief    简单捕获模式,需要在TIM中断里调用，用来计算采样波形参数
 *@param    none
 *@retval   频率
*/
void E_CAPTURE::simple_event(void){
	uint32_t    now = 0;
	now = _ICgetCompare(_timx);// + (*_overflow_times) * _period;  //get capture value
	if (now > _last_value)
		_capture = now - _last_value;
	else
		_capture = 0xffff + now - _last_value;
	//_capture = 0xffffffff + now - _last_value;

	//_capture = _capture + 1;
	_last_value = now;
	_available = true;
}

/**
 *@brief    复合捕获模式,需要在TIM中断里调用，用来计算采样波形参数
 *@param    none
 *@retval   频率
*/
void E_CAPTURE::complex_event()
{
	uint32_t    capture = 0;
	uint32_t    now = 0;

	now = _ICgetCompare( _timx ) + (*_overflow_times) * _period;  //get capture value

	if (now > _last_value)
		capture = now - _last_value;
	else if (now < _last_value)
		capture = 0xffffffff + now - _last_value;
	else{
		_available = false;
		return ;
	}
	_last_value = now;

	if (_porlicy == LL_TIM_IC_POLARITY_FALLING)//检测到下降沿，测量高电平时间完成
	{
		_high_capture = capture;
		SetPorlicy(LL_TIM_IC_POLARITY_RISING);
	} else {
		_low_capture = capture;
		SetPorlicy(LL_TIM_IC_POLARITY_FALLING);
	}
	//if ((_high_capture & _low_capture) != 0)
	if ((_high_capture!= 0) && ( _low_capture !=0))
		_available = true;
}

/**
 *@brief    静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；
 *@retval   NONE
*/
void E_CAPTURE::_irq_handler(uint32_t id){
	E_CAPTURE *hander = (E_CAPTURE*)id;
	hander->_pirq.call();
}


extern "C" {

	void TIM1_CC_IRQHandler(void)
	{
		/* Check whether CC1 interrupt is pending */
		if (LL_TIM_IsActiveFlag_CC1(TIM1) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_CC1(TIM1);
			/* TIM1 capture/compare interrupt processing(function defined in main.c) */
			irq_handler(tim_irq_ids[TIM1IQR]);
		}
		if (LL_TIM_IsActiveFlag_CC2(TIM1) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_CC2(TIM1);
			/* TIM1 capture/compare interrupt processing(function defined in main.c) */
			irq_handler(tim_irq_ids[TIM1IQR]);
		}
		
	}
#ifdef TIM2
	void TIM2_IRQHandler(void)
	{
		/* Check whether update interrupt is pending */
		if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_UPDATE(TIM2);
			irq_handler(tim_irq_ids[Irq2]);
		}
		/* TIM2 update interrupt processing */
	}
#endif
	
#ifdef TIM3
	void TIM3_IRQHandler(void)
	{
		/* Check whether update interrupt is pending */
		if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_UPDATE(TIM3);
			irq_handler(tim_irq_ids[TIM3IQR]);
		}
	}
#endif

#ifdef TIM14
	void TIM14_IRQHandler(void)
	{
		/* Check whether update interrupt is pending */
		if (LL_TIM_IsActiveFlag_UPDATE(TIM14) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_UPDATE(TIM14);
			irq_handler(tim_irq_ids[TIM14IQR]);
		}
	}
#endif
	
#ifdef TIM16
	void TIM16_IRQHandler(void)
	{
		/* Check whether update interrupt is pending */
		if (LL_TIM_IsActiveFlag_UPDATE(TIM16) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_UPDATE(TIM16);
			irq_handler(tim_irq_ids[TIM16IQR]);
		}
	}
#endif
	
#ifdef TIM17
	void TIM17_IRQHandler(void)
	{
		/* Check whether update interrupt is pending */
		if (LL_TIM_IsActiveFlag_UPDATE(TIM17) == 1)
		{
			/* Clear the update interrupt flag*/
			LL_TIM_ClearFlag_UPDATE(TIM17);
			irq_handler(tim_irq_ids[TIM17IQR]);
		}
	}
#endif
}
