/******************************************************************************/
/**
*@file    dev_Xintf.h     
*@brief   外部总线控制                   
*@author  yangbm
*@version V1.0.0
*@date    2009.8.24
*@details v1.0.0 2009.8.24 yangbm create
*/
/******************************************************************************/
#ifndef __XINTRUPT_H__
#define __XINTRUPT_H__

/*****************************************************************************/
/**@brief   macro                                      						 */
/*****************************************************************************/
#define    REG_XINTF_XTIMING0        (*((volatile uint32_t*)0x00000B20))
#define    REG_XINTF_XTIMING1        (*((volatile uint32_t*)0x00000B22))
#define    REG_XINTF_XTIMING2        (*((volatile uint32_t*)0x00000B24))
#define    REG_XINTF_XTIMING6        (*((volatile uint32_t*)0x00000B2C))
#define    REG_XINTF_XTIMING7        (*((volatile uint32_t*)0x00000B2E))
#define    REG_XINTF_XINTCNF2        (*((volatile uint32_t*)0x00000B34))
#define    REG_XINTF_XBANK           (*((volatile uint32_t*)0x00000B38))
#define    REG_XINTF_XREVISION       (*((volatile uint32_t*)0x00000B3A))
/*****************************************************************************/
/**@brief   总线初始化接口                                      				     */
/*****************************************************************************/
extern void initXintfRegs(void);

#endif

