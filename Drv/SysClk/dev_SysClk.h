/******************************************************************************/
/**
*@file    dev_SysClk.h     
*@brief   ϵͳʱ�ӳ�ʼ��ģ��ͷ�ļ�������ϵͳʱ�ӼĴ�                  
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
#define REG_SYSCLK_HISPCP     (*((volatile uint16_t *)0x0000701AU))	/* HISPCP�Ĵ������� */
#define REG_SYSCLK_LOSPCP     (*((volatile uint16_t *)0x0000701BU))	/* LOSPCP�Ĵ������� */
#define REG_SYSCLK_PCLKCR     (*((volatile uint16_t *)0x0000701CU))	/* PCLKCR�Ĵ������� */
#define REG_SYSCLK_LPMCR0     (*((volatile uint16_t *)0x0000701EU))	/* LPMCR0�Ĵ������� */
#define REG_SYSCLK_LPMCR1     (*((volatile uint16_t *)0x0000701FU))	/* LPMCR1�Ĵ������� */
#define REG_SYSCLK_PLLCR      (*((volatile uint16_t *)0x00007021U))	/* PLLCR�Ĵ������� */
#define REG_SYSCLK_SCSR       (*((volatile uint16_t *)0x00007022U))	/* SCSR�Ĵ������� */
#define REG_SYSCLK_WDCNTR     (*((volatile uint16_t *)0x00007023U))	/* WDCNTR�Ĵ������� */
#define REG_SYSCLK_WDKEY      (*((volatile uint16_t *)0x00007025U))	/* WDKEY�Ĵ������� */
#define REG_SYSCLK_WDCR       (*((volatile uint16_t *)0x00007029U))	/* WDCR�Ĵ������� */

/*****************************************************************************/
/**@brief   ģ��ӿ�����                                                     	     */
/*****************************************************************************/
extern void initSysCtrl(void);

#endif 


