/******************************************************************************/
/**
*@file    Board.c        
*@brief   板卡状态源文件                    
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief   include                                                    	     */
/*****************************************************************************/
#include  <string.h>
#include  "board.h"
#include  "dev_Gpio.h"
#include  "dev_SysClk.h"
#include  "dev_Xintf.h"
#include  "dev_Flash.h"
#include  "dev_Timer.h"
#include  "TL16C754.h"
#include  "W5300.h"
#include  "Driver.h"
#include  "Config.h"

/*****************************************************************************/
/**@brief   运行灯时间计数                                                    	     */
/*****************************************************************************/
static uint32_t  gui_OpenRunLedTimer = 0;

/*****************************************************************************/
/**@brief   QText代码段存储和运行地址由编译器生成                      			    */
/*****************************************************************************/
extern uint16_t QText_LoadStart;
extern uint16_t QText_LoadEnd;
extern uint16_t QText_RunStart;

/*****************************************************************************/
/**@brief   局部变量声明                                            			    */
/*****************************************************************************/
static void InitRunLed(void);

static  void ResetTL16C754(void);

static void dev_CpyQText(void);

/*****************************************************************************/
/**
* @brief  void initBoard
*         初始化板卡
* @param  void
* @return void
*/
/*****************************************************************************/
void initBoard(void)
{
	initSysCtrl();                             	/*初始化系统时钟 20170808*/

	initXintfRegs();                          	/*外部总线初始化 20170808*/

	cpyFlashInitSect();

	initFlash();                      
	
	dev_CpyQText();                           	/*Flash程序复制到Ram区 20170808*/
	
	InitRunLed();                               /*初始化运行灯 20170808*/
	
	ResetTL16C754();                           /*复位TL16C754 20170808*/
}

/*****************************************************************************/
/**
* @brief  void FlashRunLed
*         闪烁运行灯
* @param  void
* @return void
*/
/*****************************************************************************/
void FlashRunLed(void)
{	
	uint16_t  us_Data = 0;
	uint16_t  us_Tmp = 0;
	if (SYS_OK == IsTimeOut(200, gui_OpenRunLedTimer))
	{
		us_Data = REG_GPIO_GPFDAT&0x4000;
		us_Data = ~us_Data;
		us_Tmp = REG_GPIO_GPFDAT&0xBFFF;
		us_Tmp |= us_Data;
		REG_GPIO_GPFDAT = us_Tmp;
		gui_OpenRunLedTimer = getTick();
	}
	else
	{
		
	}
}

void  FlashRunLenTest(void)
{
	
}
/*****************************************************************************/
/**
* @brief  void SOS_Assertion_Failed
*         错误断言
* @param  char_t  * const File_Name：文件名；
* @param  const uint32_t  ui_Line：错误发生的行数
* @param  uint32_t ui_Arg：故障参数
* @return void
*/
/*****************************************************************************/
void SOS_Assertion_Failed (char_t  * const File_Name, const uint32_t  ui_Line, uint32_t ui_Arg)
{
	while(1)
	{
		
	}
}

/*****************************************************************************/
/**
* @brief  void dev_CpyQText
*         Qtext段的FLASH区程序复制到Ram区
* @param  void
* @return void
*/
/*****************************************************************************/
static void dev_CpyQText(void)
{
	memcpy( &QText_RunStart, &QText_LoadStart, &QText_LoadEnd - &QText_LoadStart );	
}

/*****************************************************************************/
/**
* @brief  void InitRunLed
*         初始化运行灯
* @param  void
* @return void
*/
/*****************************************************************************/
static void InitRunLed(void)
{
	asm(" EALLOW");
	REG_GPIO_GPFMUX &= 0xBFFFU; 
	REG_GPIO_GPFDIR |= 0x4000U;

	{asm(" EDIS");}
	REG_GPIO_GPFDAT &= 0xBFFFU; 
	gui_OpenRunLedTimer = 0;
}


/*****************************************************************************/
/**
* @brief  void ResetTL16C754
*         复位TL16C754
* @param  void
* @return void
*/
/*****************************************************************************/
static  void ResetTL16C754(void)
{
	volatile   uint32_t  ui_Cnt = 0;
	
	{asm("  EALLOW");}
	#ifdef __NET_CARD__
	REG_GPIO_GPBMUX &= 0xFFFEU; 
	REG_GPIO_GPBDIR |= 0x0001U;
	#else
	REG_GPIO_GPBMUX &= 0xFFF9U; 
	REG_GPIO_GPBDIR |= 0x0006U;
	#endif
	{asm(" EDIS");}
	#ifdef __NET_CARD__	
	REG_GPIO_GPBDAT |= 0x0001U;
	#else
	REG_GPIO_GPBDAT |= 0x0006U;
	#endif
	
	for ( ui_Cnt= 0; ui_Cnt < 15000; ui_Cnt++)
	{
		
	}
	#ifdef __NET_CARD__	
	REG_GPIO_GPBDAT &= 0xFFFEU;
	#else
	REG_GPIO_GPBDAT &= 0xFFF9U;
	#endif		
}
