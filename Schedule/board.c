/******************************************************************************/
/**
*@file    Board.c        
*@brief   �忨״̬Դ�ļ�                    
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
/**@brief   ���е�ʱ�����                                                    	     */
/*****************************************************************************/
static uint32_t  gui_OpenRunLedTimer = 0;

/*****************************************************************************/
/**@brief   QText����δ洢�����е�ַ�ɱ���������                      			    */
/*****************************************************************************/
extern uint16_t QText_LoadStart;
extern uint16_t QText_LoadEnd;
extern uint16_t QText_RunStart;

/*****************************************************************************/
/**@brief   �ֲ���������                                            			    */
/*****************************************************************************/
static void InitRunLed(void);

static  void ResetTL16C754(void);

static void dev_CpyQText(void);

/*****************************************************************************/
/**
* @brief  void initBoard
*         ��ʼ���忨
* @param  void
* @return void
*/
/*****************************************************************************/
void initBoard(void)
{
	initSysCtrl();                             	/*��ʼ��ϵͳʱ�� 20170808*/

	initXintfRegs();                          	/*�ⲿ���߳�ʼ�� 20170808*/

	cpyFlashInitSect();

	initFlash();                      
	
	dev_CpyQText();                           	/*Flash�����Ƶ�Ram�� 20170808*/
	
	InitRunLed();                               /*��ʼ�����е� 20170808*/
	
	ResetTL16C754();                           /*��λTL16C754 20170808*/
}

/*****************************************************************************/
/**
* @brief  void FlashRunLed
*         ��˸���е�
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
*         �������
* @param  char_t  * const File_Name���ļ�����
* @param  const uint32_t  ui_Line��������������
* @param  uint32_t ui_Arg�����ϲ���
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
*         Qtext�ε�FLASH�������Ƶ�Ram��
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
*         ��ʼ�����е�
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
*         ��λTL16C754
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
