/******************************************************************************/
/**
*@file    Queue.h     
*@brief   队列管理                      
*@author  yangbm
*@version V1.0.0
*@date    2014.04.09
*@details v1.0.0 2014.04.09 yangbm create
*/
/******************************************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__
/*****************************************************************************/
/**@brief   macro                                                   		*/
/*****************************************************************************/
#define    QUEUELARGE_LEN_MAX                     2048		/*大队列数据区长度*/
#define    QUEUESMALL_LEN_MAX 					 512		/*小队列数据区长度*/

/*****************************************************************************/
/**@brief   队列头结构                                                     		*/
/*****************************************************************************/
typedef  struct  tagQueueHdr
{
	uint16_t  us_QueueLen;				/* 队列内数据区长度,必须为2的N次方 */
	uint16_t  us_QueueLenMask;			/* 队列内数据区长度掩码 */
	uint16_t  us_Count;					/* 队列内数据个数 */
	uint16_t  us_Head;					/* 队列内数据读头 */
	uint16_t  us_Tail;					/* 队列内数据写头 */
	uint8_t   auc_Buf[1];				/*指向数据区*/
}SQueueHdr;

/*****************************************************************************/
/**@brief   大队列结构                                                             	 */
/*****************************************************************************/
typedef  struct  tagQueueLarge
{
	SQueueHdr st_Hdr;						/* 队列头*/
	uint8_t   auc_Buf[QUEUELARGE_LEN_MAX];	/* 队列数据区 */	
}SQueueLarge;

/*****************************************************************************/
/**@brief   小队列结构                                                             	 */
/*****************************************************************************/
typedef  struct  tagQueueSmall
{
	SQueueHdr st_Hdr;						/* 队列头*/
	uint8_t   auc_Buf[QUEUESMALL_LEN_MAX];	/* 队列数据区 */	
}SQueueSmall;

/*****************************************************************************/
/**@brief   模块接口声明                                                             	 */
/*****************************************************************************/
extern  void  Queue_Init(IN SQueueHdr *pst_QueueHdr, IN uint16_t us_QueueLen);

extern  uint8_t  Queue_Write(IN SQueueHdr *pst_QueueHdr, IN uint8_t auc_Data[], IN uint16_t us_Len  );

extern  uint16_t  Queue_Read(IN SQueueHdr *pst_QueueHdr, OUT uint8_t auc_Data[], IN uint16_t us_Len );

extern  uint8_t  Queue_IsValid(IN SQueueHdr *pst_QueueHdr);
#endif /*__QUEUE_H_*/
