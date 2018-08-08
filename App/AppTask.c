/**
 * @file   AppTask.c                                                  
 * @brief  业务处理                                                                                                            
 * @writer   yangbm                                        
 * @version  1.00
 * @date     2013.03.27
 * @history
 * @modify list：
 */
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h"
#include  "AppTask.h"
#include  "ExPort.h"
#include  "Proto.h"
#include  "Config.h"
#include  "dev_Timer.h"
#include  "W5300.h"
#include  "Crc.h"
#include  <string.h>
/*****************************************************************************/
/**@brief  初始化标志                                                                 */
/*****************************************************************************/
static uint8_t  guc_InitFlag = SYS_NO;
static SAppInfor gst_AppInfor;
static uint8_t   auc_RevBuf[1700];
static uint8_t   auc_SndMsg[SND_MTU_MAX];
/*****************************************************************************/
/**@brief  局部函数声明                                                                */
/*****************************************************************************/
/* 命令处理 */
static void  ParseCmd(IN uint8_t uc_Port,IN uint8_t uc_Cmd, IN uint8_t auc_Buf[],IN uint16_t us_Len);
/* 处理读命令 */
static  void processReadCmd(IN uint8_t uc_Port , IN uint8_t auc_Buf[], IN uint16_t us_Len );
/* 处理写命令 */
static  void processWriteCmd(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len );

static uint32_t  IPChartToDWORD(uint8_t auc_Buf[]);

static uint8_t  PickUpSndData(uint8_t  uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len);

static uint8_t   AppTaskInit(IN uint8_t auc_Buf[], IN uint8_t uc_Len);

static uint8_t NewConfDataInit(uint8_t auc_Buf[], uint16_t us_Len);

static uint8_t OldConfDataInit(uint8_t auc_Buf[], uint16_t us_Len);



void  AppTaskStartInit(void)
{
	memset((uint8_t*)&gst_AppInfor, 0 ,sizeof(gst_AppInfor));
}
/*****************************************************************************/
/**
* @brief  void AppTaskRunHandle
*         业务任务运行处理
* @param  void
* @return void
*/
/*****************************************************************************/
void  AppTaskRunHandle(void)
{  /*处理从串口得到的数据，这时从串口得到的数据存储在gast_ProtoFrame结构提中，该函数的就是按照读取后的信息执行相应的操作 20170808*/
	uint8_t   uc_Cmd = 0;
	uint8_t   uc_Cnt = 0;
	uint16_t  us_Ret = 0;
	/*轮询各协议接口获取CPU发送的命令帧，并进行解析处理*/
	for (uc_Cnt = 1; uc_Cnt < (NUM_PORT_CPU + 1); uc_Cnt++)
	{	/*读取CPU协议帧*/                                       /*从 gast_ProtoFrame 结构体获取信息 20170808*/
		us_Ret = ProtoMod_Rev(uc_Cnt, &uc_Cmd, auc_RevBuf, 1700);    /*auc_RevBuf,静态全局变量 20170808*/
	
		if (us_Ret > 0)
		{	/*解析CPU协议帧*/
			ParseCmd(uc_Cnt,uc_Cmd, auc_RevBuf, us_Ret);
		}
		else
		{
				
		}
	}
}

