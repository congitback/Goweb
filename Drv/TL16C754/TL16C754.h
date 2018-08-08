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
#define   TL16C754_DATA_VALID   		0x01		/*读取数据有效性操作码*/
#define   TL16C754_ERR_CODE   			0x02		/*读取错误状态操作码*/

#define  TIMER_OVER_DATA_TL16C754   	2000L		/* 数据、通信故障判断时间 */
#define  TIMER_OVER_COM_TL16C754    	2000L
#define  TL16C754_SND_INT_ENABLE        0x02		/*发送中断使能位*/
#define  TH_SND_DATA_NUM_TL16C754   	55			/* 每次发送写入数据长度 */

#define  TH_REV_DATA_CNT_TL16C754   	100			/*每次读取操作数据长度门限*/
#define  RHR_INTERRUPT     				0x04U		/*接收中断屏蔽位*/
#define  THR_INTERRUPT     				0x02U		/*发中断屏蔽位*/
#define  MASK_INT_TLC754				0x06U		/*串口中断屏蔽位*/

#define  ADDR_BASE_TL16C754         	0x100000L	/* TL16C754基地址 */
/* 寄存器相对地址定义 */
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
/* TL16C754串口数量(MAX_TL16C754_DEV_NUM - 1),1~(MAX_TL16C754_DEV_NUM-1)有效，0未用 */
#ifdef __NET_CARD__
#define  MAX_TL16C754_DEV_NUM       	0x4U
#else
#define  MAX_TL16C754_DEV_NUM       	0x8U
#endif
/* 寄存器组间相对距离*/
#define  SPACE_ADDR_CHANNEL		    	0x400L

#define  CRYSTAL_FREQUENCY_TL16C754		24000000L	/* TL16C754晶振频率 */

#define  SEL_CLOCK_TL16C754    			0x1L	/*时钟参数位*/

#define  MASK_RXHOLD_STATE_LSR  		0x01U	/*接收状态屏蔽位*/

/*-------------------ExUart 1 config----------------------------*/

#define  BAUD_TL16C754_CHAN1     		500000U		/*串口1波特率*/
#define  LEN_DATA_TL16C754_CHAN1 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN1 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN1 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN1   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 2 config----------------------------*/
#define  BAUD_TL16C754_CHAN2     		500000U		/*串口2波特率*/

#define  LEN_DATA_TL16C754_CHAN2 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN2 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN2 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN2   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 3 config----------------------------*/
#define  BAUD_TL16C754_CHAN3     		500000U      /*串口3波特率*/

#define  LEN_DATA_TL16C754_CHAN3 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/


#define  STOP_BIT_TL16C754_CHAN3 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN3 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN3   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN4     		500000U    /*串口4波特率*/

#define  LEN_DATA_TL16C754_CHAN4 0x3U				/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN4 0x0U				/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN4 0x0U			/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN4   0x0U			/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 5 config----------------------------*/
#define  BAUD_TL16C754_CHAN5     		38400U     /*串口5波特率*/

#define  LEN_DATA_TL16C754_CHAN5 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN5 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN5 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN5   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 6 config----------------------------*/
#define  BAUD_TL16C754_CHAN6     		38400     /*串口6波特率*/

#define  LEN_DATA_TL16C754_CHAN6 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN6 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN6 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN6   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN7     		38400    /*串口7波特率*/

#define  LEN_DATA_TL16C754_CHAN7 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN7 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN7 	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN7   	0x0U	/*0x0 -- Odd,0x1-- even */

/*-------------------ExUart 4 config----------------------------*/
#define  BAUD_TL16C754_CHAN8     		38400       /*串口8波特率*/

#define  LEN_DATA_TL16C754_CHAN8 		0x3U	/*0x00 --5,0x01--6,0x2--7,0x3--8*/

#define  STOP_BIT_TL16C754_CHAN8 		0x0U	/*0x0 -- 1 Stop bit, 0x1--2 Stop bit*/

#define  ENABLE_PARITY_TL16C754_CHAN8	0x0U	/*0x0 -- NO Parity, 0x1-- Parity*/

#define  TYPE_PARITY_TL16C754_CHAN8   	0x0U	/*0x0 -- Odd,0x1-- even */
/*---------------------串口配置-----------------------------------*/

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

/* 串口设备结构树 20170808 */
typedef   struct  tagExUartDev
{
	uint8_t  uc_Init;         /* 初始化标志，==SYS_OK---初始化成功，其他---初始化失败 */
	uint32_t ui_ErrCtr;		  /* 错误计数 */
	uint16_t us_ErrCode;	  /* 错误码 */
}SExUartDev;

/* 串口接收BUF结构 */
typedef  struct  tagExUartRevBuf
{
	uint8_t  uc_State;				/* 数据有效性状态 */
	uint32_t ui_Timer;				/* 数据更新时间 */
	SQueueSmall  st_RevQueueSmall;			/* 接收数据队列 */	
}SExUartRevBuf;

/* 串口发送BUF结构 */
typedef  struct  tagExUartSndBuf
{
	SQueueLarge  st_SndQueueLarge;		   /* 数据队列 */
}SExUartSndBuf;

/*****************************************************************************/
/**@brief  模块接口声明                                	                      	 */
/*****************************************************************************/
extern uint8_t  dev_ExUartOpen(IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag);
/* 外部串口写操作，将数据写入驱动层发送存储队列中 */
extern uint8_t  dev_ExUartWrite( IN uint8_t uc_Minor, IN char_t ac_Buf[], IN int32_t i_Len);
/* 外部串口读操作，将数据写入驱动层接收队列数据 */
extern int32_t  dev_ExUartRead(IN uint8_t uc_Minor, OUT char_t ac_Buf[],IN int32_t i_Len);
/* 外部串口控制操作，获取串口设备的故障码、数据状态、通信状态 */
extern uint8_t  dev_ExUartIoctl(IN uint8_t uc_Minor, IN uint32_t i_Arg, uint16_t *pus_Data );
/* 外部串口检测 */
extern uint32_t dev_ExUartCheck(void);

#endif /*TL16C754_H_*/
