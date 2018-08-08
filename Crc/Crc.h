/******************************************************************************/
/**
*@file    Crc.h     
*@brief   crcУ��                      
*@author  yangbm
*@version V1.0.0
*@date    2012.03.14
*@details v1.0.0 2012.03.14 yangbm create
*/
/******************************************************************************/
#ifndef __CRC_H__
#define __CRC_H__

/*****************************************************************************/
/**@brief ģ��ӿ�����  */
/*****************************************************************************/
extern uint16_t getCrc16(IN uint16_t  aus_Data[], IN int16_t s_Len);
/*��ȡ8���ݲ��������CrcУ��ֵ*/
extern uint16_t getCrc(IN uint8_t auc_Data[], IN int16_t sh_Len);
/*DWORD->4BYTE*/
extern void  DWORDToBYTE(OUT uint8_t auc_Buf[],IN uint32_t ui_Data);
/*4BYTE->DWORD*/
extern uint32_t  BYTEToDWORD(IN uint8_t auc_Buf[]);

#endif /*CRC_H_*/
