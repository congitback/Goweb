/******************************************************************************/
/**
*@file    dev_Flash.h       
*@brief   �豸FLASH��ʼ������                   
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                    	     */
/*****************************************************************************/
#include  "Typedef.h"
#include  "dev_Flash.h"

/*****************************************************************************/
/**@brief  �ɱ�������ֵ������FALSH����εļ��ص�ַ���е�ַ                       	     */
/*****************************************************************************/
extern  uint16_t  Flash_Funcs_LoadStart;

extern  uint16_t  Flash_Funcs_LoadEnd;

extern  uint16_t  Flash_Funcs_RunStart; 

/*****************************************************************************/
/**
* @brief  void cpyFlashInitSect
*         FLASH�����ݸ��Ƶ�Ram��
* @param  void
* @return void
*/
/*****************************************************************************/
void cpyFlashInitSect(void)
{
    memcpy( &Flash_Funcs_RunStart, &Flash_Funcs_LoadStart,
	        &Flash_Funcs_LoadEnd - &Flash_Funcs_LoadStart );
}

/*****************************************************************************/
/**
* @brief  void initFlash
*         FLASH�洢����ʼ��
* @param  void
* @return void
*/
/*****************************************************************************/
#pragma CODE_SECTION(initFlash, "secureRamFuncs")

void initFlash(void)
{
	asm(" EALLOW");
								
	REG_FLASH_FPWR = 3;
	REG_FLASH_FSTATUS |= 0x100U;											
	REG_FLASH_FSTDBYWAIT = 0xfF;
	REG_FLASH_FACTIVEWAIT = 0xfF;
	REG_FLASH_FBANKWAIT = 0x0303;
	REG_FLASH_FOTPWAIT = 6;
	REG_FLASH_FOPT = 1;		
	asm(" EDIS");								

    asm(" RPT #6 || NOP");
} 
