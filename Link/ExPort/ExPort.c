/******************************************************************************/
/**
*@file    ExPort.c      
*@brief   外部接口模块                  
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/ 
#include  "Typedef.h"
#include  "Driver.h"
#include  "Config.h"
#include  "dev_Timer.h"
#include  "ExPort.h"
#include  "RamBuf.h"
#include  <string.h>

uint8_t   gauc_ExportRam[EXPORT_RAM_LEN];

/*****************************************************************************/
/**@brief  外部接口设备类型                                                            */
/*****************************************************************************/  

/*****************************************************************************/
/**@brief  外部接口模块接口信息                        					         */
/*****************************************************************************/
static  SExPortInfo      gast_ExPortInfo[EXPORT_PORT_NUM];

/*****************************************************************************/
/**@brief  局部函数声明                                					         */
/*****************************************************************************/

static  void  ExPortMod_Check(void);

static  int16_t  ExPort_FindPortNum(IN uint32_t  ui_IpAddr);

static  int16_t  ReadExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len);

static  uint8_t  WriteExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len);
/*****************************************************************************/
/**
* @brief  void ExPortMod_Init
*         外部模块初始化，完成模块数据清零
* @param  void
* @return void OK
*/
/*****************************************************************************/
void ExPortMod_Init(void)
{
	memset(gast_ExPortInfo, 0, sizeof(gast_ExPortInfo));
	
	InitBuf(gauc_ExportRam, EXPORT_RAM_LEN, 200);
}
 /**
 * @brief  ExPortMod_Open,外部接收模块打开操作
 * @param  IN uint8_t uc_Port:端口号，1-2有效
 * 		   IN uint8_t uc_Major：端口主设备号，1-6有效；
 * 		   IN uint8_t uc_Minor：端口次设备号，1-4有效；
 * 		   IN uint8_t uc_DataType：数据类型 ，1-3有效;
 * @return uint8_t :SYS_OK/SYS_ERR---成功/失败 OK
 * @see 
 */ 

