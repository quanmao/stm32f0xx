/**
  ******************************************************************************
  * @file    ebox_core.c
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
  *     2017��3��4��  add Systick�жϻص��ӿ�
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

/* Includes ------------------------------------------------------------------*/
#include "ebox_core.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"

#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

// ��������жϵĴ���,��ebox_define�е���
volatile uint8_t irqCount = 0;
//cpu_t cpu;
cpu_clock_t clock;


#ifdef __cplusplus
extern "C" {
#endif

  // systick �жϻص�����ָ�룬
  fun_noPara_t  callBackFun;
  static uint16_t _multiple = 1;

  void nullFun(void){}

  /**
   *@brief    ע���жϻص�������ÿ���жϾ����ã�Ĭ������Ϊ1ms
   *@param    SystickCallBack_T fun �ص��������޲Σ�
   *@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
  */
  E_STATE SystickCallBackRegister(fun_noPara_t fun){
    if (callBackFun == NULL || callBackFun == nullFun)
    {
      callBackFun = fun;
      return E_OK;
    }else{
      return E_NG;
    }
  }

  /**
   *@brief    ע���жϻص�����������ָ���������� = �ж����� * multiple
   *@param    SystickCallBack_T fun �ص��������޲Σ�, uint8_t multiple �����������趨��������
   *@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
  */
  E_STATE SystickCallBackMultipleReg(fun_noPara_t fun,uint16_t multiple){

    if (callBackFun == NULL || callBackFun == nullFun)
    {
      callBackFun = fun;
      _multiple = multiple == 0 ? 1 : multiple;
      return E_OK;
    }else{
      return E_NG;
    }
  }

  /**
   *@brief    ����ϵͳʱ�ӣ��������������û��ڱ�ʵ�֣��������޸�ʱ��.Ĭ��48M @ HSI
   *@param    none
   *@retval   none
  */
  __weak void SystemClock_Config(void)
  {
    /* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
     Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
    						 ((8MHz * 12)/ 2)                  = 48MHz             */
    LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2};
    /* Variable to store AHB and APB buses clock configuration */
    /* Settings to have HCLK set to 48MHz and APB to 48 MHz */
    LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

    /* Switch to PLL with HSI as clock source             */
    LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
  }
    
  __IO uint32_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
  static uint8_t micro_para;

  /**
   *@brief   ebox��ʼ��������ʱ�ӣ�����systick�ж����ڣ�����ʼ���ж�
   *@param    none
   *@retval   none
  */
  void ebox_init(void)
  {
    SystemClock_Config();
		// ���θú������Լ���500�ֽ�code�ռ�
    get_system_clock();
//    get_chip_info();
    /* ����ʱ��Դ������£�systick��ʱ��Ƶ��ΪHCLK, 1ms�жϼ�����ֵӦΪHCLK/1000
     * ���HCLKΪ8M����systick��С����ʱ��Ϊ1/8us,
     */
    SysTick_Config(clock.hclk/1000);//  ÿ�� 1ms����һ���ж�
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);//systemticks clock��
    micro_para = (uint8_t)(clock.hclk/1000000);//����micros����������

    SystickCallBackRegister(nullFun);
  }
	
 /**
   *@brief    ��ȡϵͳ����ʱ��
   *@param    none
   *@retval   us
  */
  uint32_t micros(void)
  {
    //�����ʱ�����������ж��ҷ�����systick�������Ҫ��millis_secend���в���
    if (__get_PRIMASK() && ( SysTick->CTRL & (1 << 16)))
    {
      millis_seconds++; //ms����
    }
    /* millis_seconds * 1000 ��msת��Ϊus
     * cpu.clock.hclk/1000 Systick reloadֵ��Systick���¼���������cpu.clock.hclk/1000 - SysTick->VAL = ����ֵ
     * ����ֵ /(cpu.clock.hclk/1000000) = us
     * ����Ϊ millis_seconds *1000 + ((cpu.clock.hclk/1000 - SysTick->VAL)/(cpu.clock.hclk/1000000)) =
     * (millis_seconds * 1000 + (1000 - (SysTick->VAL / cpu.clock.hclk)*1000000))
     */
    //return  (millis_seconds * 1000 + (1000 - (SysTick->VAL / SysTick->LOAD)*1000));
    return  (millis_seconds * 1000 + (1000 - (SysTick->VAL / micro_para)));
  }
  
	/**
   *@brief    ��ȡϵͳ����ʱ��
   *@param    none
   *@retval   ms
  */
  uint32_t millis( void )
  {
    //�����ʱ�����������ж��ҷ�����systick�������Ҫ��millis_secend���в���
    if (__get_PRIMASK() && ( SysTick->CTRL & (1 << 16)) )
    {
      return millis_seconds++; //ms����
    }else{
      return millis_seconds;
    }
  }
	
 /**
   *@brief    ms��ʱ
   *@param    none
   *@retval   ms
  */
	 void delay_ms(uint32_t ms)
  {
    uint64_t end = GetEndTime(ms);
    while (!IsTimeOut(end));
  }
  /**
   *@brief    us��ʱ,ʹ��systick��������48Mhzʱ��ʱ��������us(1.3)���ȡ�8Mhzʱ��С6-7us,24Mhzʱ��С2.2us,16Mhzʱ��С3.5us
   *@param    uint16_t us  Ҫ��ʱ��ʱ������С1us
   *@retval   none
  */
  void  delay_us(uint16_t us)
  {
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

    ticks = (us-1) *micro_para;             /* �������� */
    tcnt = 0;
    told = SysTick->VAL;               /* ���浱ǰ����ֵ */

    while (1)
    {
      tnow = SysTick->VAL;
      tcnt += (tnow < told)? (told-tnow):(SysTick->LOAD - tnow + told);
      if (tcnt >= ticks)break;
      told = tnow;
    }
  }

  void ebox_SysTick_Handler(void)//systick�ж�
  {
    if (millis_seconds++ % _multiple == 0)
    {
      callBackFun();
    }
  }
  /**
   *@brief    ��ȡϵͳʱ��
   *@param    none
   *@retval   ȫ�ֱ���clock
  */
  void get_system_clock()
  {
    SystemCoreClockUpdate();
    LL_RCC_GetSystemClocksFreq((LL_RCC_ClocksTypeDef*)&clock);
  }

  /**
   *@brief    ��ȡϵͳ��Ϣ��CPU_ID,flashsize
   *@param    *cpu��  cpu��Ϣָ�룬����cpu��Ϣ
   *@retval   none
  */
  void get_chip_info(cpu_t *cpu)
  {
    cpu->chip_id[2] = LL_GetUID_Word0(); //���ֽ�
    cpu->chip_id[1] = LL_GetUID_Word1(); //
    cpu->chip_id[0] = LL_GetUID_Word2(); //���ֽ�
    cpu->flash_size = LL_GetFlashSize(); //оƬflash����
  }

