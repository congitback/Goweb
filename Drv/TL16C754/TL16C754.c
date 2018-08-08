/******************************************************************************/
/**
*@file    TL16C754.c      
*@brief   source file of device TL16C754 driver                   
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/

#include  "Typedef.h"
#include  "Queue.h"
#include  "dev_Timer.h"
#include  "TL16C754.h"
#include  "dev_Gpio.h"
#include  "dev_Interrupt.h"
#include  <string.h>

/*****************************************************************************/
/**@brief  �ⲿ�����豸�ṹ����                                                         */
/*****************************************************************************/
static SExUartDev     gst_ExUartDev[MAX_TL16C754_DEV_NUM];

/*****************************************************************************/
/**@brief  �ⲿ�����豸���ն���                                                         */
/*****************************************************************************/
SExUartRevBuf  gst_ExUartRevBuf[MAX_TL16C754_DEV_NUM];

/*****************************************************************************/
/**@brief  �ⲿ�����豸���Ͷ���                                                         */
/*****************************************************************************/
SExUartSndBuf  gst_ExUartSndBuf[MAX_TL16C754_DEV_NUM];

#ifdef __NET_CARD__
/*****************************************************************************/
/**@brief  �ⲿ�����豸��������������                                                   */
/*****************************************************************************/
static const  uint32_t   gst_ExUartBaud[MAX_TL16C754_DEV_NUM] =
							{BAUD_TL16C754_CHAN1, BAUD_TL16C754_CHAN2, BAUD_TL16C754_CHAN3, BAUD_TL16C754_CHAN4};

/*****************************************************************************/
/**@brief   �ⲿ�����豸���ã� 1~(MAX_TL16C754_DEV_NUM-1)��Ч��0δ��                               */
/*****************************************************************************/
static const  uint8_t    gst_ExUartConf[MAX_TL16C754_DEV_NUM] = 
							{ EXUART_CONFIG_CHAN1, EXUART_CONFIG_CHAN2, EXUART_CONFIG_CHAN3, EXUART_CONFIG_CHAN4};
#else
/*****************************************************************************/
/**@brief  �ⲿ�����豸�������������� 1~(MAX_TL16C754_DEV_NUM-1)��Ч��0δ��                  */
/*****************************************************************************/
static const  uint32_t   gst_ExUartBaud[MAX_TL16C754_DEV_NUM] =
							{BAUD_TL16C754_CHAN1, BAUD_TL16C754_CHAN2, BAUD_TL16C754_CHAN3, BAUD_TL16C754_CHAN4,
							 BAUD_TL16C754_CHAN5, BAUD_TL16C754_CHAN6, BAUD_TL16C754_CHAN7, BAUD_TL16C754_CHAN8};
/*****************************************************************************/
/**@brief  �ⲿ�����豸���ã� 1~(MAX_TL16C754_DEV_NUM-1)��Ч��0δ��                                */
/*****************************************************************************/
static const  uint8_t    gst_ExUartConf[MAX_TL16C754_DEV_NUM] = 
							{EXUART_CONFIG_CHAN1, EXUART_CONFIG_CHAN2, EXUART_CONFIG_CHAN3, EXUART_CONFIG_CHAN4,
							 EXUART_CONFIG_CHAN5, EXUART_CONFIG_CHAN6, EXUART_CONFIG_CHAN7, EXUART_CONFIG_CHAN8};
#endif
/*****************************************************************************/
/**@brief  �ֲ���������                                                                                                                */
/*****************************************************************************/							
static void  setDivisorLatchTL16C754(IN uint8_t uc_ChanNum, IN uint16_t us_Value);

static void setXOFF1andXON1TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue);

static void setXOFF2andXON2TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue);

static void setEFRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);

static void setTCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);	

static void setTLRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);						

static void setFCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);	

static void  setIERTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);

static void  dev_ExUartINTHandle(IN uint8_t uc_Minor);	

