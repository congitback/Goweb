/******************************************************************************/
/**
*@file    dev_Timer.h       
*@brief   ʱ��ģ��ͷ�ļ�                  
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
#ifndef __DEV_TIMER_H__
#define __DEV_TIMER_H__

/*****************************************************************************/
/**@brief  ��ʱ���Ȼص���������                                                        */
/*****************************************************************************/
typedef void (*TIMERPROC)( uint8_t uc_Param);

/*****************************************************************************/
/**@brief  ��ʱ����ִ�ж���ṹ                                                         */
/*****************************************************************************/
typedef struct  tagTimerObj
{
	uint8_t  uc_Init;				/*��ʼ����־,SYS_YES��ʾ�ѳ�ʼ��������ִ��*/
	uint8_t  uc_Param;				/*ִ�к����������*/
	TIMERPROC  fnTimerProc;			/*ָ��ִ��ִ�к���*/
}STimerObj;

/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
/* ��ʱ���Ĵ�������ַ */
#define BASEADDR_TIMER0    0x00000c00		/* ��ʱ��0�Ĵ�������ַ */
#define BASEADDR_TIMER1    0x00000c08		/* ��ʱ��1�Ĵ�������ַ */
#define BASEADDR_TIMER2    0x00000c10		/* ��ʱ��2�Ĵ�������ַ */

/* ��ʱ���ж�������ַ */
#define VECT_TIMER0          ((volatile PINT*)0x00000D4C)	/* ��ʱ��0�ж�������ַ */
#define VECT_TIMER1          ((volatile PINT*)0x00000D1A)	/* ��ʱ��1�ж�������ַ */
#define VECT_TIMER2          ((volatile PINT*)0x00000D1C)	/* ��ʱ��2�ж�������ַ */

#define XMNICR_CONFIG        ((volatile uint16_t*)0x00007077)


/*****************************************************************************/
/**@brief   ʱ��ģ��ӿ�����                                                    	     */
/*****************************************************************************/
extern uint32_t  getTick(void);
/* ��ʼ����ʱ��0 */
extern void initTimer0(void);
/* ��ȡϵͳusֵ,�����̶�10us */
extern uint32_t  getMicrosecond(void);
/*ʱ�䳬ʱ�жϣ���λms*/
extern uint8_t IsTimeOut(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer);
/* 10us��ʱ�䳬ʱ�ж� */
extern uint8_t IsTimeOutUs(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer);
/* ��ʱ�������̶�10us */
extern void delay10Us(IN uint32_t  ui_DelayTimer);

#endif