/*****************************************************************************/
/**
* @brief  void AppTaskInit
*         业务任务初始化，完成配置信息初始化
* @param  IN uint8_t auc_Buf[]:输入信息
* @param  IN uint8_t uc_Len：输入信息长度
* @return void
*/
/*****************************************************************************/
static uint8_t   AppTaskInit(IN uint8_t auc_Buf[], IN uint8_t uc_Len)
{   /*根据读取的数据auc_Buf[1]的不同决定执行新配置数据还是就配置数据 20170808*/
	uint8_t uc_Ret = SYS_NO;
	SOS_Assert(NULLS == auc_Buf);
	
	/*是否有配置信息，如有进行接口配置*/
	if ((auc_Buf[0] == SYS_YES)&&(uc_Len >= 2))
	{
		/*如果不是网络型设备按串口设备进行外部初始化*/
		if(auc_Buf[1] == 0)
		{
			if (SYS_YES ==NewConfDataInit(&auc_Buf[1], uc_Len - 1))
			{
				uc_Ret = SYS_YES;
			}
		}
		else
		{
			if( SYS_YES == OldConfDataInit(&auc_Buf[1], uc_Len - 1))
			{
				uc_Ret = SYS_YES;
			}	
		}
	}
	return uc_Ret;		
}
/*****************************************************************************/
/**
* @brief  void  ParseCmd
*         解析命令帧
* @param  IN uint8_t uc_Port：命令端口号,1-4有效  ;
* @param  IN uint8_t uc_Cmd：命令类别
* @param  IN uint8_t auc_Buf[]:命令数据
* @param  IN uint16_t us_Len：命令数据长度
* @return void
*/
/*****************************************************************************/
static uint8_t NewConfDataInit(uint8_t auc_Buf[], uint16_t us_Len)
{/*根据配置信息不同配置传送的IP和端口信息 20170808*/
	uint16_t  us_ConfLen = 0;
	uint32_t  ui_IpAddr = 0;
	uint32_t  ui_GateWay = 0;
	uint32_t  ui_Mask = 0;
	uint16_t  us_RevLen = 0;
	uint16_t  us_SndPeriod = 0;
	uint8_t   uc_Cnt = 0;
	uint8_t   uc_Ret = SYS_NO;
	SSockAddr 	st_SrcSockAddr ;
	SSockAddr 	st_DestSockAddr ;
	SPortConfMsg *pst_ConfHdr = NULLS;
	SDestCfg  *pst_DstCfg = NULLS;

	pst_ConfHdr = (SPortConfMsg *)auc_Buf;
	
	if (pst_ConfHdr->uc_PortSum <= EXPORT_PORT_NUM)
	{
		us_ConfLen = pst_ConfHdr->uc_PortSum*sizeof(SDestCfg) + sizeof(SPortConfMsg) - 1;
	
		if (us_Len == us_ConfLen)  /*接受的数据长度和配置数据长度相等*/
		{
			ui_IpAddr = IPChartToDWORD(pst_ConfHdr->auc_LocalIp);    /*IP地址 20170808*/
			ui_GateWay = IPChartToDWORD(pst_ConfHdr->auc_GateWay);   /*网关 20170808*/
			ui_Mask = IPChartToDWORD(pst_ConfHdr->auc_Mask);         /*子网掩码 20170808*/
		
			W5300_Init(0, ui_IpAddr,ui_GateWay ,ui_Mask, 2);
			
			st_SrcSockAddr.sin_addr = ui_IpAddr;
			
			for (uc_Cnt = 0; uc_Cnt < EXPORT_SOCKE; uc_Cnt++)   	/*轮询的w5300的8个socket 20170808*/
			{
				st_SrcSockAddr.sin_port = NUM_SOCK_PORT +  uc_Cnt;
		    	WD5300_SetSocketUDP(0, uc_Cnt, &st_SrcSockAddr);
			}
			pst_DstCfg = (SDestCfg  *)pst_ConfHdr->auc_DestCfg;
		
			for(uc_Cnt= 0; uc_Cnt< pst_ConfHdr->uc_PortSum; uc_Cnt++ )
			{
				if ( pst_DstCfg->uc_PortNum < EXPORT_PORT_NUM)
				{
					gst_AppInfor.auc_PortNum[uc_Cnt] = pst_DstCfg->uc_PortNum;
					gst_AppInfor.uc_PortSum++;
					
					st_DestSockAddr.sin_addr = IPChartToDWORD(pst_DstCfg->auc_Ip);
					st_DestSockAddr.sin_port = pst_DstCfg->auc_Port[1];
					st_DestSockAddr.sin_port = (st_DestSockAddr.sin_port<<8)|pst_DstCfg->auc_Port[0];
					
					us_RevLen = pst_DstCfg->auc_RevLen[1];
					us_RevLen = (us_RevLen<<8)|pst_DstCfg->auc_RevLen[0];
					
					us_SndPeriod =  pst_DstCfg->auc_SndPeriod[1];
					us_SndPeriod = (us_SndPeriod<<8)|pst_DstCfg->auc_SndPeriod[0];
					
					ExPortMod_OpenNet(pst_DstCfg->uc_PortNum + 1,&st_DestSockAddr, us_SndPeriod,us_RevLen);
				}
				pst_DstCfg++;
			}
			uc_Ret = SYS_YES;
		}
	}
	else
	{
		
	}
	return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  void  ParseCmd
*         解析命令帧
* @param  IN uint8_t uc_Port：命令端口号,1-4有效  ;
* @param  IN uint8_t uc_Cmd：命令类别
* @param  IN uint8_t auc_Buf[]:命令数据
* @param  IN uint16_t us_Len：命令数据长度
* @return void
*/
/*****************************************************************************/
static uint8_t  OldConfDataInit(uint8_t auc_Buf[], uint16_t us_Len)
{
	uint32_t  ui_IpAddr = 0;
	uint32_t  ui_GateWay = 0;
	uint32_t  ui_Mask = 0;
	uint8_t   uc_Cnt = 0;
	SSockAddr 	st_SrcSockAddr ;
	SSockAddr 	st_DestSockAddr ;
	SConfMsgVer1 *pst_ConfMsgVer1 = NULLS;
	uint8_t  uc_Ret = SYS_NO;
	pst_ConfMsgVer1 = (SConfMsgVer1 *)auc_Buf;
	
	if (sizeof(SConfMsgVer1) == us_Len )
	{
		ui_IpAddr = IPChartToDWORD(pst_ConfMsgVer1->ast_PortConf[0].auc_LocalIp);
		/*初始化W5300芯片*/
		W5300_Init(0, ui_IpAddr,ui_GateWay ,ui_Mask, 1);
	
		st_SrcSockAddr.sin_addr = ui_IpAddr;
		for (uc_Cnt = 0; uc_Cnt < EXPORT_SOCKE; uc_Cnt++)
		{
			st_SrcSockAddr.sin_port = NUM_SOCK_PORT +  uc_Cnt;
		    WD5300_SetSocketUDP(0, uc_Cnt, &st_SrcSockAddr);
		}
		
		for(uc_Cnt = 0; uc_Cnt < 4; uc_Cnt++)
		{
			gst_AppInfor.auc_PortNum[uc_Cnt] = uc_Cnt;
			gst_AppInfor.uc_PortSum++;
			st_SrcSockAddr.sin_addr = IPChartToDWORD(pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_LocalIp);
			st_SrcSockAddr.sin_port = pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_LocalPort[1];
			st_SrcSockAddr.sin_port = (st_SrcSockAddr.sin_port<<8)|pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_LocalPort[0];

			st_DestSockAddr.sin_addr = IPChartToDWORD(pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_DestIP);
			st_DestSockAddr.sin_port = pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_DestPort[1];
			st_DestSockAddr.sin_port = (st_DestSockAddr.sin_port<<8)|pst_ConfMsgVer1->ast_PortConf[uc_Cnt].auc_DestPort[0];		
					
			ExPortMod_OpenNet(uc_Cnt + 1, &st_DestSockAddr,0,500);
		}
		uc_Ret = SYS_YES;
	}
	else
	{
		
	}
	return uc_Ret;
}
/*****************************************************************************/
/**
* @brief  void  ParseCmd
*         解析命令帧
* @param  IN uint8_t uc_Port：命令端口号,1-4有效  ;
* @param  IN uint8_t uc_Cmd：命令类别
* @param  IN uint8_t auc_Buf[]:命令数据
* @param  IN uint16_t us_Len：命令数据长度
* @return void
*/
/*****************************************************************************/
static void  ParseCmd(IN uint8_t uc_Port,IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{  /*根据读取信息的不同，执行不同的操作 20170808*/
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert (uc_Port > PROTO_CHAN_MAX);
	
	SOS_Assert(NULL == auc_Buf);
	
	switch(uc_Cmd)	
	{
		case READ_CMD:		/*读命令响应，当通信板向CPU板请求配置信息时，读命令携带CPU的配置信息数据*/
		{	/*检测信息帧中是否有配置数据，并进行配置*/
			if (guc_InitFlag != SYS_OK)
			{
				guc_InitFlag = AppTaskInit(auc_Buf, us_Len);
			}
			/*读命令处理，相应读命令返回外部接口接收数据*/
			processReadCmd(uc_Port,auc_Buf, us_Len);
			break;	
		}	
		case WRITE_CMD:	/*写命令处理，解析写命令数据提取外部接口发送数据*/
		{
			processWriteCmd(uc_Port, auc_Buf, us_Len);
			break;
		}
		case TERMINAL_CMD:	/*维护信息命令*/
		{
			break;
		}
		default:
		{	
			break;	
		}
	}
}

/*****************************************************************************/
/**
* @brief  void processReadCmd
*         读命令处理，解析读命令向读命令发送方返回所有外部接口接收到的数据
* @param  IN uint8_t uc_Port：命令端口号,1-4有效 ;
* @param  IN uint8_t auc_Buf[]:命令数据；
* @param  IN uint16_t us_Len：命令数据长度 OK
* @return void
*/
/*****************************************************************************/ 
static  void processReadCmd(IN uint8_t uc_Port , IN uint8_t auc_Buf[], IN uint16_t us_Len )
{/*将网卡接受的信息再加上一些信息打包发送给QJK 20170808*/
	uint8_t   uc_RevPort = 0;
	uint16_t  us_Cnt = 0;
	uint16_t  us_Sum = 0;
	int16_t   sh_Ret = 0;
	int16_t   sh_RemainLen = 0;
	uint8_t   uc_SndCnt = 0;
	SAckInfoHdr  *pst_InfoHdr = NULLS;
	
	SOS_Assert((0 == uc_Port)||(uc_Port > PROTO_CHAN_MAX));
	
	SOS_Assert( NULLS == auc_Buf );
	/*填加软件版本信息*/
	auc_SndMsg[0] = SOFT_VERSIONH;
	auc_SndMsg[1] = SOFT_VERSIONM;
	auc_SndMsg[2] = SOFT_VERSIONL;
	/*如果本地已完成初始化，则不向CPU请求配置数据，否则请求配置数据*/
	if ( SYS_YES == guc_InitFlag)     /*配置数据正确了是变成SYS_YES 20170808*/
	{
		auc_SndMsg[3] = SYS_NO;
	}
	else
	{
		auc_SndMsg[3] = SYS_YES;
	}
	us_Sum = 0;
	
	pst_InfoHdr = (SAckInfoHdr *)&auc_SndMsg[4];   /*数组大小是1000 20170808*/
	sh_RemainLen = SND_MTU_MAX - 4;/*减去前4个 20170808*/
	while((uc_SndCnt < gst_AppInfor.uc_PortSum)&&((us_Sum + sizeof(SAckInfoHdr)) <=  sh_RemainLen))
	{	
		us_Sum = us_Sum + sizeof(SAckInfoHdr) - 1;
	
		uc_RevPort = gst_AppInfor.auc_PortNum[uc_SndCnt];
		
		/*判断写入数据总长度是否越界*/

		pst_InfoHdr->uc_ComNum = uc_RevPort + 1;
	
		pst_InfoHdr->uc_State = (uint8_t)ExPort_Ioctl(uc_RevPort + 1 , STATE_COM_EXPORT);   /*获取该接口的通信状态 20170808*/
	
		sh_Ret = ExPortMod_Rev( uc_RevPort + 1, uc_Port, pst_InfoHdr->auc_Data, sh_RemainLen - us_Sum - 4);  /*数据读到pst_InfoHdr->auc_Data 20170808*/
		
		if (sh_Ret >= 0)
		{
			pst_InfoHdr->uc_LenL = sh_Ret&0xFF;
			pst_InfoHdr->uc_LenH = (sh_Ret>>8)&0xFF;
			us_Cnt = sh_Ret;
		}
		else
		{
			pst_InfoHdr->uc_LenL = 0;
			pst_InfoHdr->uc_LenH = 0;
			us_Cnt = 0;
		}
		pst_InfoHdr = (SAckInfoHdr *)(&pst_InfoHdr->auc_Data[us_Cnt]);    /*指向下一片区域 20170808*/
		us_Sum = us_Sum + us_Cnt;
		uc_SndCnt++;
	}
	/*如果信息包组包完成则向对方CPU发送对相应命令帧返回读取数据*/

	ProtoMod_Send(uc_Port, READ_ACK, auc_SndMsg, us_Sum + 4);

}

/*****************************************************************************/
/**
* @brief  processWriteCmd
*         处理写命令，对CPU的写数据报文进行解析，提取各外部接口发送数据
* @param  IN uint8_t uc_Port：命令端口号,1-4有效 
* @param  IN uint8_t auc_Buf[]:输入报文;
* @param  IN uint16_t us_Len:输入报文长度
* @return void
*/
/*****************************************************************************/
static  void processWriteCmd(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len )
{   /*将us_Len个数据通过网卡发送出去 20170808*/
	uint8_t  uc_Ret = SYS_OK;
	uint16_t  us_Sum = 0;	/*已解析处理数据长度*/
	uint16_t  us_Cnt = 0;
	SWriteCmdHdr* pst_CmdHdr = NULLS;
	/*判断输入信息长度是否满足最小帧要求和是否是CPU1发送来的数据，只对CPU1发送的数据进行处理*/

	if (CPUBAORD_1 != auc_Buf[0])
	{		
		uc_Ret = SYS_ERR;
	}
	us_Sum = us_Sum + 1;
	/*解析数据区，获取各外部接口发送数据长度和数据区位置*/	
	pst_CmdHdr = (SWriteCmdHdr*)(&auc_Buf[1]);		
	
	while( (uc_Ret == SYS_OK)&&((us_Sum + sizeof(SWriteCmdHdr) -1) < us_Len))  /*直到对us_Len个数据判断完成 20170808*/
	{
		if((pst_CmdHdr->uc_Com >NUM_PORT_MAX)|| (pst_CmdHdr->uc_Com == 0))	
		{
			uc_Ret = SYS_ERR;
		}	
		else
		{
			us_Cnt = pst_CmdHdr->uc_LenH;
			us_Cnt = (us_Cnt<<8)|pst_CmdHdr->uc_LenL;
			us_Sum = us_Sum + us_Cnt + sizeof(SWriteCmdHdr) -1;
			
			if (us_Sum > us_Len)
			{
				uc_Ret = SYS_ERR;
			}
			else
			{
				PickUpSndData(pst_CmdHdr->uc_Com , pst_CmdHdr->auc_Buf, us_Cnt);
				
				pst_CmdHdr = (SWriteCmdHdr*)&pst_CmdHdr->auc_Buf[us_Cnt];
			}
		}			
	}
}
/*****************************************************************************/
/**
* @brief  uint8_t  PickUpSndData
*         从信息区内提取外部端接口发送数据帧写入外部接口模块发送队列中
* @param  IN uint8_t  uc_Port:外部接口号;
* @param  IN uint8_t auc_Buf[]:输入信息；
* @param  IN uint16_t us_Len：输入信息长度
* @return uint8_t：正常--SYS_YES，否则宕机
*/
/*****************************************************************************/
static uint8_t  PickUpSndData(IN uint8_t  uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{
	uint16_t  us_Sum = 0;
	uint16_t  us_Ctr = 0;
	/* 入参检查 */
	
	SOS_Assert(NULLS == auc_Buf);
	
	/*主控板发送来的数据可能包括多帧对外外部发送数据，根据数据区内结构提取各帧，格式为帧长度，帧数据*/
	while(us_Sum < us_Len)
	{
		us_Ctr = auc_Buf[us_Sum];
		us_Ctr = (us_Ctr<<8)|auc_Buf[us_Sum + 1];
		/*如果数据长度为零，为异常数据退出提取操作*/
		if (0 == us_Ctr)	
		{
			us_Sum = us_Len;	/*发送完成，或者没有数据 20170808*/	
		}
		else
		{	/*如果已提取的数据加上当前的帧结构数据长度超出输入信息长度为异常数据不进行写操作*/
			if ((us_Sum + us_Ctr + 2) <= us_Len)
			{	/*指向帧的数据区位置*/
				us_Sum = us_Sum + 2;
				/* 写入接收数据帧存储队列 */		
				ExPortMod_Send(uc_Port, &auc_Buf[us_Sum], us_Ctr);  /*通过网卡就将us_Ctr个数据发送出去，相当于是一帧一帧长度的发送 20170808*/
			}
			us_Sum = us_Sum + us_Ctr;
		}
	}
	return  SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint32_t  IPChartToDWORD
*         IP地址数据转换，字节类型转换成32位长整型
* @param  IN uint8_t auc_Buf[]：IP地址输入
* @return uint32_t：32位IP地址形式
*/
/*****************************************************************************/
static uint32_t  IPChartToDWORD(IN uint8_t auc_Buf[])  /*4个uint8_t 转换成1个uint32_t 20170808*/
{
	uint32_t ui_Data = 0U;
	
	if( NULLS == auc_Buf )
	{
		ui_Data = 0U;	
	}
	else
	{
		ui_Data= auc_Buf[0];
		ui_Data = (ui_Data<<8)|auc_Buf[1];
		ui_Data = (ui_Data<<8)|auc_Buf[2];
		ui_Data = (ui_Data<<8)|auc_Buf[3];
	}
	return ui_Data;
}

