#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// ebox ���
#include "ebox_define.h"
#include "ebox_config.h"
// Ŀ��cpu���
#include "target_mcu.h"


#ifdef __CC_ARM
    extern int Image$$RW_IRAM1$$ZI$$Limit;
    #define STM32_SRAM_BEGIN 	(&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
    #pragma section="HEAP"
#else
    extern int __bss_end;
#endif	 

// STM32_SRAM_SIZE������stm32fxx_define.h�ļ���,��ͬCPU���岻ͬ
#define STM32_SRAM_END          	(0x20000000 + STM32_SRAM_SIZE * 1024)

// ʹ����չ�ڴ�ʱ�Ķ��巽ʽ
#if defined (STM32_EX_SRAM)
    #define STM32_EX_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
    #define STM32_EX_SRAM_END      0x68080000 /* the end address of external SRAM */
#endif
	
  
  typedef struct EBOX_BLOCK_LINK
  {
    struct EBOX_BLOCK_LINK *nextFreeBlock;		/*<< The next free block in the list. */
    size_t blockSize;													/*<< The size of the free block. */
  } eboxBlockLink_t;

  void      ebox_heap_init(void *begin_addr, void *end_addr);			// ��ʼ��ָ�������ջ����core�ļ��е��ã���ʼ��
  void      *ebox_malloc( size_t xWantedSize );										// ����ָ����С�ڴ棬�����׵�ַ.ʵ�ʻ�ȡ��СΪ((xWantedSize+BYTE_ALIGNMENT-1)/BYTE_ALIGNMENT)*BYTE_ALIGNMENT+(sizeof(EBOX_BLOCK_LINK))
  void      ebox_free( void *ptr );																  // �ͷ�ָ���ڴ�
  size_t    ebox_get_free(void);																	  // ���ؿ����ڴ�
  size_t    ebox_get_sram_start_addr(void);											// ��ȡsram��ʼ��ַ
  size_t    ebox_get_sram_end_addr(void);												// ��ȡsram������ַ
//  uint16_t  ebox_free_block_print();


  int 	    ebox_memcmp(const void * cs,const void * ct, size_t count);	// �ڴ�Ƚϣ��Ƚ�cs��ct����count��С�����ݣ����ش����±�
  void 	    *ebox_memcpy(void * dst, const void *src, size_t count);    	// �ڴ濽������src���ݿ�����dst

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
