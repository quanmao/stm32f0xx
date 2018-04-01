/**
  ******************************************************************************
  * @file    ebox_debug.h
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/18
  * @brief   初始版本,基于串口输出调试信息，串口需要在外部创建，并和此处同名，同
		时要进行初始化
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

#ifndef  __EBOX_DEBUG_H_
#define  __EBOX_DEBUG_H_
//根据不同的输出设备调用不同的输出函数，并且要保证已经初始化
#include "ebox_uart.h"
extern E_UART usart;

#if EBOX_DEBUG
#define DBG(...) usart.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

#endif
