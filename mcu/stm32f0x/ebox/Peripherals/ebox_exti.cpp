/**
  ******************************************************************************
  * @file    ebox_exti.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/26
  * @brief
		 1	2017/7/13	cat_li	�޸��ļ���Ϊebox_exti
		 2	2018/1/22	cat_li	��ʹ��extiʱ���Ƴ����ļ������Լ���200byte code�ռ�ռ��
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
#include "ebox_exti.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"

// pin.id Ϊ8bit����4λ��ʶport 0-5,��4λΪio,0-15
#define GETEXTIPORT(a) (uint32_t)a>>4
#define GETEXTILINE(a) a&0x0f

const uint32_t LINE_MAP[] ={
	LL_EXTI_LINE_0,	LL_EXTI_LINE_1,	LL_EXTI_LINE_2,	LL_EXTI_LINE_3,	LL_EXTI_LINE_4,	LL_EXTI_LINE_5,
	LL_EXTI_LINE_6,	LL_EXTI_LINE_7,	LL_EXTI_LINE_8,	LL_EXTI_LINE_9,	LL_EXTI_LINE_10,LL_EXTI_LINE_11,
	LL_EXTI_LINE_12,LL_EXTI_LINE_13,LL_EXTI_LINE_14,LL_EXTI_LINE_15
};

const uint32_t EXTISorce_MAP[]={
	LL_SYSCFG_EXTI_LINE0,LL_SYSCFG_EXTI_LINE1,LL_SYSCFG_EXTI_LINE2,LL_SYSCFG_EXTI_LINE3,
	LL_SYSCFG_EXTI_LINE4,LL_SYSCFG_EXTI_LINE5,LL_SYSCFG_EXTI_LINE6,LL_SYSCFG_EXTI_LINE7,
	LL_SYSCFG_EXTI_LINE8,LL_SYSCFG_EXTI_LINE9,LL_SYSCFG_EXTI_LINE10,LL_SYSCFG_EXTI_LINE11,
	LL_SYSCFG_EXTI_LINE12,LL_SYSCFG_EXTI_LINE13,LL_SYSCFG_EXTI_LINE14,LL_SYSCFG_EXTI_LINE15
};

// uint32_t pObj ����� ,uint8_t line �ⲿ�ж���
typedef void (*exti_irq_handler)(uint32_t pObj,uint8_t line);

static exti_irq_handler  exti_handler;   // ��������ָ�������ָ����ľ�̬��Ա
static uint32_t  exti_irq_ids[16];    	 // ��������ַ������̬��Աʶ����󣬲����ʶ������ͨ��Ա

void exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
	exti_irq_ids[index] = id;      // ��������ַ
	exti_handler =  handler;        // ָ��ص�����
}

void E_EXTI::_init(E_PinMode mode,ExtiType type){
	_pin->mode(mode);
	_line = GETEXTILINE(_pin->_id);
	exti_irq_init(GETEXTILINE(_pin->_id),(&E_EXTI::_irq_handler),(uint32_t)this);
	switch (type)
	{
	case IT:
		LL_EXTI_EnableIT_0_31(LINE_MAP[_line]);
		LL_EXTI_DisableEvent_0_31(LINE_MAP[_line]);
		break;
	case EVENT:
		LL_EXTI_EnableEvent_0_31(LINE_MAP[_line]);
		LL_EXTI_DisableIT_0_31(LINE_MAP[_line]);
		break;
	case IT_EVENT:
		LL_EXTI_EnableIT_0_31(LINE_MAP[_line]);
		LL_EXTI_EnableEvent_0_31(LINE_MAP[_line]);
		break;
	default:
		break;
	}
}

E_EXTI::E_EXTI(E_GPIO *pin,E_PinMode mode,ExtiType type){
	_pin = pin;
	_init(mode,type);
}

E_EXTI::E_EXTI(E_PinID id,E_PinMode mode,ExtiType type){
	_pin = new E_GPIO(id);
	_init(mode,type);
}

/**
 *@brief    ʹ���ж�
 *@param    NONE
 *@retval   NONE
*/
void E_EXTI::ENABLE(TrigType type,uint32_t priority){
	switch (type)
	{
	case FALLING:
		LL_EXTI_EnableFallingTrig_0_31(LINE_MAP[_line]);
		LL_EXTI_DisableRisingTrig_0_31(LINE_MAP[_line]);
		break;
	case RISING:
		LL_EXTI_EnableRisingTrig_0_31(LINE_MAP[_line]);
		LL_EXTI_DisableFallingTrig_0_31(LINE_MAP[_line]);
		break;
	case FALL_RISING:
		LL_EXTI_EnableFallingTrig_0_31(LINE_MAP[_line]);
		LL_EXTI_EnableRisingTrig_0_31(LINE_MAP[_line]);
		break;
	default:
		break;
	}

	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
	LL_SYSCFG_SetEXTISource(GETEXTIPORT(_pin->_id), EXTISorce_MAP[_line]);

	switch (_line)
	{
	case 0:
	case 1:
		NVIC_EnableIRQ(EXTI0_1_IRQn);
		NVIC_SetPriority(EXTI0_1_IRQn,priority);
		break;
	case 2:
	case 3:
		NVIC_EnableIRQ(EXTI2_3_IRQn);
		NVIC_SetPriority(EXTI2_3_IRQn,priority);
		break;
	default:
		NVIC_EnableIRQ(EXTI4_15_IRQn);
		NVIC_SetPriority(EXTI4_15_IRQn,priority);
		break;
	}
}

