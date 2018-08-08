/******************************************************************************/
/**
*@file    dev_Timer.c       
*@brief   ʱ��ģ��Դ�ļ���������ʱ�����á��жϴ�����ʱ���ȹ���ϵͳʱ�������ʱ������ʱ�ж�                  
*@author  yangbm
*@version V1.0.0
*@date    2010.03.11
*@details v1.0.0  2010.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h"
#include  "dev_Timer.h"
#include  "dev_Interrupt.h"

/*****************************************************************************/
/**@brief  ϵͳʱ��������,������λΪ1ms                                         */
/*****************************************************************************/
volatile  uint32_t  gui_Tick = 0;

//*****************************************************************************/
/**@brief  ϵͳʱ��΢�����,������λΪ10us                                         */
/******************************************************************************/
volatile  uint32_t  gui_Microsecond = 0;

volatile  uint32_t  ui_MsCount = 0;
/* ��ʱ�� �жϴ����� */

static  uint8_t startCpuTimer(IN uint16_t us_TimerCode)  ;

static uint8_t enableTimerInt(IN uint16_t us_TimerCode, IN PINT pVect);

static uint8_t setTimerRate(IN uint16_t us_TimerCode, IN uint32_t ui_Rate);

static interrupt void timer0Exption(void); 


/*****************************************************************************/
/**
* @brief  void initTimer0
*         ���ȶ������ýӿڣ�������ʱ�����ģ��ע����ȶ���
* @param  void
* @return void������--SYS_YES������崻�
*/
/*****************************************************************************/
void initTimer0(void)					
{	
	gui_Tick = 0;
	gui_Microsecond = 0;
	/*���ö�ʱ��������ֵ*/
	(void)setTimerRate(0, 0x0064);
	/*������ʱ��*/
	(void)startCpuTimer(0);
	/*ʹ�ܶ�ʱ���ж� */
	(void)enableTimerInt(0, timer0Exption);
	return;
}
/*****************************************************************************/
/**
* @brief  uint32_t  getTick
*         ��ȡϵͳ����ʱ��
* @param  void
* @return uint32_t������ֵ��������λ1ms
*/
/*****************************************************************************/
uint32_t  getTick(void)
{
	uint32_t ui_Tick = 0;

	ui_Tick = gui_Tick;

	return ui_Tick;
}

/*****************************************************************************/
/**
* @brief  uint32_t  getMicrosecond
*         ��ȡ΢��ֵ
* @param  void
* @return uint32_t��΢��ֵ��������λ10us
*/
/*****************************************************************************/
uint32_t  getMicrosecond(void)
{
	uint32_t  ui_Microsecond = 0;
	
	ui_Microsecond = gui_Microsecond;

	return 	ui_Microsecond;
}

/*****************************************************************************/
/**
* @brief  uint8_t IsTimeOut
*         ���뼶��ʱ�ж�,������λ1ms
* @param  IN uint32_t ui_TimeLength����ʱ����
* @param  IN uint32_t ui_RefTimer����׼ʱ�� 
* @return ��ʱ---SYS_YES,δ��ʱ---SYS_NO
*/
/*****************************************************************************/
uint8_t IsTimeOut(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer)
{
	uint32_t ui_Timer = 0;
	uint32_t ui_Count = 0;
	uint32_t ui_MaxTimer = 0xFFFFFFFF;
	uint8_t  uc_Ret = SYS_ERR;
	ui_Timer =  getTick();
    		
    if (ui_Timer < ui_RefTimer )
    {
    	ui_Count = ui_MaxTimer - ui_RefTimer + ui_Timer;
    }
  	else
    {
    	ui_Count = ui_Timer - ui_RefTimer ;
    }
    if (ui_Count >=  ui_TimeLength)
    {
    	uc_Ret = SYS_OK;	
    }
    else
    {
    	uc_Ret = SYS_ERR;	
    }
    return uc_Ret;
}


