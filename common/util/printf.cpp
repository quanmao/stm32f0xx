/**
  ******************************************************************************
  * @file    printf.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/09/17
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
#include "printf.h"
#include "ebox_mem.h"
#include "ebox_config.h"
#include <stdarg.h>
#include <stdio.h>


/* default implementation: may be overridden */
uint8_t Printf::write(const uint8_t *buffer, uint8_t size)
{
  size_t n = 0;
  while (size--) {
    if (write(*buffer++)) n++;
    else break;
  }
  return n;
}

/**
 *@name     E_UART::printf(const char *fmt, ...)
 *@brief    串口格式化输出
 *@param    fmt：请百度printf函数
 *@retval   none
*/
#if USE_EBOX_MEM
void Printf::printf(const char *fmt, ...)
{
	int  size1 = 0;
	uint8_t  size2 = BUF_BLOCK;

	if (_buf != NULL)
		ebox_free(_buf);

	va_list va_params;
	va_start(va_params, fmt);

	do{
		//分配内存
		_buf = (char *)ebox_malloc(size2);
		if (_buf == NULL)
			return ;
		//格式化到缓冲区
		size1 = vsnprintf(_buf,size2, fmt, va_params);
		//如果内存不够，重新申请更大的内存空间
		if (size1 == -1  || size1 > size2)
		{
			size2+=BUF_BLOCK;
			size1 = -1;
			ebox_free(_buf);			
		}
	}while (size1 == -1);

	va_end(va_params);
	write(_buf, size1);
}
#else
#include "stdlib.h"
void Printf::printf(const char *fmt, ...)
{
	int  size1 = 0;
	uint8_t  size2 = BUF_BLOCK;

	if (_buf != NULL)
		free(_buf);

	va_list va_params;
	va_start(va_params, fmt);

	do{
		//分配内存
		_buf = (char *)malloc(size2);
		if (_buf == NULL)
			return ;
		//格式化到缓冲区
		size1 = vsnprintf(_buf,size2, fmt, va_params);
		//如果内存不够，重新申请更大的内存空间
		if (size1 == -1  || size1 > size2)
		{
			size2+=BUF_BLOCK;
			size1 = -1;
			free(_buf);			
		}
	}while (size1 == -1);

	va_end(va_params);
	write(_buf, size1);
}
#endif
