/******************************************************************************/
/**
*@file    Proto.c     
*@brief   板间协议模块                   
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
/**@brief  板间协议串口字节流接收队列                                                   */
/*****************************************************************************/ 
static  SProtoRevQueue  gast_ProtoRevQueue[PROTO_CHAN_MAX];
 
/*****************************************************************************/
/**@brief  板间协议命令帧存储结构                                                       */
/*****************************************************************************/
static  SProtoFrame      gast_ProtoFrame[PROTO_CHAN_MAX];

/*****************************************************************************/
/**@brief  局部函数声明                                                                */
/*****************************************************************************/
static uint8_t  WriteProtoRevQueue(IN uint8_t uc_Port,IN uint8_t auc_Buf[], IN uint16_t us_Len);

static int16_t DrawFrameFromQueue(IN SProtoRevQueue *pst_Queue,OUT uint8_t auc_Buf[], IN uint16_t us_Len) ;

static int16_t GetProtoFrame(IN SProtoRevQueue *pst_Queue, IN uint16_t us_MsgLen ,OUT uint8_t auc_Buf[], IN uint16_t us_Len );

/*****************************************************************************/
/**
* @brief  ProtoMod_Open
*         Proto模块打开操作，对模块数据进行初始化和打开模块对应物理通道
* @param  void
* @return uint8_t : SYS_OK----成功, SYS_ERR----失败
*/
/*****************************************************************************/
uint8_t  ProtoMod_Open(void)         /*打开四个串口 20170808*/
{
	uint8_t uc_Ret = SYS_ERR;
	uint16_t us_Err = 0;
	
	memset(gast_ProtoRevQueue, 0 ,sizeof(gast_ProtoRevQueue));
	
	memset(gast_ProtoFrame, 0, sizeof(gast_ProtoFrame));
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN1,1, &us_Err);     /*打开串口1 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);

	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN2,1, &us_Err);     /*打开串口2 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN3,1, &us_Err);     /*串口3 20170808*/    
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	uc_Ret = dev_Open(MAJOR_PROTO_DEV, PROTO_DEV_CHAN4,1, &us_Err);    /*串口4 20170808*/
	
	SOS_Assert(SYS_OK != uc_Ret);
	
	return SYS_OK;
}

