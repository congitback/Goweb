/******************************************************************************/
/**
*@file    ExPort.c      
*@brief   �ⲿ�ӿ�ģ��                  
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
/**@brief  �ⲿ�ӿ��豸����                                                            */
/*****************************************************************************/  

/*****************************************************************************/
/**@brief  �ⲿ�ӿ�ģ��ӿ���Ϣ                        					         */
/*****************************************************************************/
static  SExPortInfo      gast_ExPortInfo[EXPORT_PORT_NUM];

/*****************************************************************************/
/**@brief  �ֲ���������                                					         */
/*****************************************************************************/

static  void  ExPortMod_Check(void);

static  int16_t  ExPort_FindPortNum(IN uint32_t  ui_IpAddr);

static  int16_t  ReadExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len);

static  uint8_t  WriteExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len);
/*****************************************************************************/
/**
* @brief  void ExPortMod_Init
*         �ⲿģ���ʼ�������ģ����������
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
 * @brief  ExPortMod_Open,�ⲿ����ģ��򿪲���
 * @param  IN uint8_t uc_Port:�˿ںţ�1-2��Ч
 * 		   IN uint8_t uc_Major���˿����豸�ţ�1-6��Ч��
 * 		   IN uint8_t uc_Minor���˿ڴ��豸�ţ�1-4��Ч��
 * 		   IN uint8_t uc_DataType���������� ��1-3��Ч;
 * @return uint8_t :SYS_OK/SYS_ERR---�ɹ�/ʧ�� OK
 * @see 
 */ 

