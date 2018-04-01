/******************************************************************************
�� �� ���� stm32_define.h��
�� д �ˣ� cat_li
��дʱ�䣺 2017��3��21��
���ƽ̨�� STM32F0xx, ebox
�ӿ�˵���� stm32��ض���
�޸���־������
	NO.1-  ��ʼ�汾
	NO.2-  2017/5/16 typedef �����struct��������ͳһ�޸�Ϊt��׺,�����շ�������
				   typedef �����enum�������ͽ�β����t��׺,�����շ�������
******************************************************************************/

#ifndef __STM32_DEFINE_H_
#define __STM32_DEFINE_H_

#include "stm32f0xx.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "ebox_define.h"

#define NC  0xFFFFFFFF

// ����GPIO�ĺ궨��
// See stm32f0xx_hal_gpio.h and stm32f0xx_hal_gpio_ex.h for values of MODE, PUPD and OUTPUTTyper
// MODEռ��2bit,OTYPERռ��2bit,PUPDռ��2bit
#define PIN_DATA(MODE,OTYPER,PUPD)  ((uint8_t)(((PUPD) << 4) | (OTYPER)<<2 | ((MODE) << 0)))
//#define SET_AFNUM(X,AFNUM)	(((AFNUM) << 6)| X)
#define GET_MODE(X)   	(((X) >> 0) & 0x03)
#define GET_OTYPER(X) 	(((X) >> 2) & 0x03)
#define GET_PUPD(X)   	(((X) >> 4) & 0x03)
//#define GET_AFNUM(X)  	(((X) >> 6) & 0x0F)
// GPIO mode
#define MODE_INPUT              (LL_GPIO_MODE_INPUT)
#define MODE_OUTPUT		        (LL_GPIO_MODE_OUTPUT)
#define MODE_AF                 (LL_GPIO_MODE_ALTERNATE)
#define MODE_ANALOG             (LL_GPIO_MODE_ANALOG)
// GPIO OutputTyper
#define	OTYPER_PP				(LL_GPIO_OUTPUT_PUSHPULL)
#define OTYPER_OD				(LL_GPIO_OUTPUT_OPENDRAIN)
// GPIO UP,DOWN
#define PULLNO									(LL_GPIO_PULL_NO)
#define PULLDOWN								(LL_GPIO_PULL_DOWN)
#define PULLUP									(LL_GPIO_PULL_UP)
// ��ȡPin,����ֵ0-15
#define GETPIN(A) 	 	  (uint16_t)(1<<(A&0x0f))
// ��ȡ�˿�����������0x00,0x10,0x20,0x30,0x40,0x50
#define GETPORTINDEX(A)   (A)&0xf0

#define STM_MODE_IT_RISING          (6)
#define STM_MODE_IT_FALLING         (7)
#define STM_MODE_IT_RISING_FALLING  (8)
#define STM_MODE_EVT_RISING         (9)
#define STM_MODE_EVT_FALLING        (10)
#define STM_MODE_EVT_RISING_FALLING (11)
#define STM_MODE_IT_EVT_RESET       (12)

/* structure --------------------------------------------------------------- */

/**
 * @brief  GPIO mode��ز�������
 ********************************************************************************/
typedef enum
{
	OUTPUT_PP  		= PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLNO),
	OUTPUT_PP_PD 	= PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLDOWN),
	OUTPUT_PP_PU    = PIN_DATA(MODE_OUTPUT,OTYPER_PP,PULLUP),

	OUTPUT_OD       = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLNO),
	OUTPUT_OD_PD    = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLDOWN),
	OUTPUT_OD_PU    = PIN_DATA(MODE_OUTPUT,OTYPER_OD,PULLUP),

	AIN             = PIN_DATA(MODE_ANALOG,OTYPER_PP,PULLNO),

	INPUT           = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLNO),
	INPUT_PD        = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLDOWN),
	INPUT_PU        = PIN_DATA(MODE_INPUT,OTYPER_PP,PULLUP),

	AF_OD           = PIN_DATA(MODE_AF,OTYPER_OD,PULLNO),  //10
	AF_OD_PD        = PIN_DATA(MODE_AF,OTYPER_OD,PULLDOWN),
	AF_OD_PU        = PIN_DATA(MODE_AF,OTYPER_OD,PULLUP),

	AF_PP           = PIN_DATA(MODE_AF,OTYPER_PP,PULLNO),  //11
	AF_PP_PD        = PIN_DATA(MODE_AF,OTYPER_PP,PULLDOWN),
	AF_PP_PU        = PIN_DATA(MODE_AF,OTYPER_PP,PULLUP),
}E_PinMode;///<gpio��ģʽ

