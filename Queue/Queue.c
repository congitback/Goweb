/******************************************************************************/
/**
*@file    Queue.c     
*@brief   队列操作                      
*@author  yangbm
*@version V1.0.0
*@date    2013.11.26
*@details v1.0.0 2013.11.26 yangbm create
*/
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h"
#include  "Queue.h"
#include  <string.h>


/*****************************************************************************/
/**
* @brief  void  Queue_Init
*         队列初始化
* @param  IN SQueueHdr *pst_QueueHdr:指向队列头 
* @param  IN uint16_t us_QueueLen：队列数据区长度,必须为2的N次方
* @return void
*/
/*****************************************************************************/
void  Queue_Init(IN SQueueHdr *pst_QueueHdr, IN uint16_t us_QueueLen)
{
	uint8_t  uc_Remainder = 0;
	
	SOS_Assert(pst_QueueHdr == NULLS);
	
	SOS_Assert(0 == us_QueueLen);
	
	uc_Remainder = us_QueueLen%2;
	
	SOS_Assert(0 != uc_Remainder);
	
	memset(pst_QueueHdr, 0, sizeof(pst_QueueHdr));
	
	pst_QueueHdr->us_QueueLen = us_QueueLen;
	
	pst_QueueHdr->us_QueueLenMask = us_QueueLen -1;
}

/*****************************************************************************/
/**
* @brief  uint8_t  Queue_Write
*         队列初始化
* @param  IN SQueueHdr *pst_QueueHdr:指向队列头 
* @param  IN uint8_t auc_Data[]:写入数据
* @param  IN uint16_t us_Len:写入数据长度
* @return uint8_t:SYS_YES
*/
/*****************************************************************************/
uint8_t  Queue_Write(IN SQueueHdr *pst_QueueHdr, IN uint8_t auc_Data[], IN uint16_t us_Len  )
{
	uint16_t us_Count = 0U;
	
	SOS_Assert(NULLS == pst_QueueHdr);

	SOS_Assert(SYS_YES != Queue_IsValid(pst_QueueHdr));
	
	SOS_Assert(pst_QueueHdr->us_Tail >= pst_QueueHdr->us_QueueLen);

	SOS_Assert(pst_QueueHdr->us_Count > pst_QueueHdr->us_QueueLen);	

	SOS_Assert(NULLS == auc_Data);
	
	for ( us_Count = 0; us_Count < us_Len; us_Count++)
	{
		pst_QueueHdr->auc_Buf[pst_QueueHdr->us_Tail] = auc_Data[us_Count];
		pst_QueueHdr->us_Tail = (pst_QueueHdr->us_Tail + 1)&pst_QueueHdr->us_QueueLenMask;
		
		if (pst_QueueHdr->us_QueueLen == pst_QueueHdr->us_Count)
		{
			pst_QueueHdr->us_Head = (pst_QueueHdr->us_Head + 1)&pst_QueueHdr->us_QueueLenMask;	
		}
		else
		{
			pst_QueueHdr->us_Count++;	
		}	
	}

	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint16_t  Queue_Read
*         队列对操作
* @param  IN SQueueHdr *pst_QueueHdr:指向队列头 
* @param  OUT uint8_t auc_Data[]:数据返回存储区
* @param  IN uint16_t us_Len:存储区长度
* @return uint16_t：返回读取的数长度
*/
/*****************************************************************************/
uint16_t  Queue_Read(IN SQueueHdr *pst_QueueHdr, OUT uint8_t auc_Data[], IN uint16_t us_Len  )
{
	uint16_t us_Count = 0;
	uint16_t us_ReadLen = 0;
	
	SOS_Assert(NULLS == pst_QueueHdr);

	SOS_Assert(SYS_YES != Queue_IsValid(pst_QueueHdr));
	
	SOS_Assert(pst_QueueHdr->us_Tail >= pst_QueueHdr->us_QueueLen);

	SOS_Assert(pst_QueueHdr->us_Count > pst_QueueHdr->us_QueueLen);	

	SOS_Assert(NULLS == auc_Data);
	
	if (pst_QueueHdr->us_Count > us_Len)
	{
		us_ReadLen = us_Len;
	}
	else
	{
		us_ReadLen = pst_QueueHdr->us_Count;
	}
	
	for ( us_Count = 0; us_Count < us_ReadLen; us_Count++)
	{
		auc_Data[us_Count] = pst_QueueHdr->auc_Buf[pst_QueueHdr->us_Head];
		pst_QueueHdr->us_Head = (pst_QueueHdr->us_Head + 1)&pst_QueueHdr->us_QueueLenMask;
	}
	pst_QueueHdr->us_Count = pst_QueueHdr->us_Count - us_ReadLen;

	return us_ReadLen;
}

 
/*****************************************************************************/
/**
* @brief  uint8_t  Queue_IsValid
*         队列结构有效性判断
* @param  IN SQueueHdr *pst_QueueHdr:指向队列头 
* @return uint8_t:SYS_YES---队列结构正确,SYS_NO---队列结构错误
*/
/*****************************************************************************/
uint8_t  Queue_IsValid(IN SQueueHdr *pst_QueueHdr)
{
	uint8_t uc_Ret = SYS_YES;
	if ((pst_QueueHdr->us_QueueLen%2)==0)
	{
		if (( pst_QueueHdr->us_QueueLen - 1) == pst_QueueHdr->us_QueueLenMask)
		{
			uc_Ret = SYS_YES;
		}
		else
		{
			uc_Ret = SYS_NO;
		}
	}
	else
	{
		uc_Ret = SYS_NO;
	}

	return uc_Ret;
}
