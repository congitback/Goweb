/******************************************************************************/
/**
*@file    W5300.c       
*@brief   source file of device W5300 driver                   
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h" 
#ifdef __NET_CARD__
#include  <string.h>
#include  "dev_Gpio.h"
#include  "W5300.h"
#include  "dev_Timer.h"
#include  "dev_Interrupt.h"

/*****************************************************************************/
/**@brief  ����ӿ���Ϣ�ṹ                                                            */
/*****************************************************************************/
SNetInfor   gast_NetInfor[ETHERNET_MAX_NUM][W5300_SOCK_NUM];


/*****************************************************************************/
/**@brief  �ֲ���������                                                                                                                */
/*****************************************************************************/
static void  W5300_RST(uint8_t uc_Port);

static  void W5300_INTHandle(uint8_t uc_ChipNum);

static void W5300_SockWrite(uint8_t  uc_ChipNum, uint8_t uc_SockNum, SSockAddr *pst_SockAddr, 
							uint8_t auc_Buf[], uint16_t us_Len); 

static void  W5300_SockSendHandle(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum);

static void W5300_SockRevHandle(uint8_t  uc_ChipNum, uint8_t uc_SockNum);
/*****************************************************************************/
/**
* @brief  uint8_t W5300_Init
*         W5300��ʼ������ʼ������оƬ�ı���IP�����ء��������롢MAC��ַ
* @param  IN uint8_t  uc_Port������ӿڣ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint32_t  ui_IpAddr�����籾��IP��ַ
* @return uint8_t��SYS_YES---��������������崻�
*/
/*****************************************************************************/
uint8_t W5300_Init(IN uint8_t  uc_Port , IN uint32_t  ui_IpAddr,IN uint32_t ui_GateWay, IN uint32_t ui_Mask, IN uint8_t uc_Ver)
{
	volatile uint16_t *pus_Reg = NULLS;

	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);

		/*���ж�ģ��ע���жϴ�������*/
	ModINT_Rigster(W5300_INT_START_BIT + uc_Port, uc_Port, W5300_INTHandle);
	
	memset(gast_NetInfor[uc_Port], 0, sizeof(gast_NetInfor[uc_Port]));
	/*��λW5300*/
	W5300_RST(uc_Port);
	/*��ȡ���ؼĴ�����ַ������������*/
	pus_Reg =( volatile uint16_t*)GAR(uc_Port);					
	if (uc_Ver == VERSION1)
	{
		*pus_Reg = ui_IpAddr>>16;
		pus_Reg = pus_Reg + 2;
		*pus_Reg = (ui_IpAddr&0xFF00)|0x0001;	
	}
	else
	{
		*pus_Reg = ui_GateWay>>16;
		pus_Reg = pus_Reg + 2;
		*pus_Reg = ui_GateWay&0xFFFF;
	}
	/*��ȡSIPR�Ĵ�����ַ���б��������ַ����*/
	pus_Reg = ( volatile uint16_t*)SIPR(uc_Port);
	
	*pus_Reg = ui_IpAddr>>16;
	pus_Reg = pus_Reg + 2;
	*pus_Reg = (uint16_t)(ui_IpAddr&0xFFFF);
	
	/*��ȡSUBR�Ĵ�����ַ����������������*/
	pus_Reg = ( volatile uint16_t*)SUBR(uc_Port);
	if (uc_Ver == VERSION1)
	{
		*pus_Reg = (MASK_SUBNET>>16);
	
		pus_Reg = pus_Reg + 2;
	
		*pus_Reg = (uint16_t)(MASK_SUBNET&0xFFFF);		
	}
	else
	{
		*pus_Reg = (ui_Mask>>16);
	
		pus_Reg = pus_Reg + 2;
	
		*pus_Reg = (uint16_t)(ui_Mask&0xFFFF);
	}
	/*��ȡSHAR�Ĵ�����ַ����MAC��ַ����*/
	pus_Reg = ( volatile uint16_t*)SHAR(uc_Port);
	
	*pus_Reg = MAC_ADDR_12;
	
	pus_Reg = pus_Reg + 2;
	
	*pus_Reg = ui_IpAddr>>16;
	
	pus_Reg = pus_Reg + 2;
	
	*pus_Reg = (uint16_t)(ui_IpAddr&0xFFFF);
	/*����RTRֵ��RCR��ȷ���ط�ʱ��*/
	pus_Reg = ( volatile uint16_t*)RTR(uc_Port);
	*pus_Reg = 40;
	pus_Reg = ( volatile uint16_t*)RCR(uc_Port);
	*pus_Reg = 3;

	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint8_t  WD5300_SetSocketUDP
