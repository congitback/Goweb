#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "Typedef.h"

#define  MAX_DRIVER_TYPE			6
#define  DEV_OPERATE_CLEAR          0x24
#define  DEV_OPERATE_COMSTATE       0x25
#define  DEV_OPERATE_DATASTATE      0x26    
 
#define  DATA_VALID      			0x03
#define  COM_STATE       			0x06
#define  ERR_CODE       			0x30

typedef  struct  tagDriverMod
{
 	uint8_t  uc_Init;
 	uint8_t  (*Open)(IN  uint8_t  uc_Minor,  IN int8_t sc_OpFlag);
 	uint8_t  (*Write)(IN uint8_t uc_Minor, IN char_t *pc_Buf, IN int32_t i_Len);		
 	int32_t  (*Read)(IN  uint8_t  uc_Minor, OUT char_t *pc_Buf, IN int32_t i_Len);
 	uint8_t  (*Ioctl)(IN uint8_t uc_Minor, IN uint32_t i_Arg, uint16_t *pus_Data );
 	uint32_t (*Check)(void);
 	uint16_t  us_Crc;
}SDriverMod;

extern uint8_t dev_Open(IN uint8_t uc_Major, IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag, OUT uint16_t *pus_Err);

extern uint8_t dev_Write(IN uint8_t uc_Major, IN uint8_t uc_Minor, IN char_t *pc_Buf, IN int32_t ul_Len, 
 					     OUT  uint16_t  *pus_Err);

extern int32_t dev_Read(IN uint8_t uc_Major, IN uint8_t uc_Minor, OUT char_t *pc_Buf,  
				        IN int32_t i_Len, OUT uint16_t  *pus_Err);

extern uint8_t dev_Ioctl(IN uint8_t uc_Major , IN uint8_t uc_Minor, IN uint32_t ul_Arg, INOUT uint16_t  *pus_Data
						 ,OUT uint16_t  *pus_Err);
				         
extern  void  dev_DrvSndSchedule(void);

extern  uint8_t  InitDriverMod(void);

extern  uint32_t  dev_check(IN uint8_t uc_Major);

extern  void  dev_DrvRevSchedule(void);

#endif /*__DRIVER_H__*/