void E_EXTI::DISABLE(TrigType type){
	switch (type)
	{
	case FALLING:
		LL_EXTI_DisableFallingTrig_0_31(LINE_MAP[_line]);
		break;
	case RISING:
		LL_EXTI_DisableRisingTrig_0_31(LINE_MAP[_line]);
		break;
	case FALL_RISING:
		LL_EXTI_DisableRisingTrig_0_31(LINE_MAP[_line]);
		LL_EXTI_DisableFallingTrig_0_31(LINE_MAP[_line]);
		break;
	default:
		break;
	}
}

/**
 *@brief    EXTI ��̬��Ա���������ж��е��ã�����ִ����ػص�����
 *@param    uint32_t id �����ַ������ʶ�����IrqType irq_type �ж�����
 *@retval   NONE
*/
void E_EXTI::_irq_handler(uint32_t pObj,uint8_t line)
{
	E_EXTI *handler = (E_EXTI*)pObj;  // ָ��ص�������ַ
	handler->_pirq[handler->_pin->read()].call();

}

/**
 *@brief    exti ���ж�
 *@param    TrigType type �жϴ�������,FALLING,RISING,FALL_RISING; void (*fptr)(void) �ص�����ָ��
 *@retval   NONE
*/
void E_EXTI::attach(void (*fptr)(void),TrigType type){
	if (type == FALL_RISING){
		_pirq[FALLING].attach(fptr);
		_pirq[RISING].attach(fptr);
	}else{
		_pirq[type].attach(fptr);
	}
}

extern "C" {
	void EXTI4_15_IRQHandler(void)
	{
		for (uint8_t i = 4;i<=15;i++)
		{
			if (LL_EXTI_IsActiveFlag_0_31(LINE_MAP[i]) != RESET)
			{
				LL_EXTI_ClearFlag_0_31(LINE_MAP[i]);
				exti_handler(exti_irq_ids[i],i);
			}
		}
	}

	void EXTI0_1_IRQHandler(void){
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
			exti_handler(exti_irq_ids[0],0);
		}else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
			exti_handler(exti_irq_ids[1],1);
		}
	}

	void EXTI2_3_IRQHandler(void){
		if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
			exti_handler(exti_irq_ids[2],2);
		}else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
		{
			LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
			exti_handler(exti_irq_ids[3],3);
		}
	}
}
