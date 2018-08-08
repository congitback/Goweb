/******************************************************************************/
/**
*@file    dev_Intrupts.c       
*@brief   �ж�ģ�����                    
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief   include                                                    	     */
/*****************************************************************************/
#include  <string.h>
#include  "Typedef.h"
#include  "dev_Interrupt.h"
#include  "dev_Gpio.h"

/*****************************************************************************/
/**@brief   �жϴ���������                                                    	     */
/*****************************************************************************/
SInterruptVector   gast_INTVect[INTERRUPT_SRC_NUM];

/*****************************************************************************/
/**@brief   �ֲ���������                                                     	     */
/*****************************************************************************/
static interrupt void ModINT_Handle(void);

/*****************************************************************************/
/**
* @brief  void ModINT_Init
*         �ж�ģ���ʼ��
* @param  void
* @return void
*/
/*****************************************************************************/
void ModINT_Init(void)
{
	memset(gast_INTVect, 0,sizeof(gast_INTVect));
	
	IER = 0x00;
	IFR = 0x00;
	
	{asm(" EALLOW");}
	REG_GPIO_GPEMUX |= 0x0001;
	
	
	ENPIES = 1;         /*  */
	PIEACKX |= PIEACK_GROUP1;
	XINT1CR = 0x05;            
	
	*VECT_XINT1 = (PINT)ModINT_Handle;
	
	PIEIER1S |= 0x0008; 
	{asm(" EDIS");}
	IER |= 0x01;
}

/*****************************************************************************/
/**
* @brief  void ModINT_Rigster
*         �ж�ģ���жϴ������ע��
* @param  IN uint8_t uc_Port:�жϺ�
* @param  IN uint8_t uc_Param���жϴ���������
* @param  IN FucVect pFn_Vect���жϴ������
* @return void
*/
/*****************************************************************************/
void ModINT_Rigster(IN uint8_t uc_Port,IN uint8_t uc_Param,IN FucVect pFn_Vect) 
{ 		
 	SOS_Assert( uc_Port >= INTERRUPT_SRC_NUM );
 	
 	SOS_Assert( pFn_Vect == NULLS );
 	
	DINT;
	
	gast_INTVect[uc_Port].pFn_Vector = pFn_Vect;
	gast_INTVect[uc_Port].uc_Param = uc_Param;
	gast_INTVect[uc_Port].uc_Init = SYS_YES;
	
	EINT;	
}


/*****************************************************************************/
/**
* @brief  void ModINT_PollHandle
*         �ж�ģ���жϴ���������
* @param  IN uint8_t uc_Port:�жϺ�
* @param  IN uint8_t uc_Param���жϴ���������
* @param  IN FucVect pFn_Vect���жϴ������
* @return void
*/
/*****************************************************************************/
void ModINT_PollHandle(void)
{ 

	volatile uint8_t uc_IMS   =0;
	uint8_t uc_Cnt = 0;
	uint8_t uc_Mask = 0x01;
	
	OS_ENTER_CRITICAL();              /*�ر��ж�  20170808*/
	uc_IMS = IMS_INT_SOURCE&0x03F;    /*�����жϣ� 20170808*/
	while (uc_IMS > 0)
	{	
		uc_Mask = 0x01;
		for (uc_Cnt = 0; uc_Cnt < INTERRUPT_SRC_NUM; uc_Cnt++)
		{
			if ((uc_IMS&uc_Mask)&&(gast_INTVect[uc_Cnt].uc_Init == SYS_YES))
			{
				if (gast_INTVect[uc_Cnt].pFn_Vector != NULLS)
				{
					gast_INTVect[uc_Cnt].pFn_Vector(gast_INTVect[uc_Cnt].uc_Param);
				}
			}
			else
			{
				
			}
			uc_Mask = uc_Mask<<1;   
		}
		uc_IMS = IMS_INT_SOURCE&0x3F;   
	}
	OS_EXIT_CRITICAL();   /*�ָ��ж� 20170808*/
}

/*****************************************************************************/
/**
* @brief  interrupt void ModINT_Handle
*         �ж��ܴ������
* @param  void
* @return void
*/
/*****************************************************************************/
static interrupt void ModINT_Handle(void)
{ 

    volatile uint8_t uc_IMS   =0;
	uint8_t uc_Cnt = 0;
	uint8_t uc_Mask = 0x01;

	uc_IMS = IMS_INT_SOURCE&0x3F;

	while(uc_IMS > 0)
	{
		uc_Mask = 0x01;
		for (uc_Cnt = 0; uc_Cnt < INTERRUPT_SRC_NUM; uc_Cnt++)
		{
			if ((uc_IMS&uc_Mask)&&(gast_INTVect[uc_Cnt].uc_Init == SYS_YES))
			{
				if (gast_INTVect[uc_Cnt].pFn_Vector != NULLS)
				{
					gast_INTVect[uc_Cnt].pFn_Vector(gast_INTVect[uc_Cnt].uc_Param);
				}
			}
			else
			{
				
			}
			uc_Mask = uc_Mask<<1;
		}
		uc_IMS = IMS_INT_SOURCE&0x3F;
	}
	
	PIEACKX |= PIEACK_GROUP1;
}