*         W5300Socket���ã�����SOCKET�ı��ض˿ںš�MTU������ΪUDPģʽ
* @param  IN uint8_t  uc_Port������ӿڣ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr�����ñ���SOCK����
* @return uint8_t��SYS_YES---��������������崻�
*/
/*****************************************************************************/
uint8_t  WD5300_SetSocketUDP(IN uint8_t uc_Port,IN uint8_t uc_SockNum, IN SSockAddr* pst_SockAddr)
{
	volatile  uint16_t  *ptr = NULLS;
	uint16_t  i = 0;
	uint8_t   uc_Ret = SYS_NO;
	uint16_t  us_Bit = 0x01;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	/* ���ñ��ض˿ں�*/
	ptr = ( volatile uint16_t*)SN_PORTR(uc_Port, uc_SockNum);
	*ptr= pst_SockAddr->sin_port;

	/* ����SOCKET���MTU */
	ptr=( volatile uint16_t*)SN_MSSR(uc_Port, uc_SockNum);
	*ptr = MTU_DEFAULT_W5300;
   	
	/* ����SOCKETΪUDPģʽ */
	ptr=( volatile uint16_t*)SN_MR(uc_Port, uc_SockNum);
	*ptr=SN_MR_UDP;

	/* ��SOCKRT*/
	ptr=( volatile uint16_t*)SN_CR(uc_Port, uc_SockNum);
	*ptr=SN_CR_OPEN;
	
	
	ptr=( volatile uint16_t*)SN_SSR(uc_Port, uc_SockNum);
	i=*ptr;

	if((i&0x00ff)!=SOCK_UDP)		/* If failed to open Socket0, close Socket0 and reurn false */
	{
		ptr=( volatile uint16_t*)SN_CR(uc_Port, uc_SockNum);
		
		*ptr=SN_CR_CLOSE;
		
		uc_Ret = SYS_NO;
		gast_NetInfor[uc_Port][uc_SockNum].uc_Init = SYS_NO;
	}
	else
	{
		gast_NetInfor[uc_Port][uc_SockNum].uc_Init = SYS_YES;
		uc_Ret = SYS_YES;
	}
	
	ptr = ( volatile uint16_t*)SN_IMR(uc_Port, uc_SockNum);
	*ptr = 0;
	*ptr |= (SOCKET_REV_INT_BIT|SOCKET_SENOK_TIMEROUT_BIT);
	
	ptr = ( volatile uint16_t*)IMR(uc_Port);
	*ptr |= (us_Bit<<uc_SockNum);
	
	return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  W5300_SockSnd
*         W5300Sock�ӿڷ���
* @param  IN uint8_t  uc_Port������ӿڣ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr��Ŀ�ĵ�ַ��
* @param  IN uint8_t auc_Buf[]��������Ϣ
* @param  IN uint16_t us_Len���������ݳ���
* @return uint8_t��SYS_YES---��������������崻�
*/
/*****************************************************************************/
uint8_t W5300_SockSnd(IN uint8_t  uc_Port, IN uint8_t uc_SockNum, IN SSockAddr *pst_SockAddr,IN uint8_t auc_Buf[], IN uint16_t us_Len)
{
	uint8_t  uc_Tail = 0;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	
	/*���SOCK�ӿ��ѳ�ʼ�������з��Ͳ���*/
	if (SYS_YES == gast_NetInfor[uc_Port][uc_SockNum].uc_Init)
	{	/*������ȳ������޻��ַΪ�㲻���з��Ͳ���*/
		if ((us_Len <= W300_REV_BUF_LEN)&&(pst_SockAddr->sin_addr != 0))
		{	/*�ٽ籣���ر��ж�*/
			OS_ENTER_CRITICAL();
			/*����Ѿ����������ж��򽫷�������д�뷢�Ͷ��������жϴ��������з��ͣ�����ֱ�ӽ��з��Ͳ���*/
			if (gast_NetInfor[uc_Port][uc_SockNum].uc_FistSnd == SYS_YES)
			{
				uc_Tail = gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail;
		
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].st_SockAddr = *pst_SockAddr;
			
				memcpy(gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].auc_Buf, auc_Buf, us_Len);
			
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].us_Len = us_Len;
			
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail 
						= (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail + 1)%MAX_W5300_QUEUE_LEN;
			
				if (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Cnt >= MAX_W5300_QUEUE_LEN)
				{
					gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Hdr 
						= (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
				}
				else
				{
					gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Cnt++;
				}
				
			}
			else
			{	/*����������д��оƬ���ʹ洢����*/
				W5300_SockWrite(uc_Port, uc_SockNum, pst_SockAddr, auc_Buf, us_Len);
				gast_NetInfor[uc_Port][uc_SockNum].uc_FistSnd = SYS_YES;
			}
			/*�˳��ٽ���*/
			OS_EXIT_CRITICAL();
		}
		else
		{
			
		}	
	}
	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint8_t  W5300_SockRev