static  void  dev_ExUartRevHanle(IN uint8_t  uc_Minor);	

static  void  dev_ExUartSndHanle(IN uint8_t  uc_Minor); 

 /*****************************************************************************/
/**
* @brief  uint8_t dev_ExUartOpen
*        	��ɴ������ݽṹ��ʼ����Ӳ����ʼ��
* @param  IN uint8_t  uc_Minor: �豸��ţ���ʾͬ���豸���� 
* @param  IN int8_t sc_OpFlag����ѡ�δ�� 
* @return uint8_t : SYS_YES----�ɹ�, SYS_NO----ʧ��
*/
/*****************************************************************************/
uint8_t dev_ExUartOpen(IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag)							
{
	volatile uint8_t*  puc_LCR = NULLS;
	volatile uint8_t*  puc_MCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t   uc_Ret = SYS_OK;
	uint16_t  us_Baud = 0U;

	/* ����ж� */
 	SOS_Assert( 0U == uc_Minor );
 		
 	SOS_Assert( uc_Minor > MAX_TL16C754_DEV_NUM );
 	uc_Minor = uc_Minor -1;
	/* �Ĵ�����ַ���� */
	ui_Addr = ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;     /*��Ҫ�򿪵ĵ�ǰ�豸�ĵ�ַ���� 20170808*/
 	puc_LCR = (volatile uint8_t*)(ui_Addr);
 	*puc_LCR = gst_ExUartConf[uc_Minor];                                              /*�ڸõ�ַд����豸��ֵ 20170808*/
 	
 	ui_Addr = ADDR_BASE_TL16C754 + uc_Minor *SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
 	
 	if ( 0 == gst_ExUartBaud[uc_Minor] )
 	{
 		uc_Ret = SYS_ERR;
 	}
 	else
 	{
 		ModINT_Rigster(uc_Minor, uc_Minor, dev_ExUartINTHandle);                 /*���Ϊ�жϴ����������ж���������ע���жϺ��� 20170808*/
 		puc_MCR = (volatile uint8_t*)(ui_Addr);
		*puc_MCR |= 0x008;
 		us_Baud = (CRYSTAL_FREQUENCY_TL16C754/SEL_CLOCK_TL16C754)/(gst_ExUartBaud[uc_Minor]*16)	;
 	
 		/* ���ô���Ƶ�� */
 		setDivisorLatchTL16C754(uc_Minor,us_Baud);           /*��Ӧ���ڲ����� 20170808*/
 		
 		setEFRTL16C754(uc_Minor, 0);
 	
 		setXOFF1andXON1TL16C754(uc_Minor, 0,0);
 	
 		setXOFF2andXON2TL16C754(uc_Minor, 0,0);
 	
 		setTCRTL16C754(uc_Minor, 0);
 	
 		setTLRTL16C754(uc_Minor, 0);
 	
 		setFCRTL16C754(uc_Minor, 0x07);
 		
 		
 	
 		/* ��ʼ�����ݽṹ���� */
 		memset(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall, 0, sizeof(gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall));
 		
 		Queue_Init(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr,QUEUESMALL_LEN_MAX);
 	
 		memset(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge, 0, sizeof(gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge)); 	
 		
 		Queue_Init(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, QUEUELARGE_LEN_MAX);
 		
 		gst_ExUartDev[uc_Minor].us_ErrCode = 0;
 		gst_ExUartDev[uc_Minor].ui_ErrCtr = 0;
 		gst_ExUartRevBuf[uc_Minor].ui_Timer = getTick();
 		gst_ExUartRevBuf[uc_Minor].uc_State = SYS_ERR;
 		gst_ExUartDev[uc_Minor].uc_Init = SYS_OK;
 		
 		setIERTL16C754(uc_Minor, 0x01);                     /*���ж�  20170808*/
 	}
 	return uc_Ret;	
} 