/**
  * @brief  GPIOö�٣�����Pin_ID���˿ں�pin�ϲ�������ʹ�á�
  * ����һ��
 ********************************************************************************/
typedef enum{
	E_PORTA = 0x00,
	E_PORTB = 0x10,
	E_PORTC = 0x20,
	E_PORTD = 0x30,
	E_PORTE = 0x40,
	E_PORTF = 0x50
}E_Port;

typedef enum{
	PA_0 = E_PORTA|0, PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,PA_8,PA_9,PA_10,PA_11,PA_12,PA_13,PA_14,PA_15,
	PB_0 = E_PORTB|0, PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7,PB_8,PB_9,PB_10,PB_11,PB_12,PB_13,PB_14,PB_15,
	PC_0 = E_PORTC|0, PC_1,PC_2,PC_3,PC_4,PC_5,PC_6,PC_7,PC_8,PC_9,PC_10,PC_11,PC_12,PC_13,PC_14,PC_15,
	PD_0 = E_PORTD|0, PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7,PD_8,PD_9,PD_10,PD_11,PD_12,PD_13,PD_14,PD_15,
	PE_0 = E_PORTE|0, PE_1,PE_2,PE_3,PE_4,PE_5,PE_6,PE_7,PE_8,PE_9,PE_10,PE_11,PE_12,PE_13,PE_14,PE_15,
	PF_0 = E_PORTF|0, PF_1,PF_2,PF_3,PF_4,PF_5,PF_6,PF_7,PF_8,PF_9,PF_10,PF_11,PF_12,PF_13,PF_14,PF_15,
	P_NC = (int)0xff
}E_PinID;

typedef struct{
	E_PinID		_pin_id;		//pin_id
	E_PinMode	_pin_date;	//pin ������ mode��outputtyper,updown
	uint8_t		_pin_af;		//af����
	uint32_t	_periph_OR_ch;	//��������ͨ����
}AF_FUN_S;

// �ж�������Ϣ,�����жϳ�ʼ��
typedef enum{
	Irq1 = 0,
	Irq2,
	Irq3,
	Irq4,
	Irq5,
	Irq6,
}IrqIndex_t;

// ������Ϣ���������ַ��ʱ�ӣ��жϣ��ж��������ж�������Ҫ������Ŷ�Ӧ������uart1��Ӧirq1,i2c2��Ӧirq2��
typedef struct{
	uint32_t 	_periph_base;
	uint32_t 	_rcc;
	IRQn_Type	_irq;
	IrqIndex_t 	_irqIndex;
}Periph_S;

typedef struct{
	uint32_t 	_base;
	fun_onePara_t	_EnableClock;	// ָ��ʱ��ʹ�ܺ���
	uint32_t 	_rcc;				// ����ʱ��
	IRQn_Type	_irq;				// �����ж�
	IrqIndex_t 	_irqIndex;			// �ж�����
}Periph_SS;

/**
 *@brief    ����Pin_id��ȡ��Ӧ��������
 *@param    Periph �������ַ������ USART1_BASE  *emap ����������Ϣ������
 *@retval   ��Ӧ�����������е�������0xff ������Ϣ������
*/
__STATIC_INLINE uint8_t getPeriphIndex(uint32_t periph_base,const Periph_S *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->_periph_base  == periph_base ))
	{
		if ((emap+i)->_periph_base == NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}

__STATIC_INLINE uint8_t getPeriphIndex1(uint32_t periph_base,const Periph_SS *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->_base  == periph_base ))
	{
		if ((emap+i)->_base == NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}

/**
 *@brief    ����Pin_id��ȡ��Ӧ��������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(E_PinID pin_id,const AF_FUN_S *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->_pin_id  == pin_id ))
	{
		if ((emap+i)->_pin_id == P_NC){
			return (uint8_t)NC;
		}
		i++;
	}
	return i;
}


#endif
