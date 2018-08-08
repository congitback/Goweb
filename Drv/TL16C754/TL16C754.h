/******************************************************************************/
/**
*@file    TL16C754.h       
*@brief   header of TL16C754 uart driver                   
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/

#ifndef __TL16C754_H_
#define __TL16C754_H_

#include  "Queue.h"

/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define   TL16C754_DATA_VALID   		0x01		/*��ȡ������Ч�Բ�����*/
#define   TL16C754_ERR_CODE   			0x02		/*��ȡ����״̬������*/

#define  TIMER_OVER_DATA_TL16C754   	2000L		/* ���ݡ�ͨ�Ź����ж�ʱ�� */
#define  TIMER_OVER_COM_TL16C754    	2000L
#define  TL16C754_SND_INT_ENABLE        0x02		/*�����ж�ʹ��λ*/
#define  TH_SND_DATA_NUM_TL16C754   	55			/* ÿ�η���д�����ݳ��� */

#define  TH_REV_DATA_CNT_TL16C754   	100			/*ÿ�ζ�ȡ�������ݳ�������*/
#define  RHR_INTERRUPT     				0x04U		/*�����ж�����λ*/
#define  THR_INTERRUPT     				0x02U		/*���ж�����λ*/
#define  MASK_INT_TLC754				0x06U		/*�����ж�����λ*/

#define  ADDR_BASE_TL16C754         	0x100000L	/* TL16C754����ַ */
/* �Ĵ�����Ե�ַ���� */
#define  RHR_RELATIVE_ADDR    	   		0x0L
#define  THR_RELATIVE_ADDR       		0x0L
#define  IER_RELATIVE_ADDR       		0x1L
#define  IIR_RELATIVE_ADDR       		0x2L
#define  FCR_RELATIVE_ADDR       		0x2L
#define  LCR_RELATIVE_ADDR      		0x3U
#define  MCR_RELATIVE_ADDR       		0x4L
#define  LSR_RELATIVE_ADDR       		0x5L 
#define  MSR_RELATIVE_ADDR       		0x6L
#define  SPR_RELATIVE_ADDR       		0x7L

#define  DLL_RELATIVE_ADDR       		0x0L
#define  DLH_RELATIVE_ADDR       		0x1L
#define  EFR_RELATIVE_ADDR       		0x2L
#define  XON1_RELATIVE_ADDR      		0x4L
#define  XON2_RELATIVE_ADDR      		0x5L
#define  XOFF1_RELATIVE_ADDR    		0x6L
#define  XOFF2_RELATIVE_ADDR     		0x7L
#define  TCR_RELATIVE_ADDR       		0x6L
#define  TLR_RELATIVE_ADDR       		0x7L
#define  FIFOREADY_RELATIVE_ADDR 		0x7L
/* TL16C754��������(MAX_TL16C754_DEV_NUM - 1),1~(MAX_TL16C754_DEV_NUM-1)��Ч��0δ�� */
#ifdef __NET_CARD__
#define  MAX_TL16C754_DEV_NUM       	0x4U
#else
#define  MAX_TL16C754_DEV_NUM       	0x8U
#endif
/* �Ĵ��������Ծ���*/
#define  SPACE_ADDR_CHANNEL		    	0x400L

#define  CRYSTAL_FREQUENCY_TL16C754		24000000L	/* TL16C754����Ƶ�� */

#define  SEL_CLOCK_TL16C754    			0x1L	/*ʱ�Ӳ���λ*/

#define  MASK_RXHOLD_STATE_LSR  		0x01U	/*����״̬����λ*/

/*-------------------ExUart 1 config----------------------------*/

#define  BAUD_TL16C754_CHAN1     		500000U		/*����1������*/
#define  LEN_DATA_TL16C754_CHAN1 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN1 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN1 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN1   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 2 config----------------------------*/
#define  BAUD_TL16C754_CHAN2     		500000U		/*����2������*/

#define  LEN_DATA_TL16C754_CHAN2 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN2 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN2 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN2   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 3 config----------------------------*/
#define  BAUD_TL16C754_CHAN3     		500000U      /*����3������*/

#define  LEN_DATA_TL16C754_CHAN3 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/


#define  STOP_BIT_TL16C754_CHAN3 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN3 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN3   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN4     		500000U    /*����4������*/

#define  LEN_DATA_TL16C754_CHAN4 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN4 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN4 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN4   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 5 config----------------------------*/
#define  BAUD_TL16C754_CHAN5     		38400U     /*����5������*/

#define  LEN_DATA_TL16C754_CHAN5 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN5 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN5 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN5   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 6 config----------------------------*/
#define  BAUD_TL16C754_CHAN6     		38400     /*����6������*/

