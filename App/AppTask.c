/**
 * @file   AppTask.c                                                  
 * @brief  ҵ����                                                                                                            
 * @writer   yangbm                                        
 * @version  1.00
 * @date     2013.03.27
 * @history
 * @modify list��
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
/**@brief  ��ʼ����־                                                                 */
/*****************************************************************************/
static uint8_t  guc_InitFlag = SYS_NO;
static SAppInfor gst_AppInfor;
static uint8_t   auc_RevBuf[1700];
static uint8_t   auc_SndMsg[SND_MTU_MAX];
/*****************************************************************************/
/**@brief  �ֲ���������                                                                */
/*****************************************************************************/
/* ����� */
static void  ParseCmd(IN uint8_t uc_Port,IN uint8_t uc_Cmd, IN uint8_t auc_Buf[],IN uint16_t us_Len);
/* ��������� */
static  void processReadCmd(IN uint8_t uc_Port , IN uint8_t auc_Buf[], IN uint16_t us_Len );
/* ����д���� */
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
*         ҵ���������д���
* @param  void
* @return void
*/
/*****************************************************************************/
void  AppTaskRunHandle(void)
{  /*����Ӵ��ڵõ������ݣ���ʱ�Ӵ��ڵõ������ݴ洢��gast_ProtoFrame�ṹ���У��ú����ľ��ǰ��ն�ȡ�����Ϣִ����Ӧ�Ĳ��� 20170808*/
	uint8_t   uc_Cmd = 0;
	uint8_t   uc_Cnt = 0;
	uint16_t  us_Ret = 0;
	/*��ѯ��Э��ӿڻ�ȡCPU���͵�����֡�������н�������*/
	for (uc_Cnt = 1; uc_Cnt < (NUM_PORT_CPU + 1); uc_Cnt++)
	{	/*��ȡCPUЭ��֡*/                                       /*�� gast_ProtoFrame �ṹ���ȡ��Ϣ 20170808*/
		us_Ret = ProtoMod_Rev(uc_Cnt, &uc_Cmd, auc_RevBuf, 1700);    /*auc_RevBuf,��̬ȫ�ֱ��� 20170808*/
	
		if (us_Ret > 0)
		{	/*����CPUЭ��֡*/
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
*         ҵ�������ʼ�������������Ϣ��ʼ��
* @param  IN uint8_t auc_Buf[]:������Ϣ
* @param  IN uint8_t uc_Len��������Ϣ����
* @return void
*/
/*****************************************************************************/
static uint8_t   AppTaskInit(IN uint8_t auc_Buf[], IN uint8_t uc_Len)
{   /*���ݶ�ȡ������auc_Buf[1]�Ĳ�ͬ����ִ�����������ݻ��Ǿ��������� 20170808*/
	uint8_t uc_Ret = SYS_NO;
	SOS_Assert(NULLS == auc_Buf);
	
	/*�Ƿ���������Ϣ�����н��нӿ�����*/
	if ((auc_Buf[0] == SYS_YES)&&(uc_Len >= 2))
	{
		/*��������������豸�������豸�����ⲿ��ʼ��*/
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
*         ��������֡
* @param  IN uint8_t uc_Port������˿ں�,1-4��Ч  ;
* @param  IN uint8_t uc_Cmd���������
* @param  IN uint8_t auc_Buf[]:��������
* @param  IN uint16_t us_Len���������ݳ���
* @return void
*/
/*****************************************************************************/
static uint8_t NewConfDataInit(uint8_t auc_Buf[], uint16_t us_Len)
{/*����������Ϣ��ͬ���ô��͵�IP�Ͷ˿���Ϣ 20170808*/
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
	
		if (us_Len == us_ConfLen)  /*���ܵ����ݳ��Ⱥ��������ݳ������*/
		{
			ui_IpAddr = IPChartToDWORD(pst_ConfHdr->auc_LocalIp);    /*IP��ַ 20170808*/
			ui_GateWay = IPChartToDWORD(pst_ConfHdr->auc_GateWay);   /*���� 20170808*/
			ui_Mask = IPChartToDWORD(pst_ConfHdr->auc_Mask);         /*�������� 20170808*/
		
			W5300_Init(0, ui_IpAddr,ui_GateWay ,ui_Mask, 2);
			
			st_SrcSockAddr.sin_addr = ui_IpAddr;
			
			for (uc_Cnt = 0; uc_Cnt < EXPORT_SOCKE; uc_Cnt++)   	/*��ѯ��w5300��8��socket 20170808*/
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
*         ��������֡
* @param  IN uint8_t uc_Port������˿ں�,1-4��Ч  ;
* @param  IN uint8_t uc_Cmd���������
* @param  IN uint8_t auc_Buf[]:��������
* @param  IN uint16_t us_Len���������ݳ���
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
		/*��ʼ��W5300оƬ*/
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
*         ��������֡
* @param  IN uint8_t uc_Port������˿ں�,1-4��Ч  ;
* @param  IN uint8_t uc_Cmd���������
* @param  IN uint8_t auc_Buf[]:��������
* @param  IN uint16_t us_Len���������ݳ���
* @return void
*/
/*****************************************************************************/
static void  ParseCmd(IN uint8_t uc_Port,IN uint8_t uc_Cmd, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{  /*���ݶ�ȡ��Ϣ�Ĳ�ͬ��ִ�в�ͬ�Ĳ��� 20170808*/
	SOS_Assert(0 == uc_Port);
	
	SOS_Assert (uc_Port > PROTO_CHAN_MAX);
	
	SOS_Assert(NULL == auc_Buf);
	
	switch(uc_Cmd)	
	{
		case READ_CMD:		/*��������Ӧ����ͨ�Ű���CPU������������Ϣʱ��������Я��CPU��������Ϣ����*/
		{	/*�����Ϣ֡���Ƿ����������ݣ�����������*/
			if (guc_InitFlag != SYS_OK)
			{
				guc_InitFlag = AppTaskInit(auc_Buf, us_Len);
			}
			/*���������Ӧ��������ⲿ�ӿڽ�������*/
			processReadCmd(uc_Port,auc_Buf, us_Len);
			break;	
		}	
		case WRITE_CMD:	/*д���������д����������ȡ�ⲿ�ӿڷ�������*/
		{
			processWriteCmd(uc_Port, auc_Buf, us_Len);
			break;
		}
		case TERMINAL_CMD:	/*ά����Ϣ����*/
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
*         ������������������������ͷ����������ⲿ�ӿڽ��յ�������
* @param  IN uint8_t uc_Port������˿ں�,1-4��Ч ;
* @param  IN uint8_t auc_Buf[]:�������ݣ�
* @param  IN uint16_t us_Len���������ݳ��� OK
* @return void
*/
/*****************************************************************************/ 
static  void processReadCmd(IN uint8_t uc_Port , IN uint8_t auc_Buf[], IN uint16_t us_Len )
{/*���������ܵ���Ϣ�ټ���һЩ��Ϣ������͸�QJK 20170808*/
	uint8_t   uc_RevPort = 0;
	uint16_t  us_Cnt = 0;
	uint16_t  us_Sum = 0;
	int16_t   sh_Ret = 0;
	int16_t   sh_RemainLen = 0;
	uint8_t   uc_SndCnt = 0;
	SAckInfoHdr  *pst_InfoHdr = NULLS;
	
	SOS_Assert((0 == uc_Port)||(uc_Port > PROTO_CHAN_MAX));
	
	SOS_Assert( NULLS == auc_Buf );
	/*�������汾��Ϣ*/
	auc_SndMsg[0] = SOFT_VERSIONH;
	auc_SndMsg[1] = SOFT_VERSIONM;
	auc_SndMsg[2] = SOFT_VERSIONL;
	/*�����������ɳ�ʼ��������CPU�����������ݣ�����������������*/
	if ( SYS_YES == guc_InitFlag)     /*����������ȷ���Ǳ��SYS_YES 20170808*/
	{
		auc_SndMsg[3] = SYS_NO;
	}
	else
	{
		auc_SndMsg[3] = SYS_YES;
	}
	us_Sum = 0;
	
	pst_InfoHdr = (SAckInfoHdr *)&auc_SndMsg[4];   /*�����С��1000 20170808*/
	sh_RemainLen = SND_MTU_MAX - 4;/*��ȥǰ4�� 20170808*/
	while((uc_SndCnt < gst_AppInfor.uc_PortSum)&&((us_Sum + sizeof(SAckInfoHdr)) <=  sh_RemainLen))
	{	
		us_Sum = us_Sum + sizeof(SAckInfoHdr) - 1;
	
		uc_RevPort = gst_AppInfor.auc_PortNum[uc_SndCnt];
		
		/*�ж�д�������ܳ����Ƿ�Խ��*/

		pst_InfoHdr->uc_ComNum = uc_RevPort + 1;
	
		pst_InfoHdr->uc_State = (uint8_t)ExPort_Ioctl(uc_RevPort + 1 , STATE_COM_EXPORT);   /*��ȡ�ýӿڵ�ͨ��״̬ 20170808*/
	
		sh_Ret = ExPortMod_Rev( uc_RevPort + 1, uc_Port, pst_InfoHdr->auc_Data, sh_RemainLen - us_Sum - 4);  /*���ݶ���pst_InfoHdr->auc_Data 20170808*/
		
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
		pst_InfoHdr = (SAckInfoHdr *)(&pst_InfoHdr->auc_Data[us_Cnt]);    /*ָ����һƬ���� 20170808*/
		us_Sum = us_Sum + us_Cnt;
		uc_SndCnt++;
	}
	/*�����Ϣ������������Է�CPU���Ͷ���Ӧ����֡���ض�ȡ����*/

	ProtoMod_Send(uc_Port, READ_ACK, auc_SndMsg, us_Sum + 4);

}

/*****************************************************************************/
/**
* @brief  processWriteCmd
*         ����д�����CPU��д���ݱ��Ľ��н�������ȡ���ⲿ�ӿڷ�������
* @param  IN uint8_t uc_Port������˿ں�,1-4��Ч 
* @param  IN uint8_t auc_Buf[]:���뱨��;
* @param  IN uint16_t us_Len:���뱨�ĳ���
* @return void
*/
/*****************************************************************************/
static  void processWriteCmd(IN uint8_t uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len )
{   /*��us_Len������ͨ���������ͳ�ȥ 20170808*/
	uint8_t  uc_Ret = SYS_OK;
	uint16_t  us_Sum = 0;	/*�ѽ����������ݳ���*/
	uint16_t  us_Cnt = 0;
	SWriteCmdHdr* pst_CmdHdr = NULLS;
	/*�ж�������Ϣ�����Ƿ�������С֡Ҫ����Ƿ���CPU1�����������ݣ�ֻ��CPU1���͵����ݽ��д���*/

	if (CPUBAORD_1 != auc_Buf[0])
	{		
		uc_Ret = SYS_ERR;
	}
	us_Sum = us_Sum + 1;
	/*��������������ȡ���ⲿ�ӿڷ������ݳ��Ⱥ�������λ��*/	
	pst_CmdHdr = (SWriteCmdHdr*)(&auc_Buf[1]);		
	
	while( (uc_Ret == SYS_OK)&&((us_Sum + sizeof(SWriteCmdHdr) -1) < us_Len))  /*ֱ����us_Len�������ж���� 20170808*/
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
*         ����Ϣ������ȡ�ⲿ�˽ӿڷ�������֡д���ⲿ�ӿ�ģ�鷢�Ͷ�����
* @param  IN uint8_t  uc_Port:�ⲿ�ӿں�;
* @param  IN uint8_t auc_Buf[]:������Ϣ��
* @param  IN uint16_t us_Len��������Ϣ����
* @return uint8_t������--SYS_YES������崻�
*/
/*****************************************************************************/
static uint8_t  PickUpSndData(IN uint8_t  uc_Port, IN uint8_t auc_Buf[], IN uint16_t us_Len)
{
	uint16_t  us_Sum = 0;
	uint16_t  us_Ctr = 0;
	/* ��μ�� */
	
	SOS_Assert(NULLS == auc_Buf);
	
	/*���ذ巢���������ݿ��ܰ�����֡�����ⲿ�������ݣ������������ڽṹ��ȡ��֡����ʽΪ֡���ȣ�֡����*/
	while(us_Sum < us_Len)
	{
		us_Ctr = auc_Buf[us_Sum];
		us_Ctr = (us_Ctr<<8)|auc_Buf[us_Sum + 1];
		/*������ݳ���Ϊ�㣬Ϊ�쳣�����˳���ȡ����*/
		if (0 == us_Ctr)	
		{
			us_Sum = us_Len;	/*������ɣ�����û������ 20170808*/	
		}
		else
		{	/*�������ȡ�����ݼ��ϵ�ǰ��֡�ṹ���ݳ��ȳ���������Ϣ����Ϊ�쳣���ݲ�����д����*/
			if ((us_Sum + us_Ctr + 2) <= us_Len)
			{	/*ָ��֡��������λ��*/
				us_Sum = us_Sum + 2;
				/* д���������֡�洢���� */		
				ExPortMod_Send(uc_Port, &auc_Buf[us_Sum], us_Ctr);  /*ͨ�������ͽ�us_Ctr�����ݷ��ͳ�ȥ���൱����һ֡һ֡���ȵķ��� 20170808*/
			}
			us_Sum = us_Sum + us_Ctr;
		}
	}
	return  SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint32_t  IPChartToDWORD
*         IP��ַ����ת�����ֽ�����ת����32λ������
* @param  IN uint8_t auc_Buf[]��IP��ַ����
* @return uint32_t��32λIP��ַ��ʽ
*/
/*****************************************************************************/
static uint32_t  IPChartToDWORD(IN uint8_t auc_Buf[])  /*4��uint8_t ת����1��uint32_t 20170808*/
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

