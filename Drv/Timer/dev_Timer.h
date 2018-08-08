/******************************************************************************/
/**
*@file    dev_Timer.h       
*@brief   时间模块头文件                  
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
#ifndef __DEV_TIMER_H__
#define __DEV_TIMER_H__

/*****************************************************************************/
/**@brief  定时调度回调函数类型                                                        */
/*****************************************************************************/
typedef void (*TIMERPROC)( uint8_t uc_Param);

/*****************************************************************************/
/**@brief  定时调度执行对象结构                                                         */
/*****************************************************************************/
typedef struct  tagTimerObj
{
	uint8_t  uc_Init;				/*初始化标志,SYS_YES表示已初始化，可以执行*/
	uint8_t  uc_Param;				/*执行函数输入参数*/
	TIMERPROC  fnTimerProc;			/*指向被执行执行函数*/
}STimerObj;

/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
/* 定时器寄存器基地址 */
#define BASEADDR_TIMER0    0x00000c00		/* 定时器0寄存器基地址 */
#define BASEADDR_TIMER1    0x00000c08		/* 定时器1寄存器基地址 */
#define BASEADDR_TIMER2    0x00000c10		/* 定时器2寄存器基地址 */

/* 定时器中断向量地址 */
#define VECT_TIMER0          ((volatile PINT*)0x00000D4C)	/* 定时器0中断向量地址 */
#define VECT_TIMER1          ((volatile PINT*)0x00000D1A)	/* 定时器1中断向量地址 */
#define VECT_TIMER2          ((volatile PINT*)0x00000D1C)	/* 定时器2中断向量地址 */

#define XMNICR_CONFIG        ((volatile uint16_t*)0x00007077)


/*****************************************************************************/
/**@brief   时间模块接口声明                                                    	     */
/*****************************************************************************/
extern uint32_t  getTick(void);
/* 初始化定时器0 */
extern void initTimer0(void);
/* 获取系统us值,基本刻度10us */
extern uint32_t  getMicrosecond(void);
/*时间超时判断，单位ms*/
extern uint8_t IsTimeOut(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer);
/* 10us级时间超时判断 */
extern uint8_t IsTimeOutUs(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer);
/* 延时，基本刻度10us */
extern void delay10Us(IN uint32_t  ui_DelayTimer);

#endif


