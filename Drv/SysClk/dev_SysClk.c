/******************************************************************************/
/**
*@file    Dev_SysClk.c       
*@brief   系统时钟初始化模块                  
*@author  yangbm
*@version V1.0.0
*@date    2010.03.11
*@details v1.0.0  2010.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h"
#include "dev_SysClk.h"
    
/*****************************************************************************/
/**
* @brief  void initSysCtrl
*         系统时钟初始化
* @param  void
* @return void
*/
/*****************************************************************************/
void initSysCtrl(void)
{
    volatile uint16_t i = 0U;										

	{ asm(" EALLOW");}					
			
	REG_SYSCLK_WDCR = 0x00E8U;
	REG_SYSCLK_SCSR = 0x0000U;
	/*bit 15-4             0's:    reserved
	bit 3-0(DIV)        1010:    CLKIN = (OSCCLK * 10.0)/2*/
	REG_SYSCLK_PLLCR = 0x000AU;		

	for(i=0; i<510; i++)
	{
		{asm(" RPT #255 || NOP");}			
		REG_SYSCLK_WDKEY = 0x0055U;		
		REG_SYSCLK_WDKEY = 0x00AAU;			
	}
	/*bit 15-3              0's:    reserved
	bit 2-0(HSPCLK)       010:    High speed clock = SYSCLKOUT/4*/
	REG_SYSCLK_HISPCP = 0x0002U;		 
	/*bit 15-3              0's:    reserved
	bit 2-0(LSPCLK)       010:    Low speed clock= SYSCLKOUT/4	*/ 
	REG_SYSCLK_LOSPCP = 0x0002U;	
	REG_SYSCLK_PCLKCR = 0x5D08U;
	/*bit 15-8          00000000:    Reserved
	bit 7-2(QUALSTDBY)  111111:    number of OSCCLK clock cycles to qualify the selected inputs when waking the device from STANDBY mode is 65 OSCCLKs
	bit 1-0(LPM)            00:    Set the low power mode to IDLE	*/
	REG_SYSCLK_LPMCR0 = 0x00FCU;		
	REG_SYSCLK_LPMCR1 = 0x0000U;	
		
	{asm(" EDIS");}				
} 