/*****************************************************************************/
/**
* @brief  uint8_t dev_ExUartWrite
*         �ⲿ����ģ��д�ӿڣ������ϲ㽫��������д�������㷢�Ͷ��� 
* @param  IN uint8_t uc_Minor�� ���豸�ţ������豸���� 
* @param  IN char_t  auc_Buf[]:��������
* @param  IN nt32_t i_Len���������ݳ���
* @return SYS_YES---д�����ɹ� ,SYS_NO---д����ʧ��
*/
/*****************************************************************************/
uint8_t dev_ExUartWrite( IN uint8_t uc_Minor, IN char_t ac_Buf[], IN int32_t i_Len)
{
	uint8_t   uc_Ret = SYS_OK;
	volatile   uint8_t   *puc_IER = NULLS;
	/* ��μ�� */
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);
	
	SOS_Assert(NULLS == ac_Buf);
	uc_Minor = uc_Minor -1;
	if ( SYS_OK == gst_ExUartDev[uc_Minor].uc_Init )
	{	
		puc_IER = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor *SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR);
		/*�ٽ籣���ر��ж�*/
		OS_ENTER_CRITICAL();
		/* ����������֡д�뷢�Ͷ����� */
		uc_Ret = Queue_Write(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, (uint8_t*)ac_Buf,  i_Len);
		/*ʹ�ܴ��ڷ����ж�*/
		*puc_IER |= TL16C754_SND_INT_ENABLE;
		
		if (uc_Ret == SYS_OK)
		{		
		}
		else
		{	
			gst_ExUartDev[uc_Minor].ui_ErrCtr++;
		}
		/*�ٽ籣���˳����ж�*/
		OS_EXIT_CRITICAL();
	}
	else
	{
		uc_Ret = SYS_ERR;	
	}

	return uc_Ret;
} 

/*****************************************************************************/
/**
* @brief  int32_t dev_ExUartRead
*         �ⲿ����ģ����ӿڣ������ϲ��ȡ��������ն��������� 
* @param  IN uint8_t uc_Minor�� ���豸�ţ������豸���� 
* @param  OUT char_t auc_Buf[]:���ݷ��ش洢��
* @param  IN int32_t i_Len���洢������
* @return int32_t��-1---����>=0---��ȡ�����ݳ���
*/
/*****************************************************************************/
int32_t dev_ExUartRead(IN uint8_t uc_Minor, OUT char_t ac_Buf[],IN int32_t i_Len)
{
	int32_t  i_Ret = -1; 
	/* ��μ�� */
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);

	SOS_Assert(NULLS == ac_Buf);	
	uc_Minor = uc_Minor -1;
	if (SYS_OK == gst_ExUartDev[uc_Minor].uc_Init)
	{
		if (SYS_OK == gst_ExUartRevBuf[uc_Minor].uc_State)
		{	/*�ٽ籣���ر��ж�*/
			OS_ENTER_CRITICAL();
			
			/* ��ȡ���ڽ��ն������� */
			i_Ret = Queue_Read(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr, (uint8_t*)ac_Buf, i_Len); 
			/*�ٽ籣���˳����ж�*/
			OS_EXIT_CRITICAL();
		}
		else
		{
			i_Ret =  -1;		
		}
	}
	else
	{
		i_Ret = -1;
	}
	
	return i_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  dev_ExUartIoctl
