/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @brief   仅工作在主模式
		1    2017/5/30  移除E_PinBase的构造函数，使用PIN_ID
									增加超时，防止程序死掉。
									读写函数增加返回状态
		2    2017/7/24  修改超时模式,I2C2验证ok,增加读字节写字节函数
  *	注意：获取释放权限出需要优化。
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
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_I2C_H_
#define __EBOX_I2C_H_

#include "target_mcu.h"
#include "ebox_config.h"
//#include "stm32f0xx_ll_i2c.h"

#ifdef	USE_TIMEOUT
	#define TIMEOUT		1000	// 默认超时时间为1000us
#endif

// 分频，建立，保持，高电平，低电平
#define	C16M10K	 	__LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04)		//10k@16M
#define	C16M100K	__LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04)		//100k@16M
#define	C16M400K	__LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3)			//400k@16M

#define	C48M10K	 	__LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C48M100K	__LL_I2C_CONVERT_TIMINGS(0xb, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C48M400K	__LL_I2C_CONVERT_TIMINGS(0x5, 0x9, 0x3, 0x3, 0x3)			//400k@48M

#define	C8M10K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C8M100K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C8M400K	 	__LL_I2C_CONVERT_TIMINGS(0x0, 0x9, 0x3, 0x1, 0x3)			//400k@48M


class E_I2C
{

public:
	E_I2C(I2C_TypeDef *I2Cx,E_PinID scl,E_PinID sda);
	// 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
	void	begin(uint16_t speed);
	void    config();
	uint32_t    read_config();
	// 单字节读写
	E_STATE	write(uint8_t slave_address, uint8_t data,uint16_t timeout_us = TIMEOUT);
	uint8_t read(uint8_t slave_address);	
	uint8_t read(uint8_t slave_address,uint8_t reg_address);
	// 读写n字节
	E_STATE writeBuf(uint8_t slave_address,uint8_t *data,uint16_t num_to_write,uint16_t timeout_us = TIMEOUT);
	E_STATE	writeBuf(uint8_t slave_address,uint8_t reg_address,uint8_t *data, uint16_t num_to_write,uint16_t timeout_us = TIMEOUT);
	
	E_STATE	readBuf(uint8_t slave_address,uint8_t *data, uint16_t num_to_read,uint16_t timeout_us = TIMEOUT);
	E_STATE	readBuf(uint8_t slave_address, uint8_t reg_address, uint8_t *data,uint16_t num_to_read,uint16_t timeout_us = TIMEOUT);
	// 等待设备响应
	E_STATE waitAck(uint8_t s_addr,uint16_t timeout_us = 300);
	
	// 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
	E_STATE takeRight(uint32_t timing,uint16_t timeout_us = 300);
	// 释放I2C控制权
	void    releaseRight(void);

private:
	I2C_TypeDef 	*_i2cx;		// i2c外设
	uint32_t   	 	_timing;	// i2c时序
	uint8_t     	_busy;
};


/*
	1.支持任何IO引脚；
	2.函数接口和硬件I2C完全一样可以互相替代
	注意：
				1.该类的speed是由delay_us延时函数控制。略有不准
				2.speed设置只能为100000，200000,300k,400k。如果不是此值，则会将speed的值直接传递给delay_us.即delay_us(speed);
				3.初期调试I2C设备建议使用100k。或者大于10的值
*/
//class SoftI2c
//{
//public:
//    SoftI2c(Gpio *scl, Gpio *sda);
//    void 		begin(uint32_t speed);
//    int8_t 		config(uint32_t speed);
//    uint32_t	read_config();
//    int8_t		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
//    int8_t 		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write);
//    int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data);
//    int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_read);
//    int8_t		wait_dev_busy (uint8_t slave_address);
//public:
//    int8_t      take_i2c_right(uint32_t speed);
//    int8_t      release_i2c_right(void);

//private:
//    void 	start();
//    void 	stop();
//    int8_t 	send_ack();
//    int8_t 	send_no_ack();

//    int8_t 	send_byte(uint8_t Byte);
//    int8_t	send_7bits_address(uint8_t slave_address);
//    uint8_t receive_byte();

//    int8_t	wait_ack();

//private:
//    Gpio        *sda_pin;
//    Gpio        *scl_pin;
//    uint32_t	speed;
//    uint16_t	delay_times;
//    uint8_t 	busy;
//};

#endif