*         W5300Sock�ӿ����ݶ�ȡ
* @param  IN uint8_t  uc_Port������ӿڣ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @param  OUT uint32_t*  pui_Addr���������ݵ�Դ��ַ��
* @param  OUT uint8_t auc_Buf[]�����ݴ洢��
* @param  IN uint16_t us_Len�����ݴ洢������
* @return uint16_t�����ض�ȡ���ݳ���
*/
/*****************************************************************************/
uint16_t   W5300_SockRev(IN uint8_t  uc_Port,IN uint8_t uc_SockNum, OUT uint32_t*  pui_Addr, 
						OUT uint8_t auc_Buf[],IN uint16_t us_Len)
{   /*��w5300��һ�ζ�ȡһ֡�����ݵ�auc_Buf�У����õ���ȡ�����ݳ��Ⱥ����ݵ�IP��ַ 20170808*/
	uint16_t  us_Ret = 0;
	uint8_t   us_Hdr = 0;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);        /*uc_Port ʹ�õ����Ŀ����� 20170808*/
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);      /*W5300��Socket��� 20170808*/
	
	SOS_Assert (pui_Addr == NULLS)                 /*��������Դ��ַRemountIP 20170808*/
	/*���SOCK�ӿ��ѳ�ʼ�����ɽ������ݶ�ȡ*/
	if (gast_NetInfor[uc_Port][uc_SockNum].uc_Init == SYS_YES)
	{	/*�ٽ籣���ر��ж�*/
		OS_ENTER_CRITICAL();
		/*���ն������Ƿ������ݣ����н��ж�ȡ*/
		if (gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Cnt > 0)
		{
			us_Hdr = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr;
			/*���մ洢�������Ƿ�������������ݰ��ĳ��ȣ����������ȡ���ݷ��򷵻����ݳ���Ϊ0*/
			if (us_Len > gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len)
			{
				memcpy(auc_Buf,gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].auc_Buf,
						gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len );
						
				us_Ret = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len;
				
				*pui_Addr = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].ui_Addr;
				
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len = 0;
				
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr = (gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Cnt--;
			}
			else
			{
				*pui_Addr = 0;
				us_Ret = 0;
			}
		}
		else
		{
			*pui_Addr = 0;
			us_Ret = 0;
		}
		/*�˳��ٽ���*/
		OS_EXIT_CRITICAL();
	}
	else
	{/*���������˳� 20170808*/
		*pui_Addr = 0;
		us_Ret = 0;
	}	
	return us_Ret;	
}

/*****************************************************************************/
/**
* @brief  W5300_SockSendHandle
*         ���ʹ���
* @param  IN uint8_t  uc_ChipNum��оƬ�ţ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @param  OUT uint32_t*  pui_Addr���������ݵ�Դ��ַ��
* @param  OUT uint8_t auc_Buf[]�����ݴ洢��
* @param  IN uint16_t us_Len�����ݴ洢������
* @return uint16_t�����ض�ȡ���ݳ���
*/
/*****************************************************************************/
static void  W5300_SockSendHandle(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum)
{
	uint8_t   uc_Hdr = 0;
	SSockAddr st_SockAddr ;
	uint16_t  us_Len = 0;
	
	SOS_Assert (uc_ChipNum >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	/*���Ͷ������Ƿ������ݣ�����ȡ���ݽ��н��з��ͣ��������״η���Ϊ��*/
	if (gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Cnt > 0)	
	{
		uc_Hdr = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr;
		us_Len = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].us_Len;
		st_SockAddr = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].st_SockAddr;
		/*����������д��оƬSOCK�ӿڴ洢����*/
		W5300_SockWrite(uc_ChipNum, uc_SockNum, &st_SockAddr,
						gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].auc_Buf, us_Len);
						
		gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr 
					= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
		gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Cnt--;	
	}
	else
	{	
		gast_NetInfor[uc_ChipNum][uc_SockNum].uc_FistSnd = SYS_NO;
	}
}

