/******************************************************************************/
/**
*@file    Proto.h       
*@brief   ���Э��ģ��                 
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
#define   FLAG_SFD_CMD     		0xFE		/*֡ͷ1��ʶ*/
#define   FLAG_SFL_CMD     		0xFD		/*֡ͷ2��ʶ*/
#define   PROTO_CHAN_MAX   		4			/*Э�����ӿ���*/

/*****************************************************************************/
/**@brief   ���Э��֡�ṹ                                                    	     */
/*****************************************************************************/
typedef  struct  tagProtoHdr
{
	uint8_t  uc_SFD;				/*֡ͷ1*/
	uint8_t  uc_SFL;				/*֡ͷ2*/
	uint8_t  uc_LenH;				/*֡���ȸ��ֽ�*/
	uint8_t  uc_LenL;				/*֡���ȵ��ֽ�*/
	uint8_t  auc_NSNH;			 	/* ������� */
	uint8_t  auc_NSNL;				 /* ������� */
	uint8_t  uc_CmdNum;				/*�����*/
	uint8_t  auc_Data[1];			/*ָ��������*/
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
/* protoģ���ʼ�� */
extern  uint8_t  ProtoMod_Open(void);

/* protoģ�鷢�ͽӿ� */
extern uint8_t  ProtoMod_Send(IN uint8_t uc_Port, IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len);

/* protoģ����սӿ� */
extern uint16_t ProtoMod_Rev(IN uint8_t uc_Port, OUT uint8_t *puc_Cmd ,OUT uint8_t auc_Buf[], IN uint16_t us_Len);

/* protoģ����մ��� */
extern void ProtoMod_RevHandle(void);

#endif /*PROTO_H_*/