#define  LEN_DATA_TL16C754_CHAN6 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN6 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN6 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN6   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN7     		38400    /*����7������*/

#define  LEN_DATA_TL16C754_CHAN7 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN7 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN7 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN7   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN8     		38400       /*����8������*/

#define  LEN_DATA_TL16C754_CHAN8 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN8 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN8	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN8   	0x0U	/*0x0 -- Odd,0x1-- even */
/*---------------------��������-----------------------------------*/

#define   EXUART_CONFIG_CHAN1   ((LEN_DATA_TL16C754_CHAN1<<0)|(STOP_BIT_TL16C754_CHAN1<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN1<<3)|(TYPE_PARITY_TL16C754_CHAN1<<4))
								
#define   EXUART_CONFIG_CHAN2   ((LEN_DATA_TL16C754_CHAN2<<0)|(STOP_BIT_TL16C754_CHAN2<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN2<<3)|(TYPE_PARITY_TL16C754_CHAN2<<4))
								
#define   EXUART_CONFIG_CHAN3   ((LEN_DATA_TL16C754_CHAN3<<0)|(STOP_BIT_TL16C754_CHAN3<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN3<<3)|(TYPE_PARITY_TL16C754_CHAN3<<4))
								
#define   EXUART_CONFIG_CHAN4   ((LEN_DATA_TL16C754_CHAN4<<0)|(STOP_BIT_TL16C754_CHAN4<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN4<<3)|(TYPE_PARITY_TL16C754_CHAN4<<4))
								
#define   EXUART_CONFIG_CHAN5   ((LEN_DATA_TL16C754_CHAN5<<0)|(STOP_BIT_TL16C754_CHAN5<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN5<<3)|(TYPE_PARITY_TL16C754_CHAN5<<4))
								
#define   EXUART_CONFIG_CHAN6   ((LEN_DATA_TL16C754_CHAN6<<0)|(STOP_BIT_TL16C754_CHAN6<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN6<<3)|(TYPE_PARITY_TL16C754_CHAN6<<4))
								
#define   EXUART_CONFIG_CHAN7   ((LEN_DATA_TL16C754_CHAN7<<0)|(STOP_BIT_TL16C754_CHAN7<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN7<<3)|(TYPE_PARITY_TL16C754_CHAN7<<4))
								
#define   EXUART_CONFIG_CHAN8   ((LEN_DATA_TL16C754_CHAN8<<0)|(STOP_BIT_TL16C754_CHAN8<<2)|\
								(ENABLE_PARITY_TL16C754_CHAN8<<3)|(TYPE_PARITY_TL16C754_CHAN8<<4))

/* �����豸�ṹ�� 20170808 */
typedef   struct  tagExUartDev
{
	uint8_t  uc_Init;         /* ��ʼ����־��==SYS_OK---��ʼ���ɹ�������---��ʼ��ʧ�� */
	uint32_t ui_ErrCtr;		  /* ������� */
	uint16_t us_ErrCode;	  /* ������ */
}SExUartDev;

/* ���ڽ���BUF�ṹ */
typedef  struct  tagExUartRevBuf
{
	uint8_t  uc_State;				/* ������Ч��״̬ */
	uint32_t ui_Timer;				/* ���ݸ���ʱ�� */
	SQueueSmall  st_RevQueueSmall;			/* �������ݶ��� */	
}SExUartRevBuf;

/* ���ڷ���BUF�ṹ */
typedef  struct  tagExUartSndBuf
{
	SQueueLarge  st_SndQueueLarge;		   /* ���ݶ��� */
}SExUartSndBuf;

/*****************************************************************************/
/**@brief  ģ��ӿ�����                                	                      	 */
/*****************************************************************************/
extern uint8_t  dev_ExUartOpen(IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag);
/* �ⲿ����д������������д�������㷢�ʹ洢������ */
extern uint8_t  dev_ExUartWrite( IN uint8_t uc_Minor, IN char_t ac_Buf[], IN int32_t i_Len);
/* �ⲿ���ڶ�������������д����������ն������� */
extern int32_t  dev_ExUartRead(IN uint8_t uc_Minor, OUT char_t ac_Buf[],IN int32_t i_Len);
/* �ⲿ���ڿ��Ʋ�������ȡ�����豸�Ĺ����롢����״̬��ͨ��״̬ */
extern uint8_t  dev_ExUartIoctl(IN uint8_t uc_Minor, IN uint32_t i_Arg, uint16_t *pus_Data );
/* �ⲿ���ڼ�� */
extern uint32_t dev_ExUartCheck(void);

#endif /*TL16C754_H_*/
