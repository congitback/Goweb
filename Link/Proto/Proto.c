/******************************************************************************/
/**
*@file    Proto.c     
*@brief   ���Э��ģ��                   
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/ 
 #include  "Typedef.h"
 #include  "Proto.h"
 #include  "Driver.h"
 #include  "Config.h"
 #include  "Crc.h"
 #include  <string.h>

uint8_t   auc_TmpBuf[1700];

/*****************************************************************************/
/**@brief  ���Э�鴮���ֽ������ն���                                                   */
/*****************************************************************************/ 
static  SProtoRevQueue  gast_ProtoRevQueue[PROTO_CHAN_MAX];
 
/*****************************************************************************/
/**@brief  ���Э������֡�洢�ṹ                                                       */
/*****************************************************************************/
static  SProtoFrame      gast_ProtoFrame[PROTO_CHAN_MAX];

/*****************************************************************************/
/**@brief  �ֲ���������                                                                */
/*****************************************************************************/
static uint8_t  WriteProtoRevQueue(IN uint8_t uc_Port,IN uint8_t auc_Buf[], IN uint16_t us_Len);

static int16_t DrawFrameFromQueue(IN SProtoRevQueue *pst_Queue,OUT uint8_t auc_Buf[], IN uint16_t us_Len) ;

static int16_t GetProtoFrame(IN SProtoRevQueue *pst_Queue, IN uint16_t us_MsgLen ,OUT uint8_t auc_Buf[], IN uint16_t us_Len );