*         �ⲿ���ڿ��Ʋ����ӿڣ������ϲ��ȡ�豸״̬������״̬ 
* @param  IN uint8_t uc_Minor�� ���豸�ţ������豸����  
* @param  IN uint32_t ui_Arg:   ��ȡ���� ��TL16C754_DATA_VALID---��ȡ���ն���������Ч�ԣ�
* 								TL16C754_COM_STATE---��ȡͨ��״̬��TL16C754_ERR_CODE---��ȡ������
* @param  OUT uint16_t *pus_Data:״̬����
* @return uint8_t SYS_YES---���Ʋ����ɹ� ,SYS_NO---���Ʋ���ʧ��
*/
/*****************************************************************************/
uint8_t  dev_ExUartIoctl(IN uint8_t uc_Minor, IN uint32_t i_Arg, uint16_t *pus_Data)
{
	uint8_t uc_Ret = SYS_OK;
	
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);

	SOS_Assert(NULLS == pus_Data);	
	uc_Minor = uc_Minor -1;
	switch(i_Arg)
	{
		case TL16C754_DATA_VALID: /* ��ȡ���ն���������Ч�� */
		{
			*pus_Data = gst_ExUartRevBuf[uc_Minor].uc_State;
			break;	
		}	
		case  TL16C754_ERR_CODE:/* ��ȡ������ */
		{	
			*pus_Data = gst_ExUartDev[uc_Minor].us_ErrCode;
			break;	
		}
		default:
		{
			uc_Ret = SYS_ERR;
			break;	
		}
	}
	
	return uc_Ret;		
}