#ifdef __cplusplus
}


#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_pwr.h"

void eBox::sleep(void){
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  /* Set STOP_LPREGU mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_MAINREGU);
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  //LL_LPM_EnableDeepSleep();
  LL_LPM_EnableSleepOnExit();
  /* Request Wait For Interrupt */
  __WFI();
}

void eBox::stop(){
//	  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG,
//                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL,
//                                         LL_GPIO_PULL_NO, LL_GPIO_AF_0};

//  /* Set all GPIO in analog state to reduce power consumption,                */
//  /* Note: Debug using ST-Link is not possible during the execution of this   */
//  /*       example because communication between ST-link and the device       */
//  /*       under test is done through UART. All GPIO pins are disabled (set   */
//  /*       to analog input mode) including  UART I/O pins.                    */
//  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
//                           LL_AHB1_GRP1_PERIPH_GPIOB |
//                           LL_AHB1_GRP1_PERIPH_GPIOC |
//                           LL_AHB1_GRP1_PERIPH_GPIOD |
//                           LL_AHB1_GRP1_PERIPH_GPIOE |
//                           LL_AHB1_GRP1_PERIPH_GPIOF);

//  LL_GPIO_Init(GPIOA, &gpio_initstruct);
//  LL_GPIO_Init(GPIOB, &gpio_initstruct);
//  LL_GPIO_Init(GPIOC, &gpio_initstruct);
//  LL_GPIO_Init(GPIOD, &gpio_initstruct);
//  LL_GPIO_Init(GPIOE, &gpio_initstruct);
//  LL_GPIO_Init(GPIOF, &gpio_initstruct);

//  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA |
//                           LL_AHB1_GRP1_PERIPH_GPIOB |
//                           LL_AHB1_GRP1_PERIPH_GPIOC |
//                           LL_AHB1_GRP1_PERIPH_GPIOD |
//                           LL_AHB1_GRP1_PERIPH_GPIOE |
//                           LL_AHB1_GRP1_PERIPH_GPIOF);

  /** Request to enter STOP_LPREGU mode
    * Following procedure describe in STM32F0xx Reference Manual
    * See PWR part, section Low-power modes, STOP_LPREGU mode
    */
  /* Set STOP_LPREGU mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP_LPREGU);
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  //LL_LPM_EnableDeepSleep();
  LL_LPM_EnableSleepOnExit();

  /* Request Wait For Interrupt */
  __WFI();
}

void eBox::standby(void){
  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);

  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU();

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
  /* Set STANDBY mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  //LL_LPM_EnableDeepSleep();
  LL_LPM_EnableSleepOnExit();
  /* Request Wait For Interrupt */
  __WFI();
}


#endif
