/******************************************************************************/
/**
*@file    dev_SysClk.h     
*@brief   系统时钟初始化模块头文件，定义系统时钟寄存                  
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
#ifndef __DEV_SYSCLK_H__
#define __DEV_SYSCLK_H__


/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define REG_SYSCLK_HISPCP     (*((volatile uint16_t *)0x0000701AU))	/* HISPCP寄存器定义 */
#define REG_SYSCLK_LOSPCP     (*((volatile uint16_t *)0x0000701BU))	/* LOSPCP寄存器定义 */
#define REG_SYSCLK_PCLKCR     (*((volatile uint16_t *)0x0000701CU))	/* PCLKCR寄存器定义 */
#define REG_SYSCLK_LPMCR0     (*((volatile uint16_t *)0x0000701EU))	/* LPMCR0寄存器定义 */
#define REG_SYSCLK_LPMCR1     (*((volatile uint16_t *)0x0000701FU))	/* LPMCR1寄存器定义 */
#define REG_SYSCLK_PLLCR      (*((volatile uint16_t *)0x00007021U))	/* PLLCR寄存器定义 */
#define REG_SYSCLK_SCSR       (*((volatile uint16_t *)0x00007022U))	/* SCSR寄存器定义 */
#define REG_SYSCLK_WDCNTR     (*((volatile uint16_t *)0x00007023U))	/* WDCNTR寄存器定义 */
#define REG_SYSCLK_WDKEY      (*((volatile uint16_t *)0x00007025U))	/* WDKEY寄存器定义 */
#define REG_SYSCLK_WDCR       (*((volatile uint16_t *)0x00007029U))	/* WDCR寄存器定义 */

/*****************************************************************************/
/**@brief   模块接口声明                                                     	     */
/*****************************************************************************/
extern void initSysCtrl(void);

#endif 