/*****************************************************************************/
/**
* @brief  uint8_t IsTimeOutUs
*         ΢�뼶��ʱ�ж�,������λ10us
* @param  IN uint32_t ui_TimeLength����ʱ����
* @param  IN uint32_t ui_RefTimer����׼ʱ�� 
* @return ��ʱ---SYS_YES,δ��ʱ---SYS_NO
*/
/*****************************************************************************/
uint8_t IsTimeOutUs(IN uint32_t ui_TimeLength, IN uint32_t ui_RefTimer)
{
	uint32_t ui_Timer = 0;
	uint32_t ui_Count = 0;
	uint32_t ui_MaxTimer = 0xFFFFFFFF;
	uint8_t  uc_Ret = SYS_ERR;
	ui_Timer =  gui_Microsecond;
    		
    if (ui_Timer < ui_RefTimer )
    {
    	ui_Count = ui_MaxTimer - ui_RefTimer + ui_Timer;
    }
  	else
    {
    	ui_Count = ui_Timer - ui_RefTimer ;
    }
    if (ui_Count >=  ui_TimeLength)
    {
    	uc_Ret = SYS_OK;	
    }
    else
    {
    	uc_Ret = SYS_ERR;	
    }
    return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  void delay10Us
*         10΢�뼶��ʱ
* @param  IN uint32_t  ui_DelayTimer����ʱʱ�䣬��λ10us
* @return void
*/
/*****************************************************************************/
void delay10Us(IN uint32_t  ui_DelayTimer)
{
	uint32_t  ui_StartTimer = 0;
	uint32_t  ui_EndTimer = 0;
	uint32_t  ui_Count = 0;
	uint32_t  ui_MaxTimer = 0xFFFFFFFF;
	
	ui_StartTimer =  getMicrosecond();
	
	while(ui_Count < ui_DelayTimer)
	{
		ui_EndTimer = getMicrosecond();
		
		if (ui_EndTimer < ui_StartTimer)	
		{
			ui_Count = ui_MaxTimer - ui_StartTimer + ui_EndTimer;	
		}
		else
		{
			ui_Count = ui_EndTimer - ui_StartTimer;	
		}	
	}	
}

/*****************************************************************************/
/**
* @brief  uint8_t startCpuTimer
*         ������ʱ��
* @param  IN uint16_t us_TimerCode	����ʱ�����,0-2;
* @return uint8_t������--SYS_YES������---SYS_NO
*/
/*****************************************************************************/
static  uint8_t startCpuTimer(IN uint16_t us_TimerCode)  
{
	uint32_t ui_AddrBase = 0;
    /*��ʱ�����ӦΪ0��1��2 */  
	if(us_TimerCode > 2) 
	{
		return SYS_ERR;
	}
	 /* ������Ӧ��żĴ�����ַ */
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode; 
    
	/**TCR_TIMER(ui_AddrBase) &=0xffef; */ 
	/* ����TCR�Ĵ��� */
	*((volatile uint16_t*)(ui_AddrBase + 4)) &= 0xffef; 
     
	return SYS_OK;      
}


/*****************************************************************************/
/**
* @brief  uint8_t enableTimerInt
*         ʹ�ܶ�ʱ���ж�
* @param  IN uint16_t us_TimerCode	����ʱ�����,0-2;
* @param  IN PINT pVect����ʱ���ж�����
* @return uint8_t������--SYS_YES������---SYS_NO
*/
/*****************************************************************************/
static uint8_t enableTimerInt(IN uint16_t us_TimerCode, IN PINT pVect)
{
	uint32_t ui_AddrBase = 0;
    /*��ʱ�����ӦΪ0��1��2 */ 
	if((us_TimerCode > 2) || (pVect == NULLS)) 
	{
		return SYS_ERR;
	}
	/* ������Ӧ��żĴ�����ַ */     
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode;  

        
	asm(" SETC INTM, DBGM");

	switch(us_TimerCode)
	{   
		case 0: /*��ʱ��0�ж�����*/
		{
			ENPIES = 0;
			PIEACKX |= 0x01;
			ENPIES = 1;

			asm(" EALLOW");
			*VECT_TIMER0 = (PINT)pVect; 
			asm(" EDIS");
		    
			PIEIER1S |= 0x40;
			IER |= 0x01;
			break;
		}
		case 1:	/*��ʱ��1�ж�����*/
		{   
			IFR &=~M_INT13;
			*XMNICR_CONFIG = 0;
            asm(" EALLOW");
			*VECT_TIMER1 = pVect;
			asm(" EDIS");
            /*bit 12(INT13)  1:     Enable CPU INT13(TIMER1)*/
			IER |= M_INT13;
			break;
		}
        case 2:	/*��ʱ��2�ж�����*/
		{    
			IFR &=~M_INT14;
			asm(" EALLOW");
			*VECT_TIMER2 = pVect;
			asm(" EDIS");
            /*bit 13(INT14) 1:     Enable CPU INT14(TIMER2)*/
			IER |= M_INT14;
			break;
		}
		default:
		{
            break;
		}
	}
	*((volatile uint16_t*)((ui_AddrBase) + 4)) |=0x6000; 
	asm(" clrc INTM");
	asm(" clrc DBGM"); 
	return SYS_OK; 
}

/*****************************************************************************/
/**
* @brief  uint8_t setTimerRate
*         ���ö�ʱ��Ƶ��
* @param  IN uint16_t us_TimerCode	����ʱ�����,0-2;
* @param  IN uint32_t ui_Rate��Ƶ��ֵ��������λΪ100ns
* @return uint8_t������--SYS_YES������---SYS_NO
*/
/*****************************************************************************/
static uint8_t setTimerRate(IN uint16_t us_TimerCode, IN uint32_t ui_Rate)
{   
	uint32_t ui_AddrBase = 0;

	if(us_TimerCode > 2)
	{
		return SYS_ERR;
	}
	/* ������Ӧ��żĴ�����ַ */	
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode;  
	*((volatile uint16_t*)((ui_AddrBase) + 6)) = 0x0E0E;

	*((volatile uint16_t*)((ui_AddrBase) + 0)) = ui_Rate; 
	
	*((volatile uint16_t*)((ui_AddrBase) + 2)) = ui_Rate;
	
	return SYS_OK;
} 


/*****************************************************************************/
/**
* @brief  void timer0Exption
*         ��ʱ���жϴ�����
* @param  void
* @return void
*/
/*****************************************************************************/
static  interrupt void timer0Exption(void)			
{   
	static uint8_t uc_MsCount = 0;	
    gui_Microsecond++;
    
	if ( uc_MsCount > 98)
	{
		gui_Tick++;		
		uc_MsCount = 0;
	}
	else
	{
		uc_MsCount++;
	}
	
	PIEACKX |= PIEACK_GROUP1;
	return; 					
} 
