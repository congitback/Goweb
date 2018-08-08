/******************************************************************************/
/**
*@file    dev_Timer.c       
*@brief   时间模块源文件，包括定时器设置、中断处理、定时调度管理、系统时间管理、延时处理、超时判断                  
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
/**@brief  系统时间毫秒计数,基础单位为1ms                                         */
/*****************************************************************************/
volatile  uint32_t  gui_Tick = 0;

//*****************************************************************************/
/**@brief  系统时间微秒计数,基础单位为10us                                         */
/******************************************************************************/
volatile  uint32_t  gui_Microsecond = 0;

volatile  uint32_t  ui_MsCount = 0;
/* 定时器 中断处理函数 */

static  uint8_t startCpuTimer(IN uint16_t us_TimerCode)  ;

static uint8_t enableTimerInt(IN uint16_t us_TimerCode, IN PINT pVect);

static uint8_t setTimerRate(IN uint16_t us_TimerCode, IN uint32_t ui_Rate);

static interrupt void timer0Exption(void); 


/*****************************************************************************/
/**
* @brief  void initTimer0
*         调度对象设置接口，用于向时间调度模块注册调度对象
* @param  void
* @return void：正常--SYS_YES，否则宕机
*/
/*****************************************************************************/
void initTimer0(void)					
{	
	gui_Tick = 0;
	gui_Microsecond = 0;
	/*设置定时器周期数值*/
	(void)setTimerRate(0, 0x0064);
	/*启动定时器*/
	(void)startCpuTimer(0);
	/*使能定时器中断 */
	(void)enableTimerInt(0, timer0Exption);
	return;
}
/*****************************************************************************/
/**
* @brief  uint32_t  getTick
*         获取系统毫秒时间
* @param  void
* @return uint32_t：毫秒值，基础单位1ms
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
*         获取微秒值
* @param  void
* @return uint32_t：微秒值，基础单位10us
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
*         毫秒级超时判断,基础单位1ms
* @param  IN uint32_t ui_TimeLength：超时门限
* @param  IN uint32_t ui_RefTimer：基准时间 
* @return 超时---SYS_YES,未超时---SYS_NO
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
*         微秒级超时判断,基础单位10us
* @param  IN uint32_t ui_TimeLength：超时门限
* @param  IN uint32_t ui_RefTimer：基准时间 
* @return 超时---SYS_YES,未超时---SYS_NO
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
*         10微秒级延时
* @param  IN uint32_t  ui_DelayTimer：延时时间，单位10us
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
*         启动定时器
* @param  IN uint16_t us_TimerCode	：定时器编号,0-2;
* @return uint8_t：正常--SYS_YES，错误---SYS_NO
*/
/*****************************************************************************/
static  uint8_t startCpuTimer(IN uint16_t us_TimerCode)  
{
	uint32_t ui_AddrBase = 0;
    /*定时器编号应为0、1、2 */  
	if(us_TimerCode > 2) 
	{
		return SYS_ERR;
	}
	 /* 计算相应编号寄存器基址 */
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode; 
    
	/**TCR_TIMER(ui_AddrBase) &=0xffef; */ 
	/* 设置TCR寄存器 */
	*((volatile uint16_t*)(ui_AddrBase + 4)) &= 0xffef; 
     
	return SYS_OK;      
}


/*****************************************************************************/
/**
* @brief  uint8_t enableTimerInt
*         使能定时器中断
* @param  IN uint16_t us_TimerCode	：定时器编号,0-2;
* @param  IN PINT pVect：定时器中断向量
* @return uint8_t：正常--SYS_YES，错误---SYS_NO
*/
/*****************************************************************************/
static uint8_t enableTimerInt(IN uint16_t us_TimerCode, IN PINT pVect)
{
	uint32_t ui_AddrBase = 0;
    /*定时器编号应为0、1、2 */ 
	if((us_TimerCode > 2) || (pVect == NULLS)) 
	{
		return SYS_ERR;
	}
	/* 计算相应编号寄存器基址 */     
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode;  

        
	asm(" SETC INTM, DBGM");

	switch(us_TimerCode)
	{   
		case 0: /*定时器0中断配置*/
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
		case 1:	/*定时器1中断配置*/
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
        case 2:	/*定时器2中断配置*/
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
*         设置定时器频率
* @param  IN uint16_t us_TimerCode	：定时器编号,0-2;
* @param  IN uint32_t ui_Rate：频率值，基础单位为100ns
* @return uint8_t：正常--SYS_YES，错误---SYS_NO
*/
/*****************************************************************************/
static uint8_t setTimerRate(IN uint16_t us_TimerCode, IN uint32_t ui_Rate)
{   
	uint32_t ui_AddrBase = 0;

	if(us_TimerCode > 2)
	{
		return SYS_ERR;
	}
	/* 计算相应编号寄存器基址 */	
	ui_AddrBase = BASEADDR_TIMER0 + 8*us_TimerCode;  
	*((volatile uint16_t*)((ui_AddrBase) + 6)) = 0x0E0E;

	*((volatile uint16_t*)((ui_AddrBase) + 0)) = ui_Rate; 
	
	*((volatile uint16_t*)((ui_AddrBase) + 2)) = ui_Rate;
	
	return SYS_OK;
} 


/*****************************************************************************/
/**
* @brief  void timer0Exption
*         定时器中断处理函数
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
