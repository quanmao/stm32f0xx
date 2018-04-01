 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2017/07/07
  * @brief   uart例程，包含中断绑定 .
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

#include "ebox.h"


E_GPIO PA5(PA_5);
E_UART usart(USART1,UART1_TX,UART1_RX);

E_GPIO STEP_PIN(PB_8);
E_GPIO DIR_PIN(PB_9);


void setup()
{
	ebox_init();
	PA5.mode(OUTPUT_PP);
	usart.begin(115200);
	
	STEP_PIN.mode(OUTPUT_PP);
	DIR_PIN.mode(OUTPUT_PP);
}

int main(void)
{
    setup();
		DIR_PIN.reset();
    while(1)
    {
			
			STEP_PIN.set();
			delay_us(500);
			STEP_PIN.reset();
			delay_us(500);
    }
}




