/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
	1  2017/5/25  �Ƴ�E_PinBase�Ĺ��캯����ʹ��PIN_ID
	2  2017/5/30	����uart�ӿ�
	3	 2017/7/12	���printf���
		��֪���⣬û�н���æ���
	4	 2017/9/17  �Ƴ���ģ����printfʵ�֣���Ϊ�̳еķ�ʽ
	5	 2018/1/07	�޸���������ʱ�������ݵ�������
	6	 2018/2/07	��ֹ���������⹦�ܣ���ֹ��������
								��Ӻ���ʱ����read�ӿ�
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

#ifndef __EBOX_UART_H_
#define __EBOX_UART_H_

#include "stm32f0xx_ll_usart.h"
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"
#include "ebox_config.h"

#define UART_NUM (2)

// �ж�����
enum IrqType {
	RxIrq = 0,	// ������ɺ󴥷�
	TxIrq		// ������ɺ󴥷�
};

typedef void (*uart_irq_handler)(uint32_t id, IrqType type);

#if USE_PRINTF
#include "printf.h"
class E_UART:public Printf{
#else
class E_UART{
#endif
public:

	E_UART(USART_TypeDef *UARTx,E_PinID PinTx,E_PinID PinRx);

	// initial uart
	void begin(uint32_t speed,uint8_t data_bit, uint8_t parity, float stop_bit,uint8_t _use_dma);
	void begin(uint32_t speed,uint32_t data_bit=LL_USART_DATAWIDTH_8B, uint32_t parity =LL_USART_PARITY_NONE, uint32_t stop_bit =LL_USART_STOPBITS_1 ,uint8_t use_dma =1);

	void enable_irq(IrqType type);
	void disable_irq(IrqType type);

	//write method
	virtual uint8_t  write(uint8_t c);
	virtual uint8_t  write(const char *buffer, int size);

#if USE_PRINTF
	using 	Printf::write;
#endif

	//read method
	uint16_t 	read();
	E_STATE		read(uint16_t *dat, uint16_t timeOut_ms = 100);
	

	void   wait_busy();
	void   set_busy();
	/**
	 *@brief    �󶨾�̬�ص����� attach user event
	 *@param    IrqType type �ж����ͣ�RxIrq(���յ�����),TxIrq�����ݷ�����ɣ�;
	 *@param    void (*fptr)(void) �ص�����ָ��
	 *@retval   NONE
	*/
	void attach(void (*fptr)(void), IrqType type);

	/**
	 *@brief    �󶨳�Ա�ص�����
	 *@param    IrqType type �ж����ͣ�RxIrq(���յ�����),TxIrq�����ݷ�����ɣ�;
	 *@param    void (T::*mptr)(void) ���Ա����ָ��
	 *@param    T* tptr ���ʵ������
	 *@retval   NONE
	*/
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void), IrqType type) {
		if ((mptr != NULL) && (tptr != NULL)) {
			_irq[type].attach(tptr, mptr);
		}
	}

private:
	USART_TypeDef *UARTx;
	E_PinBase *Rx;
	E_PinBase *Tx;

	static void _irq_handler(uint32_t id, IrqType irq_type);
protected:
	FunctionPointer _irq[2];
};

#endif