/*****************************************************************************/
/**
* @brief  uint8_t  dev_ExUartCheck
*         �ⲿ����ģ���⣬������д���ͨ��״̬ 
* @param  void 
* @return uint8_t�� SYS_YES---�����ɹ� ,SYS_NO---����ʧ��
*/
/*****************************************************************************/
uint32_t  dev_ExUartCheck(void)   /*2s ���� ʱ���޶� 20170808*/
{
	uint8_t  uc_Count = 0U;
	uint32_t ui_ErrCode = 0U;
	
	for (uc_Count = 0; uc_Count < MAX_TL16C754_DEV_NUM; uc_Count++)
	{
		if (SYS_OK == gst_ExUartDev[uc_Count].uc_Init)
		{	
			if (SYS_OK == IsTimeOut(TIMER_OVER_COM_TL16C754, gst_ExUartRevBuf[uc_Count].ui_Timer))
			{
				gst_ExUartRevBuf[uc_Count].uc_State = SYS_ERR;	
			}
			else
			{	
			}
		}
		else
		{
			gst_ExUartRevBuf[uc_Count].uc_State = SYS_ERR;	
		}
	}	
	return ui_ErrCode;
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartINTHandle
*         �����жϴ�����ɴ����ж�״̬ɨ�衢���ݽ����뷢��
* @param  IN uint8_t uc_Minor�����豸�� 
* @return void
*/
/*****************************************************************************/
static void  dev_ExUartINTHandle(IN uint8_t uc_Minor)
{
	volatile uint8_t* puc_IIR = NULLS;
	volatile uint8_t   uc_INTFlag = 0;
	
	if (SYS_OK == gst_ExUartDev[uc_Minor].uc_Init)
	{	/*��ȡ�жϱ�־�Ĵ���*/
		puc_IIR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor * SPACE_ADDR_CHANNEL + IIR_RELATIVE_ADDR);
		uc_INTFlag = *puc_IIR;
		/*�����жϽ��봦��,�˳������������ж��¼�������ɣ��жϱ�־λΪ��*/
		while((uc_INTFlag&MASK_INT_TLC754) != 0)
		{	/*��ѯ�Ƿ��ǽ����ж�*/
			if ((uc_INTFlag&RHR_INTERRUPT)==RHR_INTERRUPT)
			{	/*�������ݽ��մ���*/
				dev_ExUartRevHanle(uc_Minor);	
			}
			/*��ѯ�Ƿ��Ƿ����ж�*/
			if ((uc_INTFlag & THR_INTERRUPT)==THR_INTERRUPT)
			{	/*�������ݷ��ʹ���*/
				dev_ExUartSndHanle(uc_Minor);
			}
			uc_INTFlag = *puc_IIR;
		}
	}
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartRevHanle
*         ���ڽ��մ�����ɴ���FIFO��ȡ
* @param  IN uint8_t uc_Minor�����豸�� 
* @return void
*/
/*****************************************************************************/
static  void  dev_ExUartRevHanle(IN uint8_t  uc_Minor)
{
	volatile uint8_t*  puc_LSR = NULLS;
	volatile uint8_t*  puc_RHR = NULLS;
	volatile uint8_t   uc_RHRState = 0;
	uint8_t   auc_Data[TH_REV_DATA_CNT_TL16C754];
	int32_t   i_Len = 0;
	/*��ȡ����״̬�Ĵ�����ַ*/
	puc_LSR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + LSR_RELATIVE_ADDR); 
	/*��ȡ���ռĴ�����ַ*/		
	puc_RHR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + RHR_RELATIVE_ADDR);
	/*��ȡ����״̬*/	
	uc_RHRState = (*puc_LSR) & MASK_RXHOLD_STATE_LSR;
	i_Len = 0;
	/*����н���FIFO�����ݽ��ж�ȡ����ȡ�˳����������ݶ�ȡ��ɻ����ݽ��ջ�����*/
	while((MASK_RXHOLD_STATE_LSR == uc_RHRState)&&(i_Len < TH_REV_DATA_CNT_TL16C754))
	{	
		auc_Data[i_Len] = *puc_RHR&0XFF;
		i_Len++;
		uc_RHRState = (*puc_LSR)&MASK_RXHOLD_STATE_LSR;
	}
	/*���ȡ���ݽ�����д����ն�����*/
	if ( i_Len > 0 )
	{	
		if( SYS_OK == Queue_Write(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr
								, auc_Data , i_Len))
		{
			gst_ExUartRevBuf[uc_Minor].ui_Timer= getTick();
			gst_ExUartRevBuf[uc_Minor].uc_State = SYS_OK;
		}
		else
		{
		
		}				
	}			
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartSndHanle
*         ���ڷ���������ɴ�������д�뷢��FIFO��
* @param  IN uint8_t uc_Minor�����豸�� 
* @return void
*/
/*****************************************************************************/
static  void  dev_ExUartSndHanle(uint8_t  uc_Minor) 
{
	volatile uint8_t* puc_THR = NULLS;
	volatile uint8_t* puc_IER = NULLS;
	uint8_t   auc_Data[TH_REV_DATA_CNT_TL16C754];
	uint8_t   uc_Cnt = 0;
	int32_t   i_Ret = 0;
	/*��ȡ���ڷ��͵�ַ*/
	puc_THR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + THR_RELATIVE_ADDR); 
	/*�ӷ��Ͷ����ж�ȡ���ݣ�ÿ�ζ�ȡTH_SND_DATA_NUM_TL16C754����*/		
	i_Ret = Queue_Read(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, auc_Data, TH_SND_DATA_NUM_TL16C754);
	/*��������ݽ���д�뷢��FIFO�У�����رշ����ж�*/			
	if (i_Ret > 0)
	{
		for (uc_Cnt = 0; uc_Cnt < i_Ret; uc_Cnt++)
		{						
			*puc_THR = auc_Data[uc_Cnt];
		}
	}
	else
	{
		puc_IER = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR);
		*puc_IER &= (~TL16C754_SND_INT_ENABLE);
	}	
}
/*****************************************************************************/
/**
* @brief  void  setDivisorLatchTL16C754
*         ����TL16C754�������໷�Ĵ�
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint16_t us_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void  setDivisorLatchTL16C754(IN uint8_t uc_ChanNum, IN uint16_t us_Value)  
{                           /*���ܺ�������������TL16C754����Ҳ�ѹִ��ڵĲ����� 20170808*/
	volatile uint8_t *puc_DLL = NULLS;
	volatile uint8_t *puc_DLH = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + DLL_RELATIVE_ADDR;
	puc_DLL = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + DLH_RELATIVE_ADDR;
	puc_DLH = (volatile uint8_t*)(ui_Addr);
		
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR |= 0x80U;
	*puc_DLL = us_Value&0xffU;
	*puc_DLH = (us_Value>>8)&0xffU;
	*puc_LCR = uc_Tmp;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setXOFF1andXON1TL16C754
