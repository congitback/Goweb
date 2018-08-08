/******************************************************************************/
/**
*@file    AppTask.h      
*@brief   ҵ����ģ��ͷ�ļ�                  
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
#define   CPUBAORD_1   	   0x01		/*CPU1��ʶ*/
#define   READ_CMD         0x61		/*��������*/
#define   WRITE_CMD        0x81		/*д������*/
#define   TERMINAL_CMD     0x33		/*ά��������*/
#define   READ_ACK         0x62		/*����Ӧ��*/
#define   NET_DEV_TYPE     0x11		/*�����豸������*/
#define   UART_DEV_TYPE    0x22		/*�����豸������*/
#define   NUM_PORT_CPU     4		/*��ӦCPU�ӿ�����*/
#define   NUM_EXPORT	   8		/*�ⲿ�ӿ�����*/
#define   NUM_EXPORTVER1   4
#define   NUM_SOCK_PORT    5000
#define   NUM_PORT_MAX     32
#define   SND_MTU_MAX      1000
/*****************************************************************************/
/**@brief   ������Ӧ���Ľṹͷ                                                   	     */
/*****************************************************************************/
typedef  struct tagAckInfoHdr
{
	uint8_t  uc_ComNum;		/*�ⲿ�ӿں�*/
	uint8_t  uc_State;		/*�ⲿ�ӿ�״̬*/
	uint8_t  uc_LenH;		/*�ⲿ�ӿڽ������ݳ��ȸ��ֽ�*/
	uint8_t  uc_LenL;		/*�ⲿ�ӿڽ������ݳ��ȵ��ֽ�*/
	uint8_t  auc_Data[1];	/*ָ��������*/
}SAckInfoHdr;
/*****************************************************************************/
/**@brief   д�����ͷ                                                     	     */
/*****************************************************************************/
typedef struct tagWriteCmdHdr
{
	uint8_t  uc_Com;		/*�ⲿ�ӿں�*/
	uint8_t  uc_LenH;		/*�������ݳ��ȸ��ֽ�*/
	uint8_t  uc_LenL;		/*�������ݳ��ȵ��ֽ�*/
	uint8_t  auc_Buf[1];	/*ָ��������*/
}SWriteCmdHdr;


/*****************************************************************************/
/**@brief   �ⲿ�ӿ�������Ϣ�ṹ                                                 	     */
/*****************************************************************************/
typedef  struct  tagPortConfMsg
{
	uint8_t  uc_VerFlag;
	uint8_t  auc_Ver[3];
	uint8_t  auc_LocalIp[4];			/*����IP*/
	uint8_t  auc_Mask[4];             	/*�������� 20170808*/
	uint8_t  auc_GateWay[4];			/*���� 20170808*/
	uint8_t  uc_PortSum;                /*�ⲿ�ӿ����� 20170808*/
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
	uint8_t  auc_LocalIp[4];		/*����IP*/
	uint8_t  auc_LocalPort[2];		/*���ض˿ں�*/
	uint8_t  auc_DestIP[4];			/*Ŀ��IP*/
	uint8_t  auc_DestPort[2];		/*Ŀ�Ķ˿ں�*/
}SPortConfMsgVer1;
/*****************************************************************************/
/**@brief   ������Ϣ�ӿ�                                                                  	                     */
/*****************************************************************************/
typedef  struct  tagConfMsgVer1
{
	uint8_t 			uc_Type;					/*�豸����*/
	SPortConfMsgVer1	ast_PortConf[NUM_EXPORTVER1];	/*�豸�ⲿ�ӿ�������Ϣ*/
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