/*****************************************************************************/
/**
* @brief  int32_t ProtoMod_Send
*         Proto模块发送操作接口
* @param  IN uint8_t uc_Port ：接口编号,1-4有效;
* @param  IN uint8_t uc_Cmd：发送命令编号  
* @param  IN uint8_t auc_Buf[]：写入数据;	
* @param  IN uint16_t us_Len: 写入数据长度;
* @return uint8_t : SYS_OK----成功, SYS_ERR----失败
*/
/*****************************************************************************/ 
uint8_t  ProtoMod_Send(IN uint8_t uc_Port, IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*对要通过串口发送给QJK的数据在auc_TmpBuf数据块进行打包，并发送 20170808*/
	uint16_t us_Crc = 0;
	uint16_t us_MsgLen = 0;
	uint16_t us_Err = 0;
	uint8_t  uc_Ret = SYS_OK;
	SProtoHdr *pst_Hdr = NULLS;
	
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert(NULLS == auc_Buf);
	uc_Port = uc_Port -1;
	if ((us_Len + sizeof(SProtoHdr) + 1) >  1700)	/*打包一帧数据，一帧数据不能超过1700 20170808*/
	{
		uc_Ret = SYS_ERR;	
	}
	else
	{
		pst_Hdr = (SProtoHdr*)auc_TmpBuf;                   /*auc_TmpBuf 全局变量类似一个内存块，这样处理起来数据快速理解这样，后面查下 20170808*/
		pst_Hdr->uc_SFD = FLAG_SFD_CMD;
		pst_Hdr->uc_SFL = FLAG_SFL_CMD;
		pst_Hdr->auc_NSNH = 0;
		pst_Hdr->auc_NSNL = 0;
		pst_Hdr->uc_CmdNum = uc_Cmd;
		
		memcpy(pst_Hdr->auc_Data,(char*)auc_Buf,us_Len);
	
		us_MsgLen =  us_Len + sizeof(SProtoHdr) + 1;   //+2-1? 20170808  这个数据长度包含数了帧头帧尾，和最后的CRC 
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
*         Proto模块读
* @param  IN uint8_t uc_Port ：接口编号,1-4有效;
* @param  OUT uint8_t *puc_Cmd：返回命令编号  
* @param  OUT uint8_t auc_Buf[]：数据返回区;	
* @param  IN uint8_t uc_Len:数据返回区长度;
* @return int32_t:返回读取数据长度
*/
/*****************************************************************************/
uint16_t ProtoMod_Rev(IN uint8_t uc_Port, OUT uint8_t *puc_Cmd ,OUT uint8_t auc_Buf[], IN uint16_t us_Len)
{   						/*在应用层调度，从结构体gast_ProtoFrame得到解析后的数据到auc_Buf中，命令到*puc_Cmd   20170808*/
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
*         板间协议模块接收处理执行      串口接受也是PollingSchedule模式
* @param  void
* @return void
*/
/*****************************************************************************/ 
void ProtoMod_RevHandle(void)   /*从串口读取数据并按照格式解析出来到gast_ProtoFrame结构体中 20170808*/
{
	int32_t  i_Ret = -1;
	uint16_t us_Err = 0;
	uint8_t  uc_Cnt = 0;
	uint8_t  auc_Buf[FRAME_PROTO_LEN_MAX];
	SProtoHdr *pst_Hdr = NULLS;
	/*轮询板间协议各接口进行命令帧提取*/
	for ( uc_Cnt = 0; uc_Cnt < PROTO_CHAN_MAX; uc_Cnt++ )    /*轮询四个串口接受 20170808*/
	{	/*从板间协议接口对应的串口中读数据，将数据写入接收队列中*/
		i_Ret = dev_Read(MAJOR_PROTO_DEV, PROTO_DEV_CHAN1 + uc_Cnt, (char*)auc_Buf, (int32_t)FRAME_PROTO_LEN_MAX, &us_Err);
		
		if (i_Ret <= 0)
		{
		}
		else
		{
			WriteProtoRevQueue(uc_Cnt, auc_Buf, i_Ret);     /*数据从auc_Buf中写入gast_ProtoRevQueue队列中 20170808*/
			/*对接收队列进行解析，提取命令帧*/
			i_Ret = DrawFrameFromQueue(&gast_ProtoRevQueue[uc_Cnt],auc_Buf, FRAME_PROTO_LEN_MAX);  /*找到一帧数据，并返回数据长度 20170808*/
			/*如果提取到整帧数据将数据写入与上层接口数据区*/
			if (i_Ret > (int32_t)(sizeof(SProtoHdr) + 1))
			{
				pst_Hdr = (SProtoHdr *)auc_Buf;
				gast_ProtoFrame[uc_Cnt].uc_Cmd = pst_Hdr->uc_CmdNum ;
				gast_ProtoFrame[uc_Cnt].ui_Cnt++;
				gast_ProtoFrame[uc_Cnt].us_Len = i_Ret - sizeof(SProtoHdr) - 1;      /*有效数据个数去掉尾部Crc 20170808*/
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
*         模块接收数据队列写操作 
* @param  IN uint8_t uc_Port ：接口编号,1-4有效;
* @param  IN uint8_t auc_Buf[]：写入数据;	
* @param  IN uint16_t us_Len:写入数据长度;
* @return int16_t:返回的数据帧长度
*/
/*****************************************************************************/
static uint8_t  WriteProtoRevQueue(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{/*将auc_Buf的数据写入gast_ProtoRevQueue队列中 20170808*/
	uint16_t us_Cnt = 0;
	
	SOS_Assert(uc_Port > PROTO_CHAN_MAX);
	
	SOS_Assert(NULLS == auc_Buf);
	/*检测数据接收队列有效性*/
	us_Cnt = (gast_ProtoRevQueue[uc_Port].us_Tail + PROTO_REV_QUEUE_LEN          /*us_Cnt该队列目前的数据个数 = Count 20170808*/
			  - gast_ProtoRevQueue[uc_Port].us_Head)&PROTO_REV_QUEUE_LEN_MASK;
				
	SOS_Assert(us_Cnt != (gast_ProtoRevQueue[uc_Port].us_Count&PROTO_REV_QUEUE_LEN_MASK))  /*charge 20170808*/
	/*将数据写入队列中，如果队列满删除老数据继续存储*/
	for ( us_Cnt = 0; us_Cnt < us_Len; us_Cnt++ )
	{
		gast_ProtoRevQueue[uc_Port].auc_Buf[gast_ProtoRevQueue[uc_Port].us_Tail] = auc_Buf[us_Cnt];
		gast_ProtoRevQueue[uc_Port].us_Tail = (gast_ProtoRevQueue[uc_Port].us_Tail + 1)&PROTO_REV_QUEUE_LEN_MASK;
		
		if (PROTO_REV_QUEUE_LEN == gast_ProtoRevQueue[uc_Port].us_Count)
		{	/*作这样一次判断就是循环队列已经满了，这次的数据存储覆盖了第0个数据 20170808*/
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
*         解析协议帧格式，获取数据帧 
* @param  IN SProtoRevQueue *pst_Queue：字节流接收队列
* @param  OUT uint8_t auc_Buf[]:数据帧返回区;		
* @param  IN uint16_t us_Len:数据帧返回区长度;
* @return int16_t:返回的数据帧长度
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
	/*如果数据接收队列中数据个数大于最小帧数据长度进行数据帧提取，退出条件为队列中数据个数小于帧长度或提取到帧数据*/
	while((pst_Queue->us_Count > (sizeof(SProtoHdr) + 1)) && (uc_Flag == SYS_NO))
	{	/*查找帧头，如不是删除数据*/	
		if (FLAG_SFD_CMD == pst_Queue->auc_Buf[us_Head] )
		{
			us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			
			if (FLAG_SFL_CMD == pst_Queue->auc_Buf[us_Head])
			{	/*获取帧长度信息，继续进行数据帧有效性检测*/
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
				us_MsgLen = pst_Queue->auc_Buf[us_Head];
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
				us_MsgLen = (us_MsgLen<<8)|pst_Queue->auc_Buf[us_Head];
				/*进行数据帧有效性检测，并提取数据帧*/
				sh_Ret = GetProtoFrame(pst_Queue, us_MsgLen ,auc_Buf, us_Len );
				
				if (sh_Ret < 0 )  /*双帧头都不正确 20170808*/
				{   
					pst_Queue->us_Head = (pst_Queue->us_Head + 2)&PROTO_REV_QUEUE_LEN_MASK;
					pst_Queue->us_Count = pst_Queue->us_Count - 2;
					us_Head = pst_Queue->us_Head;
					pst_Queue->ui_ErrCnt++;
				}
				else
				{   /*找到数据帧 20170808*/
					pst_Queue->us_Head = (pst_Queue->us_Head + sh_Ret)&PROTO_REV_QUEUE_LEN_MASK;
					pst_Queue->us_Count = pst_Queue->us_Count - sh_Ret;
					us_Head = pst_Queue->us_Head;
					uc_Flag = SYS_YES;
					pst_Queue->ui_Cnt++;
				}
			}
			else   /*第二帧头不正确 20170808*/
			{
				pst_Queue->us_Head = (pst_Queue->us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			    pst_Queue->us_Count = pst_Queue->us_Count - 1;	
			    us_Head = pst_Queue->us_Head;	
			    pst_Queue->ui_ErrCnt++;
			}
		}
		else   /*第一帧头不正确 20170808 */
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
*         测协议帧格式，获取数据帧 
* @param  IN SProtoRevQueue *pst_Queue：字节流接收队列
* @param  IN uint16_t us_MsgLen：数据帧长度，从字节流中获取的数据帧长度信息;
* @param  OUT uint8_t auc_Buf[]:数据帧返回区;		
* @param  IN uint16_t us_Len:数据帧返回区长度;
* @return int16_t:返回的数据帧长度
*/
/*****************************************************************************/
static int16_t  GetProtoFrame(IN SProtoRevQueue *pst_Queue, IN uint16_t us_MsgLen ,OUT uint8_t auc_Buf[], IN uint16_t us_Len )
{   /*将数据从队列提取到auc_Buf中，提取的数据包含尾部的Crc，得到的数据长度us_MsgLen是数据的总长度 20170808*/
	uint16_t  us_Head = 0;
	uint16_t  us_Cnt = 0;
	uint16_t  us_Crc = 0; 
	int16_t   sh_Ret = -1;
	
	SOS_Assert(NULLS == pst_Queue);
	
	SOS_Assert(NULLS == auc_Buf);

	SOS_Assert(0 == us_Len);
	/*检测信息帧长度是否有效，数据帧长度应满足最小帧长度和小于等于接收区长度*/   /*us_MsgLen < us_Len 帧长度应该小于接受数据长度 20170808*/
	if ((us_MsgLen < (sizeof(SProtoHdr) + 1)) || (us_MsgLen > us_Len) )
	{
		sh_Ret = -1;
	}
	else
	{	/*如果接收队列内没有整帧数据先不进行数据帧提取操作，如果有进行数据帧有效性检测*/
		if (pst_Queue->us_Count < us_MsgLen)
		{
			sh_Ret = 0;
		}	
		else
		{
			us_Head = pst_Queue->us_Head ;
						
			for (us_Cnt = 0; us_Cnt <  us_MsgLen; us_Cnt++)   /*us_MsgLen 包含最后两个字节的Crc 20170808*/
			{
				auc_Buf[us_Cnt] = pst_Queue->auc_Buf[us_Head];
				us_Head = (us_Head + 1)&PROTO_REV_QUEUE_LEN_MASK;
			}	
			us_Crc = auc_Buf[us_MsgLen-1]|(auc_Buf[us_MsgLen-2]<<8);
			/*如果数据帧有效返回数据帧长度，否则返回错误由上层删除帧头信息继续查找数据帧*/			
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
