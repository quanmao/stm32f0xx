/**
  ******************************************************************************
  * @file    ebox_spi.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/11/11
  * @brief   ����������ģʽ
		1  2017/5/30  �Ƴ�E_PinBase�Ĺ��캯����ʹ��PIN_ID
									���ӳ�ʱ����ֹ����������
									��д�������ӷ���״̬
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

#ifndef __EBOX_SPI_H_
#define __EBOX_SPI_H_

#include "target_mcu.h"
#include "ebox_define.h"
#include "stm32f0xx_ll_spi.h"

//spi��ع����������//////////////////////////////////
#define SPI_MODE0 0x02		//����1����
#define SPI_MODE1 0x00		//����2����
#define SPI_MODE2 0x03		//����1����
#define SPI_MODE3 0x01		//����2������ʱ�ӿ��и�

#define SPI_CLOCK_DIV2     LL_SPI_BAUDRATEPRESCALER_DIV2
#define SPI_CLOCK_DIV4     LL_SPI_BAUDRATEPRESCALER_DIV4
#define SPI_CLOCK_DIV8     LL_SPI_BAUDRATEPRESCALER_DIV8
#define SPI_CLOCK_DIV16    LL_SPI_BAUDRATEPRESCALER_DIV16
#define SPI_CLOCK_DIV32    LL_SPI_BAUDRATEPRESCALER_DIV32
#define SPI_CLOCK_DIV64    LL_SPI_BAUDRATEPRESCALER_DIV64
#define SPI_CLOCK_DIV128   LL_SPI_BAUDRATEPRESCALER_DIV128
#define SPI_CLOCK_DIV256   LL_SPI_BAUDRATEPRESCALER_DIV256

#define SPI_BITODER_MSB		LL_SPI_MSB_FIRST
#define SPI_BITODER_LSB		LL_SPI_LSB_FIRST

typedef struct
{
	uint8_t     dev_num;
	uint8_t     mode;
	uint16_t    prescaler;
	uint16_t    bit_order;
}E_SPI_CONFIG_T;

class E_SPI{
public:
	E_SPI(SPI_TypeDef *SPIx, E_PinID sck, E_PinID miso, E_PinID mosi);

	void 	 config(E_SPI_CONFIG_T *spi_config);

	uint8_t	 writeChar(uint8_t data);
	int8_t 	 writeBuf(uint8_t *data, uint16_t data_length);
	uint8_t   read();
	int8_t   read(uint8_t *recv_data);
	int8_t   read(uint8_t *recv_data, uint16_t data_length);

private:
	SPI_TypeDef *_spi;
	E_LOCK_T     _lock;
	uint8_t _index;
};

#endif
