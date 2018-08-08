/******************************************************************************/
/**
*@file    ExPort.h      
*@brief   �ⲿ�ӿ�ģ��                      
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
#define  STATE_COM_EXPORT				0x11		/*��ȡ�ӿ�״̬��*/
#define  ERRCNT_EXPORT  				0x12		/*��ȡ�ӿڴ������ֵ*/
#define  CNT_EXPORT                     0x13		/*��ȡ�ӿڽ��ռ���ֵ*/

#define  EXPORT_PORT_NUM        	 	16			/*�ⲿ�ӿ�����*/
#define  EXPORT_SOCKE                   8


#define  EXPORT_REV_FRAME_LEN           1000L
#define  EXPORT_RAM_LEN  				30*1024L

typedef  struct tagExPortRevBuf
{
	uint16_t  us_RevId;	            /*�ڴ���ID 20170808*/
	uint32_t  ui_Timer;				/*�����洢�ڴ���ڵ�����д��ʱ��  20170808*/		
}SExPortRevBuf;

/*****************************************************************************/
/**@brief   �ⲿ�ӿ������ַӳ��                                                 	     */
/*****************************************************************************/
typedef  struct  tagExPortNetMap
{
	uint32_t  ui_IpAddr;		/*����IP��ַ*/
	uint16_t  us_Port;			/*�ⲿ�ӿں�*/
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
/**@brief   �ⲿ�ӿ���Ϣ�ṹ                                                  	     */
/*****************************************************************************/
#define  REV_SEC_MAX		4                              /*����ô��� ʹ�������е�4���ڴ�飬�൱�ڶ�Ӧ���ذ��4��CPU 20170808*/

typedef  struct  tagExPortInfo
{
	uint8_t  uc_Init;					/*��ʼ����־*/
	uint8_t  uc_ComState;				/*�ӿ�ͨ��״̬*/
	uint32_t ui_Timer ;					/*�ӿ�ͨ�Ÿ���ʱ��*/
	uint32_t ui_Cnt ;					/*�ӿڽ�������֡����*/
	SExPortNetMap st_NetMap;			/*�����ַӳ��*/
	SExPortRevBuf ast_RevBuf[REV_SEC_MAX];      /**/
}SExPortInfo;

/*****************************************************************************/
/**@brief   �ⲿ�ӿ�ģ��ӿ�����                                                 	     */
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