uint8_t  ExPortMod_OpenNet(IN uint8_t uc_Port, IN SSockAddr  *pst_DestAddr, uint16_t us_SndPriod, uint16_t us_Len)
{/*初始化全局变量gast_ExPortInfo<用来指示目的地址目的IP端口等信息>和申请的内存块 20170808*/
	uint8_t uc_Cnt = 0;
	int16_t sh_ID = -1;
	ExQueue *pst_ExpQueue = NULLS;
	
 	SOS_Assert(0 == uc_Port);
 
 	SOS_Assert(uc_Port > EXPORT_PORT_NUM);
 	
 	SOS_Assert(NULLS == pst_DestAddr);	
 	
 	uc_Port = uc_Port -1;
	
 	/*清空接口信息数据*/
 	memset(&gast_ExPortInfo[uc_Port],0, sizeof (gast_ExPortInfo[uc_Port]));
 	
 	/*设置外部接口映射地址为目的地址和目的端口号用于网络设备数据与接口关联*/	
 	gast_ExPortInfo[uc_Port].st_NetMap.ui_IpAddr = pst_DestAddr->sin_addr;
 	gast_ExPortInfo[uc_Port].st_NetMap.us_Port = pst_DestAddr->sin_port;
 	gast_ExPortInfo[uc_Port].st_NetMap.us_SndPerid = us_SndPriod;

	for(uc_Cnt = 0; uc_Cnt < REV_SEC_MAX; uc_Cnt++)
	{
		sh_ID = CreatObj(gauc_ExportRam, us_Len + sizeof(ExQueue) + 3);	
		
		SOS_Assert(sh_ID <= 0);
		
		gast_ExPortInfo[uc_Port].ast_RevBuf[uc_Cnt].us_RevId = sh_ID;
		
		pst_ExpQueue = (ExQueue *)GetObjAddr(gauc_ExportRam,sh_ID);
		
		SOS_Assert(pst_ExpQueue == NULLS);
		
		memset((uint8_t*)pst_ExpQueue, 0, us_Len + sizeof(ExQueue));
		
		pst_ExpQueue->us_QueueLen = us_Len;
	}
	
 	gast_ExPortInfo[uc_Port].uc_Init = SYS_YES;

 	
	return  SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint8_t  ExPortMod_Send
*         外部接收模块写操作，用于将数据写入外部接口对应物理接口
* @param  IN uint8_t uc_Port:端口号，1-4有效
* @param  IN uint8_t auc_Buf[]:发送数据;
* @param  IN uint16_t us_Len:发送数据长度;
* @return uint8_t：SYS_YES---操作正常，SYS_NO---操作失败 OK
*/
/*****************************************************************************/
uint8_t  ExPortMod_Send(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*将us_Len个auc_Buf数据通过socket uc_Port发送出去 20170808*/
 	uint8_t  uc_Ret = SYS_OK;
 	uint8_t  uc_SndPort = 0U;
 	
 	SSockAddr  st_SockAddr ;

	SOS_Assert(0 == uc_Port);
 	
	SOS_Assert(uc_Port > EXPORT_PORT_NUM);
	
	SOS_Assert(NULLS == auc_Buf);
	
	uc_Port = uc_Port -1;
	/*如果外部接口已初始化和发送数据长度不为零*/
	if ((SYS_OK == gast_ExPortInfo[uc_Port].uc_Init)&&(us_Len >0))
	{		/*如果为网络设备将数据写入网络接口中*/
	
		st_SockAddr.sin_addr = gast_ExPortInfo[uc_Port].st_NetMap.ui_IpAddr;
		st_SockAddr.sin_port = gast_ExPortInfo[uc_Port].st_NetMap.us_Port;
		uc_SndPort = uc_Port%EXPORT_SOCKE;
		uc_Ret = W5300_SockSnd(0, uc_SndPort, &st_SockAddr, auc_Buf,  us_Len);
	}
	else
	{
		uc_Ret = SYS_ERR;
	}		
 	return uc_Ret;
}
  
 /*****************************************************************************/
/**
* @brief  int16_t  ExPortMod_Rev
*         外部接收模块读操作，向数据区返回的数据结构为数据长度+数据帧，返回的数据区可包括多帧
* @param  IN uint8_t uc_Port:端口号，1-4有效
* @param  IN uint8_t uc_Sect：读取数据帧存储区号
* @param  OUT uint8_t auc_Buf[]：接收数据区;
* @param  IN uint16_t us_Len:接收数据区长度;
* @return int16_t : 0---无数据,>0---接收数据长度,<0----接收数据区小，无法读取 OK
*/ 
/*****************************************************************************/
int16_t  ExPortMod_Rev(IN uint8_t uc_Port, IN uint8_t uc_Sect, OUT uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*从内存块中将数据读取到auc_Buf中 20170808*/
 	int16_t sh_ID = -1;
 	int16_t sh_Ret = -1;
 	ExQueue *pst_ExpQueue = NULLS;
 	
 	SOS_Assert((0 == uc_Port)||(uc_Port > EXPORT_PORT_NUM));
	
	SOS_Assert((0 == uc_Sect)||(uc_Sect > REV_SEC_MAX));
	
	SOS_Assert(NULLS == auc_Buf);
	
	uc_Port = uc_Port -1;
	uc_Sect = uc_Sect -1;
	/*如果外部接口已初始化进行读操作*/
	if(SYS_OK == gast_ExPortInfo[uc_Port].uc_Init)
	{
		sh_ID = gast_ExPortInfo[uc_Port].ast_RevBuf[uc_Sect].us_RevId;
		
		pst_ExpQueue = (ExQueue *)GetObjAddr(gauc_ExportRam,sh_ID);
		
		SOS_Assert(pst_ExpQueue == NULLS);
		/*检查数据帧存储队列有效性*/
		sh_Ret = ReadExQueue(pst_ExpQueue, auc_Buf, us_Len);

	}
	/*返回读取数据长度*/
 	return sh_Ret;
}

/*****************************************************************************/
/**
* @brief  uint32_t  ExPort_Ioctl
*         外部接口控制操作
* @param  IN uint8_t uc_Port：端口号，1-4有效
* @param  IN uint8_t  uc_Arg：操作码，STATE_COM_EXPORT--获取接口通信状态，
* 								ERRCNT_EXPORT--获取外部接口接收队列错误计数；
* 								CNT_EXPORT--获取外部接口接收队列接收计数
* @return uint8_t：SYS_YES---操作正常，否则宕机 OK
*/
/*****************************************************************************/
uint32_t  ExPort_Ioctl(IN uint8_t uc_Port, IN uint8_t  uc_Arg)
{	/*得到外部接口的通信状态 20170808*/
	uint32_t  ui_Ret = 0;
	
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert(uc_Port > EXPORT_PORT_NUM);
	uc_Port = uc_Port -1;
	switch(uc_Arg)	
	{
		case STATE_COM_EXPORT:
		{
			ui_Ret = gast_ExPortInfo[uc_Port].uc_ComState;
			break;	
		}	
		case  ERRCNT_EXPORT:
		{
			
			break;	
		}
		case CNT_EXPORT:
		{
		
			break;
		}
		default:
		{
			ui_Ret = 0;
			break;	
		}
	}
	
	return ui_Ret;
}

 /*****************************************************************************/
/**
* @brief  void ExPortMod_RevHandle
*         外部接口模块接收处理执行
* @param  void
* @return void OK
*/
/*****************************************************************************/
void ExPortMod_RevHandle(void)/*循环调度接受网卡数据，存入内存池中并更新到期的数据块 20170808*/
{
	uint8_t  uc_Cnt = 0;
	uint8_t  auc_Buf[EXPORT_REV_FRAME_LEN];
	int32_t  i_Ret = 0;
	int32_t  i_Port = -1;
	int16_t  sh_ID = -1;
	uint32_t ui_IpAddr = 0;
 	ExQueue *pst_ExQueue = NULLS;
 	
	for (uc_Cnt = 0; uc_Cnt < EXPORT_SOCKE; uc_Cnt++)
	{
		i_Ret =	W5300_SockRev(0, uc_Cnt, &ui_IpAddr, auc_Buf, EXPORT_REV_FRAME_LEN); /*从网卡0循环socket读取数据到auc_Buf中*/
		
		if (i_Ret > 0)
		{
			i_Port = ExPort_FindPortNum(ui_IpAddr);
			
			if (i_Port >= 0)
			{
				if (gast_ExPortInfo[i_Port].uc_Init == SYS_YES)              /*i_Port最终是通过配置数据得到的 20170808*/
				{
					
					for(uc_Cnt = 0; uc_Cnt < REV_SEC_MAX; uc_Cnt++)
					{
						sh_ID = gast_ExPortInfo[i_Port].ast_RevBuf[uc_Cnt].us_RevId;
		
						pst_ExQueue = (ExQueue *)GetObjAddr(gauc_ExportRam,sh_ID);
						
						SOS_Assert(pst_ExQueue==NULLS);
						
						WriteExQueue(pst_ExQueue,auc_Buf, i_Ret);
						
						gast_ExPortInfo[i_Port].ast_RevBuf[uc_Cnt].ui_Timer = getTick();  
					}
					gast_ExPortInfo[i_Port].ui_Cnt++;
					gast_ExPortInfo[i_Port].uc_ComState = SYS_YES;
					gast_ExPortInfo[i_Port].ui_Timer = getTick();              /*更新数据状态 20170808*/
				}
			}
		}
	}
	
	ExPortMod_Check();	                /*检测通信状态，2s没接受正常数据通信中断，清除时间到达2s的内存块的数据*/
}

/*****************************************************************************/
/**
* @brief  void  ExPortMod_Check
*         检测外部接口模块各接口通信状态
* @param  void
* @return void OK
*/
/*****************************************************************************/
static  void  ExPortMod_Check(void)   /*检测通信状态，清楚时间到达2s的内存块的数据*/
{
	uint8_t  uc_Cnt = 0;
	uint8_t  i = 0;
	uint16_t us_ID = 0;
	ExQueue *pst_ExQueue = NULLS;
	
	for (uc_Cnt = 0; uc_Cnt < EXPORT_PORT_NUM; uc_Cnt++)
	{
		if(gast_ExPortInfo[uc_Cnt].uc_Init == SYS_YES)
		{
			if (SYS_YES == IsTimeOut(2000, gast_ExPortInfo[uc_Cnt].ui_Timer))
			{
				gast_ExPortInfo[uc_Cnt].uc_ComState = SYS_NO;	
			}	
			
			for(i = 0; i < REV_SEC_MAX; i++)
			{    /*内存池中的数据生存周期是2s*/
				if (SYS_YES == IsTimeOut(2000, gast_ExPortInfo[uc_Cnt].ast_RevBuf[i].ui_Timer))
				{
					us_ID = gast_ExPortInfo[uc_Cnt].ast_RevBuf[i].us_RevId;
					
					pst_ExQueue = (ExQueue *)GetObjAddr(gauc_ExportRam,us_ID);
					
					SOS_Assert(pst_ExQueue==NULLS);
					
					pst_ExQueue->us_Cnt = 0;
					pst_ExQueue->us_Hdr = 0;
					pst_ExQueue->us_Tail = 0;
				}
			}	
		}	
	}	
}

/*****************************************************************************/
/**
* @brief  uint8_t  ExPort_FindPortNum
*         根据IP地址查询接口映射数据返回外部接口号
* @param  IN uint32_t  ui_IpAddr：输入IP地址
* @return uint8_t：返回外部接口号，1-EXPORT_PORT_NUM OK
*/
/*****************************************************************************/
static  int16_t  ExPort_FindPortNum(IN uint32_t  ui_IpAddr)
{
	uint8_t   uc_Port = 0U;
	int16_t  sh_Ret = -1;
	for(uc_Port = 0; uc_Port < EXPORT_PORT_NUM ; uc_Port++)	        /*从IP得知得到从哪个外部接口得到数据 20170808*/
	{
		if (gast_ExPortInfo[uc_Port].st_NetMap.ui_IpAddr == ui_IpAddr)  /*初始化得到 20170808*/
		{
			sh_Ret = uc_Port;
		}
	}
	return sh_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  WriteExQueue
*         将auc_Buf中的数据存入队列ExQueue中
* @param  IN uint32_t  ui_IpAddr：输入IP地址
* @return uint8_t：返回外部接口号，1-EXPORT_PORT_NUM
*/
/*****************************************************************************/
static  uint8_t  WriteExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len)
{
	int16_t sh_Remain = 0;
	uint16_t us_FrameLen = 0;
	uint16_t us_Head = 0;
	uint8_t  uc_Ret = SYS_NO;
	uint16_t us_Cnt = 0;
	
	SOS_Assert(NULLS == pstExQueue);
	
	SOS_Assert(NULLS == auc_Buf);
	
	sh_Remain = pstExQueue->us_QueueLen - pstExQueue->us_Cnt;     /*循环队列剩余的长度 20170808*/
	
	while((sh_Remain < (us_Len + 2))&&(pstExQueue->us_Cnt > 2))/*如果剩余空间小于写入长度，则删除队列里的一帧数据*/
	{/*需要存储的剩余空间不足以存储一帧数据 20170808*/
		us_Head = pstExQueue->us_Hdr;           /*从队列的数据区提取传输的数据帧结构 20170808*/
		us_FrameLen = pstExQueue->auc_Buf[us_Head];
			
		us_Head = (us_Head + 1)%pstExQueue->us_QueueLen;
		us_FrameLen = (us_FrameLen<<8)|pstExQueue->auc_Buf[us_Head];
		
		us_FrameLen = us_FrameLen + 2;            /*要存入的这帧数据长度 20170808*/
		if (us_FrameLen  <= pstExQueue->us_Cnt)
		{  /*从该循环队列剔除一帧数据 20170808*/
			pstExQueue->us_Hdr = (pstExQueue->us_Hdr +  us_FrameLen)%pstExQueue->us_QueueLen;
			pstExQueue->us_Cnt = pstExQueue->us_Cnt -  us_FrameLen;
		}
		else
		{ /*数据长度比现有数据个数大，出错直接清掉所有的 20170808*/
			pstExQueue->us_Cnt = 0;
			pstExQueue->us_Hdr = 0;
			pstExQueue->us_Tail = 0;
		}
		sh_Remain = pstExQueue->us_QueueLen - pstExQueue->us_Cnt;     /*更新一下剩余区间 20170808*/
	}
	
	if (sh_Remain >= (us_Len + 2))
	{
		pstExQueue->auc_Buf[pstExQueue->us_Tail] = us_Len>>8;               /*先存入帧长度 20170808*/
		pstExQueue->us_Tail = (pstExQueue->us_Tail + 1)% pstExQueue->us_QueueLen;
		pstExQueue->auc_Buf[pstExQueue->us_Tail] = us_Len&0xFF;
		pstExQueue->us_Tail = (pstExQueue->us_Tail + 1)% pstExQueue->us_QueueLen;
		
		for(us_Cnt = 0; us_Cnt < us_Len; us_Cnt++)
		{
			pstExQueue->auc_Buf[pstExQueue->us_Tail] = 	auc_Buf[us_Cnt];
			pstExQueue->us_Tail = (pstExQueue->us_Tail + 1)% pstExQueue->us_QueueLen;
		}
		pstExQueue->us_Cnt = pstExQueue->us_Cnt + us_Len + 2;
		uc_Ret = SYS_YES;
	}
	else
	{
		uc_Ret = SYS_NO;		
	}	
	return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  ExPort_FindPortNum
*         根据IP地址查询接口映射数据返回外部接口号
* @param  IN uint32_t  ui_IpAddr：输入IP地址
* @return uint8_t：返回外部接口号，1-EXPORT_PORT_NUM
*/
/*****************************************************************************/
static  int16_t  ReadExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len)
{										/*将内存块的队列中数据都读取到auc_Buf中 20170808*/
	uint16_t us_FrameLen = 0;
	uint16_t us_Head = 0;
	int16_t  sh_Ret = 0;
	uint16_t us_Cnt = 0;
	int16_t  sh_Remain = -1;
	
	SOS_Assert(NULLS == pstExQueue);
	
	SOS_Assert(NULLS == auc_Buf);
	
	if (pstExQueue->us_Cnt != 0)
	{
		if ( pstExQueue->us_Cnt < us_Len)
		{
			for(us_Cnt = 0; us_Cnt < pstExQueue->us_Cnt; us_Cnt++)
			{
				auc_Buf[us_Cnt] = pstExQueue->auc_Buf[pstExQueue->us_Hdr];
				pstExQueue->us_Hdr = (pstExQueue->us_Hdr + 1)%pstExQueue->us_QueueLen;
			}
			sh_Ret = pstExQueue->us_Cnt;
			pstExQueue->us_Cnt = 0;
			pstExQueue->us_Hdr = 0;
			pstExQueue->us_Tail = 0;

		}
		else
		{
			sh_Remain = us_Len;
			while((sh_Remain > 0)&&(pstExQueue->us_Cnt != 0))  /*要读us_Len个数据，直到读完为止 20170808*/
			{
				us_Head = pstExQueue->us_Hdr;
				us_FrameLen = pstExQueue->auc_Buf[us_Head];
			
				us_Head = (us_Head + 1)%pstExQueue->us_QueueLen;
				us_FrameLen = (us_FrameLen<<8)|pstExQueue->auc_Buf[us_Head];
				
				if ((us_FrameLen + 2) <= pstExQueue->us_Cnt)
				{
					if (sh_Remain >= (us_FrameLen + 2))
					{
						for(us_Cnt = 0; us_Cnt < (us_FrameLen + 2); us_Cnt++)
						{
							auc_Buf[us_Cnt] = pstExQueue->auc_Buf[pstExQueue->us_Hdr];
							pstExQueue->us_Hdr = (pstExQueue->us_Hdr + 1)%pstExQueue->us_QueueLen;
						}
						pstExQueue->us_Cnt = pstExQueue->us_Cnt - us_FrameLen -2;
						sh_Ret = sh_Ret + us_FrameLen + 2;
						sh_Remain = sh_Remain - us_FrameLen - 2;
					}
					else
					{
						sh_Remain = 0;
					}
				}
				else
				{
					pstExQueue->us_Hdr = 0;
					pstExQueue->us_Cnt = 0;
					pstExQueue->us_Tail = 0;
				}
			}
		}
	}
	else
	{
		sh_Ret = 0;
	}
	
	return sh_Ret;
}
