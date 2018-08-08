/******************************************************************************/
/**
*@file    ExPort.h      
*@brief   外部接口模块                      
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
#ifndef __EXPORT_H_
#define __EXPORT_H_
#include  "Typedef.h"
#include  "W5300.h"
/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define  STATE_COM_EXPORT				0x11		/*获取接口状态码*/
#define  ERRCNT_EXPORT  				0x12		/*获取接口错误计数值*/
#define  CNT_EXPORT                     0x13		/*获取接口接收计数值*/

#define  EXPORT_PORT_NUM        	 	16			/*外部接口数量*/
#define  EXPORT_SOCKE                   8


#define  EXPORT_REV_FRAME_LEN           1000L
#define  EXPORT_RAM_LEN  				30*1024L

typedef  struct tagExPortRevBuf
{
	uint16_t  us_RevId;	            /*内存块的ID 20170808*/
	uint32_t  ui_Timer;				/*声明存储内存块内的数据写入时间  20170808*/		
}SExPortRevBuf;

/*****************************************************************************/
/**@brief   外部接口网络地址映射                                                 	     */
/*****************************************************************************/
typedef  struct  tagExPortNetMap
{
	uint32_t  ui_IpAddr;		/*网络IP地址*/
	uint16_t  us_Port;			/*外部接口号*/
	uint16_t  us_SndPerid;
}SExPortNetMap;

typedef struct tagExQueue
{
	uint16_t us_QueueLen;
	uint16_t us_Hdr;
	uint16_t us_Tail;
	uint16_t us_Cnt;	
	uint8_t  auc_Buf[1];
}ExQueue;
/*****************************************************************************/
/**@brief   外部接口信息结构                                                  	     */
/*****************************************************************************/
#define  REV_SEC_MAX		4                              /*先这么理解 使用了其中的4个内存块，相当于对应主控板的4个CPU 20170808*/

typedef  struct  tagExPortInfo
{
	uint8_t  uc_Init;					/*初始化标志*/
	uint8_t  uc_ComState;				/*接口通信状态*/
	uint32_t ui_Timer ;					/*接口通信更新时间*/
	uint32_t ui_Cnt ;					/*接口接收数据帧计数*/
	SExPortNetMap st_NetMap;			/*网络地址映射*/
	SExPortRevBuf ast_RevBuf[REV_SEC_MAX];      /**/
}SExPortInfo;

/*****************************************************************************/
/**@brief   外部接口模块接口声明                                                 	     */
/*****************************************************************************/
extern  void ExPortMod_Init(void);


extern  uint8_t  ExPortMod_OpenNet(IN uint8_t uc_Port, IN SSockAddr  *pst_DestAddr,
							uint16_t us_SndPriod, uint16_t us_Len);

extern  uint8_t  ExPortMod_Send(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len);

extern  int16_t  ExPortMod_Rev(IN uint8_t uc_Port, IN uint8_t uc_Sect, 
									OUT uint8_t auc_Buf[], IN uint16_t us_Len);
									
extern  uint32_t  ExPort_Ioctl(IN uint8_t uc_Port, IN uint8_t  uc_Arg);

extern  void ExPortMod_RevHandle(void);
#endif /*__EXPORT_H_*/
