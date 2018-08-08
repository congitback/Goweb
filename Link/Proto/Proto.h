/******************************************************************************/
/**
*@file    Proto.h       
*@brief   板间协议模块                 
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
#ifndef __PROTO_H__
#define __PROTO_H__
/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define   FLAG_SFD_CMD     		0xFE		/*帧头1标识*/
#define   FLAG_SFL_CMD     		0xFD		/*帧头2标识*/
#define   PROTO_CHAN_MAX   		4			/*协议最大接口数*/

/*****************************************************************************/
/**@brief   板间协议帧结构                                                    	     */
/*****************************************************************************/
typedef  struct  tagProtoHdr
{
	uint8_t  uc_SFD;				/*帧头1*/
	uint8_t  uc_SFL;				/*帧头2*/
	uint8_t  uc_LenH;				/*帧长度高字节*/
	uint8_t  uc_LenL;				/*帧长度低字节*/
	uint8_t  auc_NSNH;			 	/* 报文序号 */
	uint8_t  auc_NSNL;				 /* 报文序号 */
	uint8_t  uc_CmdNum;				/*命令号*/
	uint8_t  auc_Data[1];			/*指向数据区*/
}SProtoHdr;

#define  PROTO_REV_QUEUE_LEN   2048
#define  PROTO_REV_QUEUE_LEN_MASK   2047

typedef  struct tagProtoRevQueue
{
	uint16_t  us_Tail;
	uint16_t  us_Head;
	uint16_t  us_Count;	
	uint8_t   auc_Buf[PROTO_REV_QUEUE_LEN];
	uint32_t  ui_Cnt;
	uint32_t  ui_ErrCnt;
}SProtoRevQueue;

#define  FRAME_PROTO_LEN_MAX     1700
typedef  struct  tagProtoFrame
{
	uint32_t  ui_Cnt;
	uint32_t  ui_Timer;
	uint8_t   uc_Com;
	uint8_t   uc_Cmd;
	uint16_t  us_Len;
	uint8_t   auc_Buf[FRAME_PROTO_LEN_MAX];	
	
}SProtoFrame;
/* proto模块初始化 */
extern  uint8_t  ProtoMod_Open(void);

/* proto模块发送接口 */
extern uint8_t  ProtoMod_Send(IN uint8_t uc_Port, IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len);

/* proto模块接收接口 */
extern uint16_t ProtoMod_Rev(IN uint8_t uc_Port, OUT uint8_t *puc_Cmd ,OUT uint8_t auc_Buf[], IN uint16_t us_Len);

/* proto模块接收处理 */
extern void ProtoMod_RevHandle(void);

#endif /*PROTO_H_*/
