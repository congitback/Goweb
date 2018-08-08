/******************************************************************************/
/**
*@file    AppTask.h      
*@brief   业务处理模块头文件                  
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
#ifndef __APPTASK_H_
#define __APPTASK_H_

/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define   CPUBAORD_1   	   0x01		/*CPU1标识*/
#define   READ_CMD         0x61		/*读命令码*/
#define   WRITE_CMD        0x81		/*写命令码*/
#define   TERMINAL_CMD     0x33		/*维护命令码*/
#define   READ_ACK         0x62		/*读响应码*/
#define   NET_DEV_TYPE     0x11		/*网络设备类型码*/
#define   UART_DEV_TYPE    0x22		/*串口设备类型码*/
#define   NUM_PORT_CPU     4		/*对应CPU接口数量*/
#define   NUM_EXPORT	   8		/*外部接口数量*/
#define   NUM_EXPORTVER1   4
#define   NUM_SOCK_PORT    5000
#define   NUM_PORT_MAX     32
#define   SND_MTU_MAX      1000
/*****************************************************************************/
/**@brief   命令响应报文结构头                                                   	     */
/*****************************************************************************/
typedef  struct tagAckInfoHdr
{
	uint8_t  uc_ComNum;		/*外部接口号*/
	uint8_t  uc_State;		/*外部接口状态*/
	uint8_t  uc_LenH;		/*外部接口接收数据长度高字节*/
	uint8_t  uc_LenL;		/*外部接口接收数据长度低字节*/
	uint8_t  auc_Data[1];	/*指向数据区*/
}SAckInfoHdr;
/*****************************************************************************/
/**@brief   写命令报文头                                                     	     */
/*****************************************************************************/
typedef struct tagWriteCmdHdr
{
	uint8_t  uc_Com;		/*外部接口号*/
	uint8_t  uc_LenH;		/*发送数据长度高字节*/
	uint8_t  uc_LenL;		/*发送数据长度低字节*/
	uint8_t  auc_Buf[1];	/*指向发送数据*/
}SWriteCmdHdr;


/*****************************************************************************/
/**@brief   外部接口配置信息结构                                                 	     */
/*****************************************************************************/
typedef  struct  tagPortConfMsg
{
	uint8_t  uc_VerFlag;
	uint8_t  auc_Ver[3];
	uint8_t  auc_LocalIp[4];			/*本地IP*/
	uint8_t  auc_Mask[4];             	/*子网掩码 20170808*/
	uint8_t  auc_GateWay[4];			/*网关 20170808*/
	uint8_t  uc_PortSum;                /*外部接口数量 20170808*/
	uint8_t  auc_DestCfg[1];	
}SPortConfMsg;

typedef  struct  tagDestCfg
{
	uint8_t  uc_PortNum;
	uint8_t  auc_SndLen[2];	
	uint8_t  auc_RevLen[2];	
	uint8_t  auc_SndPeriod[2];
	uint8_t  auc_Ip[4];
	uint8_t  auc_Port[2];
}SDestCfg;

typedef  struct  tagPortConfMsgVer1
{
	uint8_t  auc_LocalIp[4];		/*本地IP*/
	uint8_t  auc_LocalPort[2];		/*本地端口号*/
	uint8_t  auc_DestIP[4];			/*目的IP*/
	uint8_t  auc_DestPort[2];		/*目的端口号*/
}SPortConfMsgVer1;
/*****************************************************************************/
/**@brief   配置信息接口                                                                  	                     */
/*****************************************************************************/
typedef  struct  tagConfMsgVer1
{
	uint8_t 			uc_Type;					/*设备类型*/
	SPortConfMsgVer1	ast_PortConf[NUM_EXPORTVER1];	/*设备外部接口配置信息*/
}SConfMsgVer1;


typedef  struct tagAppInfor
{
	uint8_t auc_PortNum[NUM_PORT_MAX];          /*NUM_PORT_MAX = 32 20170808*/
	uint8_t uc_PortSum;
	uint8_t uc_SndPortNum;
}SAppInfor;
extern void  AppTaskRunHandle(void);
extern void  AppTaskStartInit(void);
#endif /*APPTASK_H_*/
