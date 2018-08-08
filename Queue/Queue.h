/******************************************************************************/
/**
*@file    Queue.h     
*@brief   ���й���                      
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
#define    QUEUELARGE_LEN_MAX                     2048		/*���������������*/
#define    QUEUESMALL_LEN_MAX 					 512		/*С��������������*/

/*****************************************************************************/
/**@brief   ����ͷ�ṹ                                                     		*/
/*****************************************************************************/
typedef  struct  tagQueueHdr
{
	uint16_t  us_QueueLen;				/* ����������������,����Ϊ2��N�η� */
	uint16_t  us_QueueLenMask;			/* �������������������� */
	uint16_t  us_Count;					/* ���������ݸ��� */
	uint16_t  us_Head;					/* ���������ݶ�ͷ */
	uint16_t  us_Tail;					/* ����������дͷ */
	uint8_t   auc_Buf[1];				/*ָ��������*/
}SQueueHdr;

/*****************************************************************************/
/**@brief   ����нṹ                                                             	 */
/*****************************************************************************/
typedef  struct  tagQueueLarge
{
	SQueueHdr st_Hdr;						/* ����ͷ*/
	uint8_t   auc_Buf[QUEUELARGE_LEN_MAX];	/* ���������� */	
}SQueueLarge;

/*****************************************************************************/
/**@brief   С���нṹ                                                             	 */
/*****************************************************************************/
typedef  struct  tagQueueSmall
{
	SQueueHdr st_Hdr;						/* ����ͷ*/
	uint8_t   auc_Buf[QUEUESMALL_LEN_MAX];	/* ���������� */	
}SQueueSmall;

/*****************************************************************************/
/**@brief   ģ��ӿ�����                                                             	 */
/*****************************************************************************/
extern  void  Queue_Init(IN SQueueHdr *pst_QueueHdr, IN uint16_t us_QueueLen);

extern  uint8_t  Queue_Write(IN SQueueHdr *pst_QueueHdr, IN uint8_t auc_Data[], IN uint16_t us_Len  );

extern  uint16_t  Queue_Read(IN SQueueHdr *pst_QueueHdr, OUT uint8_t auc_Data[], IN uint16_t us_Len );

extern  uint8_t  Queue_IsValid(IN SQueueHdr *pst_QueueHdr);
#endif /*__QUEUE_H_*/