uint8_t  ExPortMod_OpenNet(IN uint8_t uc_Port, IN SSockAddr  *pst_DestAddr, uint16_t us_SndPriod, uint16_t us_Len)
{/*��ʼ��ȫ�ֱ���gast_ExPortInfo<����ָʾĿ�ĵ�ַĿ��IP�˿ڵ���Ϣ>��������ڴ�� 20170808*/
	uint8_t uc_Cnt = 0;
	int16_t sh_ID = -1;
	ExQueue *pst_ExpQueue = NULLS;
	
 	SOS_Assert(0 == uc_Port);
 
 	SOS_Assert(uc_Port > EXPORT_PORT_NUM);
 	
 	SOS_Assert(NULLS == pst_DestAddr);	
 	
 	uc_Port = uc_Port -1;
	
 	/*��սӿ���Ϣ����*/
 	memset(&gast_ExPortInfo[uc_Port],0, sizeof (gast_ExPortInfo[uc_Port]));
 	
 	/*�����ⲿ�ӿ�ӳ���ַΪĿ�ĵ�ַ��Ŀ�Ķ˿ں����������豸������ӿڹ���*/	
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
*         �ⲿ����ģ��д���������ڽ�����д���ⲿ�ӿڶ�Ӧ����ӿ�
* @param  IN uint8_t uc_Port:�˿ںţ�1-4��Ч
* @param  IN uint8_t auc_Buf[]:��������;
* @param  IN uint16_t us_Len:�������ݳ���;
* @return uint8_t��SYS_YES---����������SYS_NO---����ʧ�� OK
*/
/*****************************************************************************/
uint8_t  ExPortMod_Send(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*��us_Len��auc_Buf����ͨ��socket uc_Port���ͳ�ȥ 20170808*/
 	uint8_t  uc_Ret = SYS_OK;
 	uint8_t  uc_SndPort = 0U;
 	
 	SSockAddr  st_SockAddr ;

	SOS_Assert(0 == uc_Port);
 	
	SOS_Assert(uc_Port > EXPORT_PORT_NUM);
	
	SOS_Assert(NULLS == auc_Buf);
	
	uc_Port = uc_Port -1;
	/*����ⲿ�ӿ��ѳ�ʼ���ͷ������ݳ��Ȳ�Ϊ��*/
	if ((SYS_OK == gast_ExPortInfo[uc_Port].uc_Init)&&(us_Len >0))
	{		/*���Ϊ�����豸������д������ӿ���*/
	
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
*         �ⲿ����ģ��������������������ص����ݽṹΪ���ݳ���+����֡�����ص��������ɰ�����֡
* @param  IN uint8_t uc_Port:�˿ںţ�1-4��Ч
* @param  IN uint8_t uc_Sect����ȡ����֡�洢����
* @param  OUT uint8_t auc_Buf[]������������;
* @param  IN uint16_t us_Len:��������������;
* @return int16_t : 0---������,>0---�������ݳ���,<0----����������С���޷���ȡ OK
*/ 
/*****************************************************************************/
int16_t  ExPortMod_Rev(IN uint8_t uc_Port, IN uint8_t uc_Sect, OUT uint8_t auc_Buf[], IN uint16_t us_Len)
{   /*���ڴ���н����ݶ�ȡ��auc_Buf�� 20170808*/
 	int16_t sh_ID = -1;
 	int16_t sh_Ret = -1;
 	ExQueue *pst_ExpQueue = NULLS;
 	
 	SOS_Assert((0 == uc_Port)||(uc_Port > EXPORT_PORT_NUM));
	
	SOS_Assert((0 == uc_Sect)||(uc_Sect > REV_SEC_MAX));
	
	SOS_Assert(NULLS == auc_Buf);
	
	uc_Port = uc_Port -1;
	uc_Sect = uc_Sect -1;
	/*����ⲿ�ӿ��ѳ�ʼ�����ж�����*/
	if(SYS_OK == gast_ExPortInfo[uc_Port].uc_Init)
	{
		sh_ID = gast_ExPortInfo[uc_Port].ast_RevBuf[uc_Sect].us_RevId;
		
		pst_ExpQueue = (ExQueue *)GetObjAddr(gauc_ExportRam,sh_ID);
		
		SOS_Assert(pst_ExpQueue == NULLS);
		/*�������֡�洢������Ч��*/
		sh_Ret = ReadExQueue(pst_ExpQueue, auc_Buf, us_Len);

	}
	/*���ض�ȡ���ݳ���*/
 	return sh_Ret;
}

/*****************************************************************************/
/**
* @brief  uint32_t  ExPort_Ioctl
*         �ⲿ�ӿڿ��Ʋ���
* @param  IN uint8_t uc_Port���˿ںţ�1-4��Ч
* @param  IN uint8_t  uc_Arg�������룬STATE_COM_EXPORT--��ȡ�ӿ�ͨ��״̬��
* 								ERRCNT_EXPORT--��ȡ�ⲿ�ӿڽ��ն��д��������
* 								CNT_EXPORT--��ȡ�ⲿ�ӿڽ��ն��н��ռ���
* @return uint8_t��SYS_YES---��������������崻� OK
*/
/*****************************************************************************/
uint32_t  ExPort_Ioctl(IN uint8_t uc_Port, IN uint8_t  uc_Arg)
{	/*�õ��ⲿ�ӿڵ�ͨ��״̬ 20170808*/
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
*         �ⲿ�ӿ�ģ����մ���ִ��
* @param  void
* @return void OK
*/
/*****************************************************************************/
void ExPortMod_RevHandle(void)/*ѭ�����Ƚ����������ݣ������ڴ���в����µ��ڵ����ݿ� 20170808*/
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
		i_Ret =	W5300_SockRev(0, uc_Cnt, &ui_IpAddr, auc_Buf, EXPORT_REV_FRAME_LEN); /*������0ѭ��socket��ȡ���ݵ�auc_Buf��*/
		
		if (i_Ret > 0)
		{
			i_Port = ExPort_FindPortNum(ui_IpAddr);
			
			if (i_Port >= 0)
			{
				if (gast_ExPortInfo[i_Port].uc_Init == SYS_YES)              /*i_Port������ͨ���������ݵõ��� 20170808*/
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
					gast_ExPortInfo[i_Port].ui_Timer = getTick();              /*��������״̬ 20170808*/
				}
			}
		}
	}
	
	ExPortMod_Check();	                /*���ͨ��״̬��2sû������������ͨ���жϣ����ʱ�䵽��2s���ڴ�������*/
}

/*****************************************************************************/
/**
* @brief  void  ExPortMod_Check
*         ����ⲿ�ӿ�ģ����ӿ�ͨ��״̬
* @param  void
* @return void OK
*/
/*****************************************************************************/
static  void  ExPortMod_Check(void)   /*���ͨ��״̬�����ʱ�䵽��2s���ڴ�������*/
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
			{    /*�ڴ���е���������������2s*/
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
*         ����IP��ַ��ѯ�ӿ�ӳ�����ݷ����ⲿ�ӿں�
* @param  IN uint32_t  ui_IpAddr������IP��ַ
* @return uint8_t�������ⲿ�ӿںţ�1-EXPORT_PORT_NUM OK
*/
/*****************************************************************************/
static  int16_t  ExPort_FindPortNum(IN uint32_t  ui_IpAddr)
{
	uint8_t   uc_Port = 0U;
	int16_t  sh_Ret = -1;
	for(uc_Port = 0; uc_Port < EXPORT_PORT_NUM ; uc_Port++)	        /*��IP��֪�õ����ĸ��ⲿ�ӿڵõ����� 20170808*/
	{
		if (gast_ExPortInfo[uc_Port].st_NetMap.ui_IpAddr == ui_IpAddr)  /*��ʼ���õ� 20170808*/
		{
			sh_Ret = uc_Port;
		}
	}
	return sh_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  WriteExQueue
*         ��auc_Buf�е����ݴ������ExQueue��
* @param  IN uint32_t  ui_IpAddr������IP��ַ
* @return uint8_t�������ⲿ�ӿںţ�1-EXPORT_PORT_NUM
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
	
	sh_Remain = pstExQueue->us_QueueLen - pstExQueue->us_Cnt;     /*ѭ������ʣ��ĳ��� 20170808*/
	
	while((sh_Remain < (us_Len + 2))&&(pstExQueue->us_Cnt > 2))/*���ʣ��ռ�С��д�볤�ȣ���ɾ���������һ֡����*/
	{/*��Ҫ�洢��ʣ��ռ䲻���Դ洢һ֡���� 20170808*/
		us_Head = pstExQueue->us_Hdr;           /*�Ӷ��е���������ȡ���������֡�ṹ 20170808*/
		us_FrameLen = pstExQueue->auc_Buf[us_Head];
			
		us_Head = (us_Head + 1)%pstExQueue->us_QueueLen;
		us_FrameLen = (us_FrameLen<<8)|pstExQueue->auc_Buf[us_Head];
		
		us_FrameLen = us_FrameLen + 2;            /*Ҫ�������֡���ݳ��� 20170808*/
		if (us_FrameLen  <= pstExQueue->us_Cnt)
		{  /*�Ӹ�ѭ�������޳�һ֡���� 20170808*/
			pstExQueue->us_Hdr = (pstExQueue->us_Hdr +  us_FrameLen)%pstExQueue->us_QueueLen;
			pstExQueue->us_Cnt = pstExQueue->us_Cnt -  us_FrameLen;
		}
		else
		{ /*���ݳ��ȱ��������ݸ����󣬳���ֱ��������е� 20170808*/
			pstExQueue->us_Cnt = 0;
			pstExQueue->us_Hdr = 0;
			pstExQueue->us_Tail = 0;
		}
		sh_Remain = pstExQueue->us_QueueLen - pstExQueue->us_Cnt;     /*����һ��ʣ������ 20170808*/
	}
	
	if (sh_Remain >= (us_Len + 2))
	{
		pstExQueue->auc_Buf[pstExQueue->us_Tail] = us_Len>>8;               /*�ȴ���֡���� 20170808*/
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
*         ����IP��ַ��ѯ�ӿ�ӳ�����ݷ����ⲿ�ӿں�
* @param  IN uint32_t  ui_IpAddr������IP��ַ
* @return uint8_t�������ⲿ�ӿںţ�1-EXPORT_PORT_NUM
*/
/*****************************************************************************/
static  int16_t  ReadExQueue(ExQueue *pstExQueue, uint8_t auc_Buf[], uint16_t us_Len)
{										/*���ڴ��Ķ��������ݶ���ȡ��auc_Buf�� 20170808*/
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
			while((sh_Remain > 0)&&(pstExQueue->us_Cnt != 0))  /*Ҫ��us_Len�����ݣ�ֱ������Ϊֹ 20170808*/
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