/*****************************************************************************/
/**
* @brief  W5300_SockSendHandle
*         ���ʹ���
* @param  IN uint8_t  uc_ChipNum��оƬ�ţ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr���������ݵ�Ŀ�ĵ�ַ��
* @param  IN uint8_t auc_Buf[]����������
* @param  IN uint16_t us_Len�����ݳ���
* @return void
*/
/*****************************************************************************/
static void W5300_SockWrite(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum,IN SSockAddr *pst_SockAddr,IN uint8_t auc_Buf[],IN uint16_t us_Len) 
{
	volatile uint16_t *ptr = NULLS;
	uint16_t i = 0U; 
	uint16_t us_Data = 0U;
	uint16_t us_Sum = 0U;
	
	SOS_Assert (uc_ChipNum >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	/*��ȡĿ��IP�Ĵ�����ַ*/
	ptr = ( volatile uint16_t*)SN_DIPR(uc_ChipNum, uc_SockNum);
	/*����Ŀ��IP�Ĵ���*/
	*ptr = pst_SockAddr->sin_addr>>16;
	ptr = ptr +2;
	*ptr = (uint16_t)(pst_SockAddr->sin_addr&0XFFFF);
	/*����Ŀ�Ķ˿ں�*/
	ptr=( volatile uint16_t*)SN_DPORTR(uc_ChipNum, uc_SockNum);
	
	*ptr= pst_SockAddr->sin_port;
	/*��16bit����*/
	if(us_Len&0x0001)
	{
		us_Sum=(us_Len+1)/2;
	}
	else
	{
		us_Sum=us_Len/2;
	}
	/*��ȡSOCKET����FIFO�Ĵ�����ַ*/
	ptr=( volatile uint16_t*)SN_TX_FIFOR(uc_ChipNum, uc_SockNum);
	
	for(i=0; i<us_Sum;i++)
	{
		us_Data = (auc_Buf[2*i]<<8)|(auc_Buf[2*i + 1]);
		*ptr = us_Data;
	}
	/*���÷������ݳ���*/
	ptr=( volatile uint16_t*)SN_TX_WRSR(uc_ChipNum, uc_SockNum);
	*ptr =0;
	ptr = ptr  + 2;
	*ptr = us_Len;
	/*��������*/
	ptr=( volatile uint16_t*)SN_CR(uc_ChipNum, uc_SockNum);
	*ptr= SN_CR_SEND;
	
	return ;
}



/*****************************************************************************/
/**
* @brief  W5300_SockRevHandle
*         ���մ�����ȡ����BUF
* @param  IN uint8_t  uc_ChipNum��оƬ�ţ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @param  IN uint8_t uc_SockNum:SOCKET�ţ�0- W5300_SOCK_NUM
* @return void
*/
/*****************************************************************************/
static void W5300_SockRevHandle(uint8_t  uc_ChipNum, uint8_t uc_SockNum)
{
	volatile uint16_t *ptr = NULLS;
	uint16_t us_Sum = 0;
	uint16_t rx_size =  0;
	uint16_t us_Len = 0;
	uint16_t i = 0;
	uint8_t  uc_Tail = 0;
	uint16_t us_Data = 0;
	uint16_t us_Cnt = 0;
	uint8_t  uc_AlFlag = SYS_NO;
	/*���SOCK�ӿ��ѳ�ʼ�����ɽ������ݶ�ȡ*/
	if (gast_NetInfor[uc_ChipNum][uc_SockNum].uc_Init == SYS_YES)
	{	/*��ȡ����״̬�Ĵ�����ַ��ȡ�������ݳ���*/
		ptr=( volatile uint16_t*)SN_RX_RSR(uc_ChipNum, uc_SockNum);
				
		us_Sum=*ptr;
		ptr = ptr + 2;
		us_Sum=*ptr;
		/*�������������С���Ƚ��н�����������һ���������ڿ����ж������֡����ȡ�˳�����Ϊ����������ȫ����ȡ��ɣ�*/
		while( us_Sum >= 8)
		{	/*��ȡ����FIFO�Ĵ�����ַ*/				
			ptr=( volatile uint16_t*)SN_RX_FIFOR(uc_ChipNum, uc_SockNum);
			uc_Tail = gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail;
			/*��ȡ��������Դ��ַ*/	
			gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr = *ptr;
			gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr 
					= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr<<16)|*ptr;
			/*��ȡ��������֡����*/
			us_Data = *ptr;	/*�˴�Ϊ��ȡ�˿ںţ��˿���Ϣ���Բ���*/
			rx_size = *ptr;
			/*16bit����*/
			if(rx_size&0x0001)
			{
				us_Len = (rx_size+1)/2; 
				uc_AlFlag = SYS_YES;
				//us_Len = us_Len -1;
			}
			else
			{
				us_Len = rx_size/2;
				uc_AlFlag = SYS_NO;
			}
			/*�����������������ն���BUF����Ҫ��������ݶ�ȡ��������пն�ȡ����*/
			if (rx_size < W300_REV_BUF_LEN)
			{	us_Cnt = 0;
				for(i=0; i<us_Len; i++)
				{	/*��ȡ���ݽ�16��������ת��Ϊ8�������ݴ洢*/
					us_Data = *ptr ;
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].auc_Buf[us_Cnt] = us_Data>>8;
					us_Cnt++;
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].auc_Buf[us_Cnt] = us_Data&0xFF;
					us_Cnt++;
				}
				/*��¼��ȡ����֡����*/
				if (uc_AlFlag == SYS_YES)
				{
					us_Cnt = us_Cnt -1;
				}
				else
				{
					
				}
				gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].us_Len = us_Cnt;
				gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail 
						= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail + 1)%MAX_W5300_QUEUE_LEN;
				/*������ն���������ɾ��������֡*/	
				if(gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Cnt>= MAX_W5300_QUEUE_LEN)
				{
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Hdr 
								= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;						
				}
				else
				{
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Cnt++;
				}
			}
			else
			{	/*��ȡ�ղ���*/
				for(i=0; i<us_Len; i++)
				{
					us_Data = *ptr ;
				}
			}
			
			us_Sum = us_Sum - rx_size - 8;
		}
		ptr=( volatile uint16_t*)SN_CR(uc_ChipNum, uc_SockNum);		/* Set RECV command */
		*ptr=SN_CR_RECV;
	}
}

