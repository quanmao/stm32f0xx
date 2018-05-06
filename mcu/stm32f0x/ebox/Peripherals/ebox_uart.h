/**
  ******************************************************************************
  * @file    ebox_uart.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
	1  2017/5/25  移除E_PinBase的构造函数，使用PIN_ID
	2  2017/5/30	完善uart接口
	3	 2017/7/12	添加printf输出
		已知问题，没有进行忙检测
	4	 2017/9/17  移除本模块内printf实现，改为继承的方式
	5	 2018/1/07	修复接收数据时发送数据导致锁死
	6	 2018/2/07	禁止串口溢出检测功能，防止程序卡死。
								添加含超时检测的read接口
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

//#include "stm32f0xx_ll_usart.h"		移动到芯片定义文件
#include "ebox_define.h"
#include "ebox_gpio.h"
#include "ebox_Template.h"
#include "ebox_config.h"

#define UART_NUM (2)

// 中断类型
enum IrqType {
	RxIrq = 0,	// 接收完成后触发
	TxIrq		// 发送完成后触发
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
	 *@brief    绑定静态回调函数 attach user event
	 *@param    IrqType type 中断类型，RxIrq(接收到数据),TxIrq（数据发送完成）;
	 *@param    void (*fptr)(void) 回调函数指针
	 *@retval   NONE
	*/
	void attach(void (*fptr)(void), IrqType type);

	/**
	 *@brief    绑定成员回调函数
	 *@param    IrqType type 中断类型，RxIrq(接收到数据),TxIrq（数据发送完成）;
	 *@param    void (T::*mptr)(void) 类成员函数指针
	 *@param    T* tptr 类的实例对象
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
