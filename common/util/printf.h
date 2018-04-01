/**
  ******************************************************************************
  * @file    printf.h
  * @author  cat_li
  * @version V2.0
  * @date    2017/09/17
  * @brief	 格式化输出，子类需要实现write
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

#ifndef __PRINTF_H_
#define __PRINTF_H_

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

class Printf{
public:
	Printf(){}
	//write method
	virtual uint8_t  write(uint8_t) = 0;
	virtual uint8_t  write(const uint8_t *buffer, uint8_t size);
		
//    uint8_t write(const char *str) {
//      if (str == NULL) return 0;
//      return write((const uint8_t *)str, strlen(str));
//    }
    uint8_t write(const char *buffer, uint8_t size){
      return write((const uint8_t *)buffer, size);
    }
	
	void   printf(const char *fmt, ...);
private:
	char   *_buf;
};
#ifdef __cplusplus
}
#endif

#endif
