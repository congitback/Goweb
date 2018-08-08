/******************************************************************************/
/**
*@file    dev_Gpio.h       
*@brief   GPIO模块寄存器数据结构定义                    
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  macro                                                     	     */
/*****************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

#include  "Typedef.h"
/* GPIO寄存器定义 */    	
#define  REG_GPIO_GPAMUX         (*((volatile uint16_t*)0x0070C0))
#define  REG_GPIO_GPADIR         (*((volatile uint16_t*)0x0070C1))
#define  REG_GPIO_GPAQUAL        (*((volatile uint16_t*)0x0070C2))
#define  REG_GPIO_GPADAT         (*((volatile uint16_t*)0x0070E0))

#define  REG_GPIO_GPBMUX         (*((volatile uint16_t*)0x0070C4))
#define  REG_GPIO_GPBDIR         (*((volatile uint16_t*)0x0070C5))
#define  REG_GPIO_GPBQUAL        (*((volatile uint16_t*)0x0070C6))
#define  REG_GPIO_GPBDAT         (*((volatile uint16_t*)0x0070E4))

#define  REG_GPIO_GPDMUX         (*((volatile uint16_t*)0x0070CC))
#define  REG_GPIO_GPDDIR         (*((volatile uint16_t*)0x0070CD))
#define  REG_GPIO_GPDQUAL        (*((volatile uint16_t*)0x0070CE))
#define  REG_GPIO_GPDDAT         (*((volatile uint16_t*)0x0070EC))

#define  REG_GPIO_GPEMUX         (*((volatile uint16_t*)0x0070D0))
#define  REG_GPIO_GPEDIR         (*((volatile uint16_t*)0x0070D1))
#define  REG_GPIO_GPEQUAL        (*((volatile uint16_t*)0x0070D2))
#define  REG_GPIO_GPEDAT         (*((volatile uint16_t*)0x0070F0))

#define  REG_GPIO_GPFMUX         (*((volatile uint16_t*)0x0070D4))
#define  REG_GPIO_GPFDIR         (*((volatile uint16_t*)0x0070D5))
#define  REG_GPIO_GPFDAT         (*((volatile uint16_t*)0x0070F4))

#define  REG_GPIO_GPGMUX         (*((volatile uint16_t*)0x0070D8))
#define  REG_GPIO_GPGDIR         (*((volatile uint16_t*)0x0070D9))
#define  REG_GPIO_GPGDAT         (*((volatile uint16_t*)0x0070F8))

#endif 