*         ����TL16C754����XOFF1�Ĵ�����XON1�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_XoffValue��XOFF1�Ĵ���д��ֵ
* @param  IN uint8_t uc_XonValue��XON1�Ĵ���д��ֵ
* @return void
*/
/*****************************************************************************/
static void setXOFF1andXON1TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue)
{
	volatile uint8_t *puc_XOFF1 = NULLS;
	volatile uint8_t *puc_XON1 = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XOFF1_RELATIVE_ADDR;
	puc_XOFF1 = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + XON1_RELATIVE_ADDR;
	puc_XON1 = (volatile uint8_t*)(ui_Addr);	
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_XOFF1 = uc_XoffValue;
	*puc_XON1 = uc_XonValue;
	*puc_LCR = uc_Tmp;
	
	return;
}
/*****************************************************************************/
/**
* @brief  void setXOFF2andXON2TL16C754
*         ����TL16C754����XOFF1�Ĵ�����XON1�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_XoffValue��XOFF2�Ĵ���д��ֵ
* @param  IN uint8_t uc_XonValue��XON2�Ĵ���д��ֵ
* @return void
*/
/*****************************************************************************/
static void setXOFF2andXON2TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue)
{
	volatile uint8_t *puc_XOFF2 = NULLS;
	volatile uint8_t *puc_XON2 = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + XOFF2_RELATIVE_ADDR;
	puc_XOFF2 = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XON2_RELATIVE_ADDR;
	puc_XON2 = (volatile uint8_t*)(ui_Addr);	
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_XOFF2 = uc_XoffValue;
	*puc_XON2 = uc_XonValue;
	*puc_LCR = uc_Tmp;

	return;
}
/*****************************************************************************/
/**
* @brief  void setEFRTL16C754
*         ����TL16C754����EFR�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void setEFRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XOFF2_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_Value;
	*puc_LCR = uc_Tmp;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setTCRTL16C754
*         ����TL16C754����TCR�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void setTCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_MCR = NULLS;
	volatile uint8_t *puc_TCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	uint8_t uc_TmpMCR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);

	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
	puc_MCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + TCR_RELATIVE_ADDR;
	puc_TCR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;	
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
	
	uc_TmpMCR = *puc_MCR;
	
	*puc_MCR |= 0x40U; 
	*puc_TCR = uc_Value;
	*puc_MCR = uc_TmpMCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;

	return;
}


/*****************************************************************************/
/**
* @brief  void setTCRTL16C754
*         ����TL16C754����TLR�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void setTLRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_MCR = NULLS;
	volatile uint8_t *puc_TLR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	uint8_t uc_TmpMCR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
	puc_MCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + TLR_RELATIVE_ADDR;
	puc_TLR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
		
	uc_TmpMCR = *puc_MCR;
	*puc_MCR |= 0x40U;
	*puc_TLR = uc_Value;
	
	*puc_MCR = uc_TmpMCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setFCRTL16C754
*         ����TL16C754����FCR�Ĵ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void setFCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_FCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + FCR_RELATIVE_ADDR;
	puc_FCR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
	*puc_FCR = uc_Value;
	*puc_LCR = 0xBF;
	
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;

	return;
}
/*****************************************************************************/
/**
* @brief  void setIERTL16C754
*         ����TL16C754�����ж�ʹ�ܼĴ���
* @param  IN uint8_t uc_ChanNum�����ںţ�0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value��д��ֵ
* @return void
*/
/*****************************************************************************/
static void  setIERTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_IER = NULLS;
	uint32_t  ui_Addr = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR;
	puc_IER = (volatile uint8_t*)(ui_Addr);

	*puc_IER = uc_Value;
	
	return;
}
