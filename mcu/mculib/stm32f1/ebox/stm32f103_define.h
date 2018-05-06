/******************************************************************************
程 序 名： stm32f103_define.h　
编 写 人： cat_li
编写时间： 2018年5月6日
接口说明： stm32f103外设定义(tssop20).
修改日志：　　
	NO.1-  初始版本
*******************************************************************************/

#ifndef __STM32F103_DEFINE_H_
#define __STM32F103_DEFINE_H_

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_pwr.h"

#include "stm32_define.h"

#define I2C1_SCL	PA_9
#define	I2C1_SDA	PA_10

#define SPI_SCK		PA_5
#define SPI_MISO	PA_6
#define SPI_MOSI	PA_7

#define TIM3CH1		TIM3,PA_6
#define	TIM3CH2		TIM3,PA_7
#define TIM3CH4		TIM3,PB_1
#define	TIM14CH1	TIM14,PA_4
#define	TIM16CH1	TIM16,PA_6
#define	TIM17CH1	TIM17,PA_7
#define TIM1CH2		TIM1,PA_9
#define TIM1CH3		TIM1,PA_10

#define STM32_SRAM_SIZE					16		//内存大小，ebox_mem调用

#include "stm32f1xx_ll_adc.h"

static const AF_FUN_S ADC_MAP[] = {
//	PA_0,AIN,0,LL_ADC_CHANNEL_0,
//	PA_1,AIN,0,LL_ADC_CHANNEL_1,
//	PA_2,AIN,0,LL_ADC_CHANNEL_2,
//	PA_3,AIN,0,LL_ADC_CHANNEL_3,
//	PA_4,AIN,0,LL_ADC_CHANNEL_4,
//	PA_5,AIN,0,LL_ADC_CHANNEL_5,
//	PA_6,AIN,0,LL_ADC_CHANNEL_6,
//	PA_7,AIN,0,LL_ADC_CHANNEL_7,

//	PB_1,AIN,0,LL_ADC_CHANNEL_9,

	P_NC
};

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

static const AF_FUN_S TIM_MAP[] ={
	// TIM3,CH1,CH2,CH4
//	PA_6,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH1,
//	PA_7,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH2,
//	PB_1,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH4,
//	// TIM16,CH1
//	PA_6,AF_PP,LL_GPIO_AF_5,TIM16_BASE+TIMxCH1,
//	// TIM17,CH1
//	PA_7,AF_PP,LL_GPIO_AF_5,TIM17_BASE+TIMxCH1,
//	// TIM14,CH1
//	PA_4,AF_PP,LL_GPIO_AF_4,TIM14_BASE+TIMxCH1,
//	// TIM1,CH2,CH3
//	PA_9,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH2,
//	PA_10,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH3,
	P_NC
};

// 不同tim irq索引
#define TIM1IQR	Irq1
#define TIM3IQR	Irq2
#define TIM14IQR	Irq3
#define TIM16IQR	Irq4
#define TIM17IQR	Irq5

static const Periph_SS TIM_INFO[]={
//	TIM1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM1,TIM1_BRK_UP_TRG_COM_IRQn,TIM1IQR,
////	TIM1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM1,TIM1_CC_IRQn,TIM1IQR,
//	TIM3_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_TIM3,TIM3_IRQn,TIM3IQR,
//	TIM14_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_TIM14,TIM14_IRQn,TIM14IQR,
//	TIM16_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM16,TIM16_IRQn,TIM16IQR,
//	TIM17_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM17,TIM17_IRQn,TIM17IQR,
	NC
};

static const AF_FUN_S SPI_MAP[] = {
//	PA_5,AF_PP_PU,0,SPI1_BASE,
//	PA_6,AF_PP_PU,0,SPI1_BASE,
//	PA_7,AF_PP_PU,0,SPI1_BASE,
	P_NC
};
		
static const Periph_S SPI_INFO[] = {
//	SPI1_BASE,LL_APB1_GRP2_PERIPH_SPI1,USART1_IRQn,Irq1,
	NC
};

// PA2,9,14 tx; PA3,10 rx
static const AF_FUN_S UART_MAP[] ={
//	PA_2,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	PA_3,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	PA_9,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	PA_10,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	PA_14,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
	P_NC
};

static const Periph_SS UART_INFO[] = {
//	USART1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_USART1,USART1_IRQn,Irq1,
	NC
};

// PA9 scl,PA10 SDA	
static const AF_FUN_S I2C_MAP[]={
	// i2c1
//	PA_9,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
//	PA_10,AF_PP_PU,LL_GPIO_AF_4,I2C1_BASE,
	P_NC
};

static const Periph_S I2C_INFO[] ={
//	I2C1_BASE,LL_APB1_GRP1_PERIPH_I2C1,I2C1_IRQn,Irq1,
	NC
};

#endif
