#ifndef RAMBUF_H_
#define RAMBUF_H_
#include  "Typedef.h"


typedef  struct tagRamObj
{
	uint8_t   uc_Inited;
	uint16_t  us_Id;
	uint32_t  ui_Addr;
	uint32_t  ui_Len;
}SRamObj;

typedef  struct tagRamBuf
{
	
	uint32_t ui_RamSpace;
	uint32_t ui_RamRemain;
	uint32_t ui_UsableAddr;
	uint32_t us_ObjSum;
	uint16_t us_Init;
	uint16_t us_Resed;
	SRamObj  ast_Obj[1]; 
}SRamBuf;

extern uint8_t  InitBuf(uint8_t auc_Ram[], uint32_t ui_Len, uint16_t us_ObjNum );

extern int32_t  CreatObj(uint8_t auc_Ram[], uint32_t  ui_Len);

extern uint8_t   WriteBuf(uint8_t auc_Ram[], uint16_t us_Id, uint8_t auc_Buf[], uint32_t ui_Len);

extern int32_t   ReadBuf(uint8_t auc_Ram[], uint16_t us_Id, uint8_t auc_Buf[], uint32_t ui_Len);

extern uint8_t* GetObjAddr(uint8_t auc_Ram[], uint16_t us_Id);
#endif /*M_BUF_H_*/
