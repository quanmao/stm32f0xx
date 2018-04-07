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
	usart.printf("0x%x \r\n",get_resetType());
}



#define DELAY 25
uint8_t i =250;
E_WDG dog;
#define GETEND(a) 	(a+i)
#define ISOUT(end,delay)  ((uint8_t)(end-i)>delay)
int main(void)
{
//		uint8_t i = 250;
		uint8_t j = DELAY + i;

    setup();
//		dog.begin(26208);
		DIR_PIN.reset();
    while(1)
    {
				delay_ms(10000);
				usart.printf("准备复位");
				ebox_rst();
				for(;1;i++){
//						usart.printf( "i = %d j = %d cha = %d  bool = %d bool = %d  \r\n",i,j,(uint8_t)(j-i),j>=i,ISOUT(j,DELAY));
//						delay_ms(5000);
				}
    }
}




