#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// ebox 组件
#include "ebox_define.h"
#include "ebox_config.h"
// 目标cpu组件
#include "target_mcu.h"


#ifdef __CC_ARM
    extern int Image$$RW_IRAM1$$ZI$$Limit;
    #define STM32_SRAM_BEGIN 	(&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
    #pragma section="HEAP"
#else
    extern int __bss_end;
#endif	 

// STM32_SRAM_SIZE定义在stm32fxx_define.h文件中,不同CPU定义不同
#define STM32_SRAM_END          	(0x20000000 + STM32_SRAM_SIZE * 1024)

// 使用扩展内存时的定义方式
#if defined (STM32_EX_SRAM)
    #define STM32_EX_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
    #define STM32_EX_SRAM_END      0x68080000 /* the end address of external SRAM */
#endif
	
  
  typedef struct EBOX_BLOCK_LINK
  {
    struct EBOX_BLOCK_LINK *nextFreeBlock;		/*<< The next free block in the list. */
    size_t blockSize;													/*<< The size of the free block. */
  } eboxBlockLink_t;

  void      ebox_heap_init(void *begin_addr, void *end_addr);			// 初始化指定区域堆栈。在core文件中调用，初始化
  void      *ebox_malloc( size_t xWantedSize );										// 分配指定大小内存，返回首地址.实际获取大小为((xWantedSize+BYTE_ALIGNMENT-1)/BYTE_ALIGNMENT)*BYTE_ALIGNMENT+(sizeof(EBOX_BLOCK_LINK))
  void      ebox_free( void *ptr );																  // 释放指定内存
  size_t    ebox_get_free(void);																	  // 返回空闲内存
  size_t    ebox_get_sram_start_addr(void);											// 获取sram起始地址
  size_t    ebox_get_sram_end_addr(void);												// 获取sram结束地址
//  uint16_t  ebox_free_block_print();


  int 	    ebox_memcmp(const void * cs,const void * ct, size_t count);	// 内存比较，比较cs和ct区域count大小的数据，返回错误下标
  void 	    *ebox_memcpy(void * dst, const void *src, size_t count);    	// 内存拷贝，将src内容拷贝到dst

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