/*****************************************************************************/
/**
* @brief  void W5300_INTHandle
*         ����оƬ�жϴ���
* @param  IN uint8_t  uc_ChipNum��оƬ�ţ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @return void
*/
/*****************************************************************************/
static  void W5300_INTHandle(uint8_t uc_ChipNum)
{
	volatile uint16_t*   puc_IRReg = NULLS;	
	volatile uint16_t*   puc_SockIRReg = NULLS;
	volatile uint16_t    us_IR = 0;
	volatile uint16_t    us_SockIR = 0;
	uint16_t    us_Mask = 0x01;
	uint8_t     uc_Cnt = 0;
	/*��ȡ�жϱ�־�Ĵ�������ȡ�жϱ�־��Ϣ*/
	puc_IRReg = ( volatile uint16_t*)IR(uc_ChipNum);
	us_IR = *puc_IRReg;
	/*ɨ������оƬ����SOCK��ȷ�������жϵ�SOCKET���в���*/
	for(uc_Cnt = 0; uc_Cnt < W5300_SOCK_NUM; uc_Cnt++)
	{
		if (us_IR&us_Mask)
		{	/*��ȡSOCKET�жϱ�־�Ĵ���*/
			puc_SockIRReg = ( volatile uint16_t*)SN_IR(uc_ChipNum, uc_Cnt);
			us_SockIR = *puc_SockIRReg;
			/*����ǳ�ʱ�жϻ����ж�ִ�з�������*/
			if(us_SockIR&SOCKET_SENOK_TIMEROUT_BIT)
			{	/*����жϱ�־λ*/
				*puc_SockIRReg = SOCKET_SENOK_TIMEROUT_BIT;
					
				W5300_SockSendHandle(uc_ChipNum, uc_Cnt);
			}
			/*����ǽ����ж�ִ�н��մ���*/
			if (us_SockIR&SOCKET_REV_INT_BIT)
			{
				W5300_SockRevHandle(uc_ChipNum, uc_Cnt);
				/*����жϱ�־λ*/
				*puc_SockIRReg = SOCKET_REV_INT_BIT;
			}
		}
		us_Mask = us_Mask<<1;
	}
}
/*****************************************************************************/
/**
* @brief  void W5300_INTHandle
*         ����оƬ��λ
* @param  IN uint8_t uc_Port���ӿںţ�0-ETHERNET_MAX_NUM����Ӧ��ͬ������оƬ
* @return void
*/
/*****************************************************************************/
static void  W5300_RST(IN uint8_t uc_Port)
{
	uint16_t us_Bit = 0;
	
	us_Bit = W5300_RST_BIT<<uc_Port;
	
	asm(" EALLOW");
	REG_GPIO_GPBMUX &= (~us_Bit);
	REG_GPIO_GPBDIR |= us_Bit;	
	asm(" EDIS");
	
	REG_GPIO_GPBDAT |= us_Bit;
	
	delay10Us(10);
	
	REG_GPIO_GPBDAT &= (~us_Bit);
	
	
	delay10Us(20);
	
	REG_GPIO_GPBDAT |= us_Bit;
}
#endif


