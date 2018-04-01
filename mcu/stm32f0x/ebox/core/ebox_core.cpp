/**
  ******************************************************************************
  * @file    ebox_core.c
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief
  *     2017年3月4日  add Systick中断回调接口
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

// 标记屏蔽中断的次数,在ebox_define中调用
volatile uint8_t irqCount = 0;
//cpu_t cpu;
cpu_clock_t clock;


#ifdef __cplusplus
extern "C" {
#endif

  // systick 中断回调函数指针，
  fun_noPara_t  callBackFun;
  static uint16_t _multiple = 1;

  void nullFun(void){}

  /**
   *@brief    注册中断回调函数，每次中断均调用，默认周期为1ms
   *@param    SystickCallBack_T fun 回调函数（无参）
   *@retval   E_OK  注册成功， E_NG 注册失败
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
   *@brief    注册中断回调函数，可以指定调用周期 = 中断周期 * multiple
   *@param    SystickCallBack_T fun 回调函数（无参）, uint8_t multiple 倍数。用来设定调用周期
   *@retval   E_OK  注册成功， E_NG 注册失败
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
   *@brief    配置系统时钟，弱声明，允许用户在别处实现，以用来修改时钟.默认48M @ HSI
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
    
  __IO uint32_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
  static uint8_t micro_para;

  /**
   *@brief   ebox初始化，配置时钟，设置systick中断周期，并初始化中断
   *@param    none
   *@retval   none
  */
  void ebox_init(void)
  {
    SystemClock_Config();
		// 屏蔽该函数可以减少500字节code空间
    get_system_clock();
//    get_chip_info();
    /* 不改时钟源的情况下，systick的时钟频率为HCLK, 1ms中断计数器值应为HCLK/1000
     * 如果HCLK为8M，则systick最小计数时间为1/8us,
     */
    SysTick_Config(clock.hclk/1000);//  每隔 1ms产生一次中断
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);//systemticks clock；
    micro_para = (uint8_t)(clock.hclk/1000000);//减少micros函数计算量

    SystickCallBackRegister(nullFun);
  }
	
 /**
   *@brief    获取系统运行时间
   *@param    none
   *@retval   us
  */
  uint32_t micros(void)
  {
    //如果此时屏蔽了所有中断且发生了systick溢出，需要对millis_secend进行补偿
    if (__get_PRIMASK() && ( SysTick->CTRL & (1 << 16)))
    {
      millis_seconds++; //ms计数
    }
    /* millis_seconds * 1000 将ms转化为us
     * cpu.clock.hclk/1000 Systick reload值，Systick向下计数，所以cpu.clock.hclk/1000 - SysTick->VAL = 计数值
     * 计数值 /(cpu.clock.hclk/1000000) = us
     * 最终为 millis_seconds *1000 + ((cpu.clock.hclk/1000 - SysTick->VAL)/(cpu.clock.hclk/1000000)) =
     * (millis_seconds * 1000 + (1000 - (SysTick->VAL / cpu.clock.hclk)*1000000))
     */
    //return  (millis_seconds * 1000 + (1000 - (SysTick->VAL / SysTick->LOAD)*1000));
    return  (millis_seconds * 1000 + (1000 - (SysTick->VAL / micro_para)));
  }
  
	/**
   *@brief    获取系统运行时间
   *@param    none
   *@retval   ms
  */
  uint32_t millis( void )
  {
    //如果此时屏蔽了所有中断且发生了systick溢出，需要对millis_secend进行补偿
    if (__get_PRIMASK() && ( SysTick->CTRL & (1 << 16)) )
    {
      return millis_seconds++; //ms计数
    }else{
      return millis_seconds;
    }
  }
	
 /**
   *@brief    ms延时
   *@param    none
   *@retval   ms
  */
	 void delay_ms(uint32_t ms)
  {
    uint64_t end = GetEndTime(ms);
    while (!IsTimeOut(end));
  }
  /**
   *@brief    us延时,使用systick计数器。48Mhz时钟时可以满足us(1.3)精度。8Mhz时最小6-7us,24Mhz时最小2.2us,16Mhz时最小3.5us
   *@param    uint16_t us  要延时的时长，最小1us
   *@retval   none
  */
  void  delay_us(uint16_t us)
  {
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

    ticks = (us-1) *micro_para;             /* 计数周期 */
    tcnt = 0;
    told = SysTick->VAL;               /* 保存当前计数值 */

    while (1)
    {
      tnow = SysTick->VAL;
      tcnt += (tnow < told)? (told-tnow):(SysTick->LOAD - tnow + told);
      if (tcnt >= ticks)break;
      told = tnow;
    }
  }

  void ebox_SysTick_Handler(void)//systick中断
  {
    if (millis_seconds++ % _multiple == 0)
    {
      callBackFun();
    }
  }
  /**
   *@brief    获取系统时钟
   *@param    none
   *@retval   全局变量clock
  */
  void get_system_clock()
  {
    SystemCoreClockUpdate();
    LL_RCC_GetSystemClocksFreq((LL_RCC_ClocksTypeDef*)&clock);
  }

  /**
   *@brief    获取系统信息，CPU_ID,flashsize
   *@param    *cpu：  cpu信息指针，返回cpu信息
   *@retval   none
  */
  void get_chip_info(cpu_t *cpu)
  {
    cpu->chip_id[2] = LL_GetUID_Word0(); //低字节
    cpu->chip_id[1] = LL_GetUID_Word1(); //
    cpu->chip_id[0] = LL_GetUID_Word2(); //高字节
    cpu->flash_size = LL_GetFlashSize(); //芯片flash容量
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
