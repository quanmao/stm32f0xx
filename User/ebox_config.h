/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox���òü��ļ�
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

#ifndef __EBOX_CONFIG_H
#define __EBOX_CONFIG_H

//�Ƿ�ʹ��printf����,�ù���ռ�ô洢�ռ�϶࣬Ŀǰȷ�ϵ�ʹ�øù��ܵİ���uart��1602
#define USE_PRINTF	1

#if USE_PRINTF
//�Ƿ�ʹ��DEBUG,��Ҫ��ebox_debug.h�ļ��ж�������豸 1 ʹ��  0 ��ʹ��
#define EBOX_DEBUG  1
#endif

/*ѡ���ڴ�����ļ�, 1 ʹ��ebxo_mem�ڴ���� 0 microlib�ڴ����
 * ebox_mem������δʹ���ڴ���Ϊ�ڴ��������,������ռ��flash��microlib��Լ500byte
 * microlib�ɹ�����ڴ�������.s�����ļ���heap_size����
 */
#define USE_EBOX_MEM  1


/*�ڴ���С����̬�����ڴ�ʱ����,��С���׵��¶�β��ܷ���ɹ��������˷��ڴ档�����
 *���������ļ� starup_stm32fxxxx.s �е� Heap_Size
 *printfģ���е���
 */
#define	BUF_BLOCK				64	//64

/* flashд��������ʱ���Ƿ�������ǵ�ǰ�������������; 
 * 0 д�������ݣ���������ǰ��������λ�þ����ݡ� ��Ҫռ��FLASH_PAGE_SIZE��С���ڴ���Ϊ������
 * 1 ֻ������д������ݡ� ��ʡ�ڴ�
 */
#define FLASH_OVERRIDE	1

//�Ƿ�ʹ�ó�ʱ,1 ʹ��  0 ��ʹ��
#define USE_TIMEOUT			1
//�Ƿ�ʹ���ڲ��ο���ѹ 1 ʹ��, 0 ��ʹ��
#define USE_INTERVREF		0


#endif

