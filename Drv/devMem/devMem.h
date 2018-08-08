/**
 * @file   Mem.h                                                    
 * @brief  ÄÚ´æ¹ÜÀí                                            
 * 
 * @writer   yangbm                                          
 * @version  1.00
 * @date     2012.04.09
 * @history
 * @modify list :
 * 
 */
#ifndef __DEVMEM_H__
#define __DEVMEM_H__
#include "Typedef.h"

#define  MEM_BASE_ADDR   0x81000
#define  MEM_SIZE        0x40000
#define  UART_ADDR      0x17FE40

extern  uint8_t  MemCheck(void);
extern  uint16_t* MemTestDevice( volatile uint32_t* pui_BaseAddr, uint32_t ui_Bytes );
extern uint16_t * MemTestAddressBus(IN volatile uint16_t* pus_BaseAddr,IN uint32_t ui_Bytes);
#endif /*MEM_H_*/
