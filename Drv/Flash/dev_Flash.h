/**
 * @file   Dev_Flash.h                                                   
 * @brief                                                                                                    
 * Data type.       
 *
 * @author   yangbm                                          
 * @version  1.00
 * @date     2011.03.01
 * @history
 */
#ifndef __DEV_FLASH_H__
#define __DEV_FALSH_H__
#include "Typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Flash Registers */

#define FLASH_SLEEP   0x0000U
#define FLASH_STANDBY 0x0001U
#define FLASH_ACTIVE  0x0003U
/* 内部FLASH寄存器定义 */

#define REG_FLASH_FOPT     (*((volatile uint16_t *)0x00000A80U))

#define REG_FLASH_FPWR     (*((volatile uint16_t *)0x00000A82U))
 
#define REG_FLASH_FSTATUS  (*((volatile uint16_t *)0x00000A83U))

#define REG_FLASH_FSTDBYWAIT  (*((volatile uint16_t *)0x00000A84U))

#define REG_FLASH_FACTIVEWAIT  (*((volatile uint16_t *)0x00000A85U))

#define REG_FLASH_FBANKWAIT  (*((volatile uint16_t *)0x00000A86U))

#define REG_FLASH_FOTPWAIT  (*((volatile uint16_t *)0x00000A87U))

extern void initFlash(void);

extern void cpyFlashInitSect(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