/*****************************************************************************/
/**
* @brief  ProtoMod_Open
*         Protoģ��򿪲�������ģ�����ݽ��г�ʼ���ʹ�ģ���Ӧ����ͨ��
* @param  void
* @return uint8_t : SYS_OK----�ɹ�, SYS_ERR----ʧ��
*/
/*****************************************************************************/
uint8_t  ProtoMod_Open(void)         /*���ĸ����� 20170808*/
{
	uint8_t uc_Ret = SYS_ERR;
	uint16_t us_Err = 0;
	
	memset(gast_ProtoRevQueue, 0 ,sizeof(gast_ProtoRevQueue));
	
	memset(gast_ProtoFrame, 0, sizeof(gast_ProtoFrame));
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN1,1, &us_Err);     /*�򿪴���1 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);

	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN2,1, &us_Err);     /*�򿪴���2 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN3,1, &us_Err);     /*����3 20170808*/    
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN4,1, &us_Err);    /*����4 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	return SYS_OK;
}

/*****************************************************************************/
/**
* @brief  int32_t ProtoMod_Send
*         Protoģ�鷢�Ͳ����ӿ�
* @param  IN uint8_t uc_Port ���ӿڱ��,1-4��Ч;
* @param  IN uint8_t uc_Cmd������������  
* @param  IN uint8_t auc_Buf[]��д������;	
* @param  IN uint16_t us_Len: д�����ݳ���;
* @return uint8_t : SYS_OK----�ɹ�, SYS_ERR----ʧ��
*/
/*****************************************************************************/ 
uint8_t  ProtoMod_Send(IN uint8_t uc_Port, IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*��Ҫͨ�����ڷ��͸�QJK��������auc_TmpBuf���ݿ���д���������� 20170808*/
	uint16_t us_Crc = 0;
	uint16_t us_MsgLen = 0;
	uint16_t us_Err = 0;
	uint8_t  uc_Ret = SYS_OK;
	SProtoHdr *pst_Hdr = NULLS;
	
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert(NULLS == auc_Buf);
	uc_Port = uc_Port -1;
	if ((us_Len + sizeof(SProtoHdr) + 1) >  1700)	/*���һ֡���ݣ�һ֡���ݲ��ܳ���1700 20170808*/
	{
		uc_Ret = SYS_ERR;	
	}
	else
	{
		pst_Hdr = (SProtoHdr*)auc_TmpBuf;                   /*auc_TmpBuf ȫ�ֱ�������һ���ڴ�飬���������������ݿ������������������� 20170808*/
		pst_Hdr->uc_SFD = FLAG_SFD_CMD;
		pst_Hdr->uc_SFL = FLAG_SFL_CMD;
		pst_Hdr->auc_NSNH = 0;
		pst_Hdr->auc_NSNL = 0;
		pst_Hdr->uc_CmdNum = uc_Cmd;
		
		memcpy(pst_Hdr->auc_Data,(char*)auc_Buf,us_Len);
	
		us_MsgLen =  us_Len + sizeof(SProtoHdr) + 1;   //+2-1? 20170808  ������ݳ��Ȱ�������֡ͷ֡β��������CRC 
		pst_Hdr->uc_LenL = us_MsgLen&0xFF;
		pst_Hdr->uc_LenH = (us_MsgLen>>8)&0xFF;
		us_MsgLen = us_MsgLen -2;
		
		us_Crc = getCrc(auc_TmpBuf, us_MsgLen);
		
		auc_TmpBuf[us_MsgLen + 1] = us_Crc&0xFF;
		auc_TmpBuf[us_MsgLen] = (us_Crc>>8)&0xFF; 
		
		uc_Ret = dev_Write(MAJOR_PROTO_DEV,PROTO_DEV_CHAN1 + uc_Port , (char*)auc_TmpBuf, (int32_t)(us_MsgLen + 2), &us_Err);
	}

	return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  int32_t ProtoMod_Rev
*         Protoģ���
* @param  IN uint8_t uc_Port ���ӿڱ��,1-4��Ч;
* @param  OUT uint8_t *puc_Cmd������������  
* @param  OUT uint8_t auc_Buf[]�����ݷ�����;	
* @param  IN uint8_t uc_Len:���ݷ���������;
* @return int32_t:���ض�ȡ���ݳ���
*/
/*****************************************************************************/
uint16_t ProtoMod_Rev(IN uint8_t uc_Port, OUT uint8_t *puc_Cmd ,OUT uint8_t auc_Buf[], IN uint16_t us_Len)
{   						/*��Ӧ�ò���ȣ��ӽṹ��gast_ProtoFrame�õ�����������ݵ�auc_Buf�У����*puc_Cmd   20170808*/
	uint16_t   us_Ret = 0;
	
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert(uc_Port > PROTO_CHAN_MAX);
	
	SOS_Assert(NULLS == auc_Buf);
	
	SOS_Assert(NULLS == puc_Cmd);
	uc_Port = uc_Port -1;
	if ( 0 < gast_ProtoFrame[uc_Port].us_Len )
	{
		if ( us_Len > gast_ProtoFrame[uc_Port].us_Len )
		{
			memcpy(auc_Buf, gast_ProtoFrame[uc_Port].auc_Buf,gast_ProtoFrame[uc_Port].us_Len );
			us_Ret = gast_ProtoFrame[uc_Port].us_Len;
			*puc_Cmd = gast_ProtoFrame[uc_Port].uc_Cmd;
		}
		else
		{
			us_Ret = 0;
		}
		gast_ProtoFrame[uc_Port].us_Len = 0;
		gast_ProtoFrame[uc_Port].uc_Cmd = 0;
	}
	else
	{
		us_Ret = 0;
	}
	return us_Ret;	
}

/*****************************************************************************/
/**
* @brief  void ProtoMod_RevHandle
*         ���Э��ģ����մ���ִ��      ���ڽ���Ҳ��PollingScheduleģʽ
* @param  void
* @return void
*/
/*****************************************************************************/ 
void ProtoMod_RevHandle(void)   /*�Ӵ��ڶ�ȡ���ݲ����ո�ʽ����������gast_ProtoFrame�ṹ���� 20170808*/
{
	int32_t  i_Ret = -1;
	uint16_t us_Err = 0;
	uint8_t  uc_Cnt = 0;
	uint8_t  auc_Buf[FRAME_PROTO_LEN_MAX];
	SProtoHdr *pst_Hdr = NULLS;
	/*��ѯ���Э����ӿڽ�������֡��ȡ*/
	for ( uc_Cnt = 0; uc_Cnt < PROTO_CHAN_MAX; uc_Cnt++ )    /*��ѯ�ĸ����ڽ��� 20170808*/
	{	/*�Ӱ��Э��ӿڶ�Ӧ�Ĵ����ж����ݣ�������д����ն�����*/
		i_Ret = dev_Read(MAJOR_PROTO_DEV, PROTO_DEV_CHAN1 + uc_Cnt, (char*)auc_Buf, (int32_t)FRAME_PROTO_LEN_MAX, &us_Err);
		
		if (i_Ret <= 0)
		{
		}
		else
		{
			WriteProtoRevQueue(uc_Cnt, auc_Buf, i_Ret);     /*���ݴ�auc_Buf��д��gast_ProtoRevQueue������ 20170808*/
			/*�Խ��ն��н��н�������ȡ����֡*/
			i_Ret = DrawFrameFromQueue(&gast_ProtoRevQueue[uc_Cnt],auc_Buf, FRAME_PROTO_LEN_MAX);  /*�ҵ�һ֡���ݣ����������ݳ��� 20170808*/
			/*�����ȡ����֡���ݽ�����д�����ϲ�ӿ�������*/
			if (i_Ret > (int32_t)(sizeof(SProtoHdr) + 1))
			{
				pst_Hdr = (SProtoHdr *)auc_Buf;
				gast_ProtoFrame[uc_Cnt].uc_Cmd = pst_Hdr->uc_CmdNum ;
				gast_ProtoFrame[uc_Cnt].ui_Cnt++;
				gast_ProtoFrame[uc_Cnt].us_Len = i_Ret - sizeof(SProtoHdr) - 1;      /*��Ч���ݸ���ȥ��β��Crc 20170808*/
				memcpy(gast_ProtoFrame[uc_Cnt].auc_Buf, pst_Hdr->auc_Data, gast_ProtoFrame[uc_Cnt].us_Len);
			}
			else
			{
				
			} 
		}
	}
}

/*****************************************************************************/
/**
* @brief  uint8_t  WriteProtoRevQueue
*         ģ��������ݶ���д���� 
* @param  IN uint8_t uc_Port ���ӿڱ��,1-4��Ч;
* @param  IN uint8_t auc_Buf[]��д������;	
* @param  IN uint16_t us_Len:д�����ݳ���;
* @return int16_t:���ص�����֡����
*/
/*****************************************************************************/
static uint8_t  WriteProtoRevQueue(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{/*��auc_Buf������д��gast_ProtoRevQueue������ 20170808*/
	uint16_t us_Cnt = 0;
	
	SOS_Assert(uc_Port > PROTO_CHAN_MAX);
	
	SOS_Assert(NULLS == auc_Buf);
	/*������ݽ��ն�����Ч��*/
	us_Cnt = (gast_ProtoRevQueue[uc_Port].us_Tail + PROTO_REV_QUEUE_LEN          /*us_Cnt�ö���Ŀǰ�����ݸ��� = Count 20170808*/
			  - gast_ProtoRevQueue[uc_Port].us_Head)&PROTO_REV_QUEUE_LEN_MASK;
				
	SOS_Assert(us_Cnt != (gast_ProtoRevQueue[uc_Port].us_Count&PROTO_REV_QUEUE_LEN_MASK))  /*charge 20170808*/
	/*������д������У����������ɾ�������ݼ����洢*/
	for ( us_Cnt = 0; us_Cnt < us_Len; us_Cnt++ )
	{
		gast_ProtoRevQueue[uc_Port].auc_Buf[gast_ProtoRevQueue[uc_Port].us_Tail] = auc_Buf[us_Cnt];
		gast_ProtoRevQueue[uc_Port].us_Tail = (gast_ProtoRevQueue[uc_Port].us_Tail + 1)&PROTO_REV_QUEUE_LEN_MASK;
		
		if (PROTO_REV_QUEUE_LEN == gast_ProtoRevQueue[uc_Port].us_Count)
		{	/*������һ���жϾ���ѭ�������Ѿ����ˣ���ε����ݴ洢�����˵�0������ 20170808*/
			gast_ProtoRevQueue[uc_Port].us_Head = (gast_ProtoRevQueue[uc_Port].us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;	 	
		}
		else
		{
			gast_ProtoRevQueue[uc_Port].us_Count++;
		}
	}
	return SYS_OK;
}

/*****************************************************************************/
/**
* @brief  int16_t DrawFrameFromQueue
*         ����Э��֡��ʽ����ȡ����֡ 
* @param  IN SProtoRevQueue *pst_Queue���ֽ������ն���
* @param  OUT uint8_t auc_Buf[]:����֡������;		
* @param  IN uint16_t us_Len:����֡����������;
* @return int16_t:���ص�����֡����
*/
/*****************************************************************************/
static int16_t DrawFrameFromQueue(IN SProtoRevQueue *pst_Queue,OUT uint8_t auc_Buf[], IN uint16_t us_Len) 
{
	uint16_t  us_Head = 0;
	uint16_t  us_MsgLen = 0; 
	int16_t   sh_Ret = 0;
	uint8_t   uc_Flag = SYS_NO;
	
	SOS_Assert(NULLS == pst_Queue);
	
	SOS_Assert(NULLS == auc_Buf);

	SOS_Assert(0 == us_Len);

	us_Head = pst_Queue->us_Head;
	/*������ݽ��ն��������ݸ���������С֡���ݳ��Ƚ�������֡��ȡ���˳�����Ϊ���������ݸ���С��֡���Ȼ���ȡ��֡����*/
	while((pst_Queue->us_Count > (sizeof(SProtoHdr) + 1)) && (uc_Flag == SYS_NO))
	{	/*����֡ͷ���粻��ɾ������*/	
		if (FLAG_SFD_CMD == pst_Queue->auc_Buf[us_Head] )
		{
			us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			
			if (FLAG_SFL_CMD == pst_Queue->auc_Buf[us_Head])
			{	/*��ȡ֡������Ϣ��������������֡��Ч�Լ��*/
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
				us_MsgLen = pst_Queue->auc_Buf[us_Head];
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
				us_MsgLen = (us_MsgLen<<8)|pst_Queue->auc_Buf[us_Head];
				/*��������֡��Ч�Լ�⣬����ȡ����֡*/
				sh_Ret = GetProtoFrame(pst_Queue, us_MsgLen ,auc_Buf, us_Len );
				
				if (sh_Ret < 0 )  /*˫֡ͷ������ȷ 20170808*/
				{   
					pst_Queue->us_Head = (pst_Queue->us_Head + 2)&PROTO_REV_QUEUE_LEN_MASK;
					pst_Queue->us_Count = pst_Queue->us_Count - 2;
					us_Head = pst_Queue->us_Head;
					pst_Queue->ui_ErrCnt++;
				}
				else
				{   /*�ҵ�����֡ 20170808*/
					pst_Queue->us_Head = (pst_Queue->us_Head + sh_Ret)&PROTO_REV_QUEUE_LEN_MASK;
					pst_Queue->us_Count = pst_Queue->us_Count - sh_Ret;
					us_Head = pst_Queue->us_Head;
					uc_Flag = SYS_YES;
					pst_Queue->ui_Cnt++;
				}
			}
			else   /*�ڶ�֡ͷ����ȷ 20170808*/
			{
				pst_Queue->us_Head = (pst_Queue->us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			    pst_Queue->us_Count = pst_Queue->us_Count - 1;	
			    us_Head = pst_Queue->us_Head;	
			    pst_Queue->ui_ErrCnt++;
			}
		}
		else   /*��һ֡ͷ����ȷ 20170808 */
		{
			pst_Queue->us_Head = (pst_Queue->us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			pst_Queue->us_Count = pst_Queue->us_Count - 1;	
			us_Head = pst_Queue->us_Head;	
			pst_Queue->ui_ErrCnt++;
		}	
	}

	return sh_Ret;		
} 

/*****************************************************************************/
/**
* @brief  int16_t  GetProtoFrame
*         ��Э��֡��ʽ����ȡ����֡ 
* @param  IN SProtoRevQueue *pst_Queue���ֽ������ն���
* @param  IN uint16_t us_MsgLen������֡���ȣ����ֽ����л�ȡ������֡������Ϣ;
* @param  OUT uint8_t auc_Buf[]:����֡������;		
* @param  IN uint16_t us_Len:����֡����������;
* @return int16_t:���ص�����֡����
*/
/*****************************************************************************/
static int16_t  GetProtoFrame(IN SProtoRevQueue *pst_Queue, IN uint16_t us_MsgLen ,OUT uint8_t auc_Buf[], IN uint16_t us_Len )
{   /*�����ݴӶ�����ȡ��auc_Buf�У���ȡ�����ݰ���β����Crc���õ������ݳ���us_MsgLen�����ݵ��ܳ��� 20170808*/
	uint16_t  us_Head = 0;
	uint16_t  us_Cnt = 0;
	uint16_t  us_Crc = 0; 
	int16_t   sh_Ret = -1;
	
	SOS_Assert(NULLS == pst_Queue);
	
	SOS_Assert(NULLS == auc_Buf);

	SOS_Assert(0 == us_Len);
	/*�����Ϣ֡�����Ƿ���Ч������֡����Ӧ������С֡���Ⱥ�С�ڵ��ڽ���������*/   /*us_MsgLen < us_Len ֡����Ӧ��С�ڽ������ݳ��� 20170808*/
	if ((us_MsgLen < (sizeof(SProtoHdr) + 1)) || (us_MsgLen > us_Len) )
	{
		sh_Ret = -1;
	}
	else
	{	/*������ն�����û����֡�����Ȳ���������֡��ȡ����������н�������֡��Ч�Լ��*/
		if (pst_Queue->us_Count < us_MsgLen)
		{
			sh_Ret = 0;
		}	
		else
		{
			us_Head = pst_Queue->us_Head ;
						
			for (us_Cnt = 0; us_Cnt <  us_MsgLen; us_Cnt++)   /*us_MsgLen ������������ֽڵ�Crc 20170808*/
			{
				auc_Buf[us_Cnt] = pst_Queue->auc_Buf[us_Head];
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			}	
			us_Crc = auc_Buf[us_MsgLen-1]|(auc_Buf[us_MsgLen-2]<<8);
			/*�������֡��Ч��������֡���ȣ����򷵻ش������ϲ�ɾ��֡ͷ��Ϣ������������֡*/			
			if (getCrc(auc_Buf ,us_MsgLen - 2) == us_Crc)
			{
				sh_Ret = us_MsgLen;
			}
			else
			{
				sh_Ret = -1;
			}
		}	
	}	
	return sh_Ret;
}
