/******************************************************************************/
/**
*@file    M_Buf.c      
*@brief   外部接口模块                  
*@author  yangbm
*@version V1.0.0
*@date    2016.10.21
*@details v1.0.0  2016.10.21 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "RamBuf.h"
#include  <string.h>



/*****************************************************************************/
/**
* @brief  void MBuf_Init(void)
*         MBuf池初始化
* @param  void
* @return void OK
*/
/*****************************************************************************/
uint8_t  InitBuf(uint8_t auc_Ram[], uint32_t ui_Len, uint16_t us_ObjNum )
{
	SRamBuf *pst_RamBuf = NULLS;
	uint32_t  ui_UsedSize = 0;
	
	SOS_Assert(NULLS == auc_Ram); /*边界检查*/
	
	SOS_Assert(us_ObjNum == 0);
	
	ui_UsedSize = sizeof(SRamBuf) + (us_ObjNum - 1)*sizeof(SRamObj) + 4; /*确定描述区占用空间，SRamBuf 中有一个SRamObj结构数据*/
	
	SOS_Assert(ui_Len <= ui_UsedSize);	/*判断空间是否足够*/
	
	pst_RamBuf = (SRamBuf *)auc_Ram;
	
	pst_RamBuf->ui_RamSpace = ui_Len;
	pst_RamBuf->us_ObjSum = us_ObjNum;
	pst_RamBuf->us_Resed = 0;
	
	memset((uint8_t*)pst_RamBuf->ast_Obj, 0 , us_ObjNum*sizeof(SRamObj));
	
	pst_RamBuf->ui_RamRemain = ui_Len - ui_UsedSize;
	
	pst_RamBuf->ui_UsableAddr = (uint32_t)&auc_Ram[ui_UsedSize];
	
	if(pst_RamBuf->ui_UsableAddr%2)/*对齐矫正*/
	{
		pst_RamBuf->ui_UsableAddr = pst_RamBuf->ui_UsableAddr +1;
		pst_RamBuf->ui_RamRemain = pst_RamBuf->ui_RamRemain -1;
	}
	else
	{
		
	}
	pst_RamBuf->us_Init = SYS_YES;
	
	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  MBuf_New
*         MBuf池初始化
* @param  uint16_t us_Len：申请空间长度；
* @param  uint16_t us_Arg：块信息
* @return uint16_t --返回申请块头 OK
*/
/*****************************************************************************/
int32_t  CreatObj(uint8_t auc_Ram[], uint32_t  ui_Len)
{
	int32_t i_Ret = -1;
	
	uint16_t us_Cnt = 0;
	
	SRamBuf *pst_RamBuf = NULLS;
	
	SOS_Assert(NULLS == auc_Ram);
	
	pst_RamBuf = (SRamBuf *)auc_Ram;
	
	SOS_Assert(SYS_YES != pst_RamBuf->us_Init);
	
	SOS_Assert(ui_Len > pst_RamBuf->ui_RamRemain);
	
	for (us_Cnt = 1; us_Cnt < pst_RamBuf->us_ObjSum; us_Cnt++)
	{
		if (pst_RamBuf->ast_Obj[us_Cnt].uc_Inited != SYS_YES)
		{
			pst_RamBuf->ast_Obj[us_Cnt].ui_Addr = pst_RamBuf->ui_UsableAddr;
			
			pst_RamBuf->ast_Obj[us_Cnt].us_Id =  us_Cnt;
			pst_RamBuf->ast_Obj[us_Cnt].ui_Len = ui_Len;
			memset((uint8_t*)pst_RamBuf->ui_UsableAddr,0, ui_Len);
			pst_RamBuf->ui_RamRemain = pst_RamBuf->ui_RamRemain - ui_Len;
			pst_RamBuf->ui_UsableAddr = pst_RamBuf->ui_UsableAddr + ui_Len;
			
			if (pst_RamBuf->ui_UsableAddr%2) /*对齐校正*/
			{
				if (pst_RamBuf->ui_RamRemain  > 0)
				{
					pst_RamBuf->ui_UsableAddr = pst_RamBuf->ui_UsableAddr + 1;
					pst_RamBuf->ui_RamRemain = pst_RamBuf->ui_RamRemain -1;
				}
			}
			else
			{
				
			}
			pst_RamBuf->ast_Obj[us_Cnt].uc_Inited = SYS_YES;
			i_Ret = us_Cnt;
			break;
		}
	}
	
	return i_Ret;
}

/*****************************************************************************/
/**
* @brief  MBuf_Free
*         MBuf池初始化
* @param  uint16_t us_BlockNum：释放块头编号；
* @return void ok
*/
/*****************************************************************************/
uint8_t   WriteBuf(uint8_t auc_Ram[], uint16_t us_Id, uint8_t auc_Buf[], uint32_t ui_Len)
{
	SRamBuf *pst_RamBuf = NULLS;
	
	SOS_Assert(NULLS == auc_Ram);
	
	pst_RamBuf = (SRamBuf *)auc_Ram;
	
	SOS_Assert(auc_Buf == NULLS);
	
	SOS_Assert(SYS_YES != pst_RamBuf->us_Init);
	
	SOS_Assert(us_Id >= pst_RamBuf->us_ObjSum);
	
	SOS_Assert(pst_RamBuf->ast_Obj[us_Id].ui_Len < ui_Len);
	
	memcpy((uint8_t*)pst_RamBuf->ast_Obj[us_Id].ui_Addr, auc_Buf, ui_Len);
	
	return SYS_YES;
}
/*****************************************************************************/
/**
* @brief  MBuf_Free
*         MBuf池初始化
* @param  uint16_t us_BlockNum：释放块头编号；
* @return void ok
*/
/*****************************************************************************/
int32_t   ReadBuf(uint8_t auc_Ram[], uint16_t us_Id, uint8_t auc_Buf[], uint32_t ui_Len)
{
	SRamBuf *pst_RamBuf = NULLS;
	int32_t  i_Ret = -1;
	
	SOS_Assert(NULLS == auc_Ram);
	
	pst_RamBuf = (SRamBuf *)auc_Ram;
	
	SOS_Assert(auc_Buf == NULLS);
	
	SOS_Assert(SYS_YES != pst_RamBuf->us_Init);
	
	SOS_Assert(us_Id >= pst_RamBuf->us_ObjSum);
	
	SOS_Assert(us_Id != pst_RamBuf->ast_Obj[us_Id].us_Id);	
	
	if (ui_Len > pst_RamBuf->ast_Obj[us_Id].ui_Len)
	{
		memcpy(auc_Buf, (uint8_t*)pst_RamBuf->ast_Obj[us_Id].ui_Addr, 
				pst_RamBuf->ast_Obj[us_Id].ui_Len);
		i_Ret = pst_RamBuf->ast_Obj[us_Id].ui_Len;
	}
	else
	{
		memcpy(auc_Buf, (uint8_t*)pst_RamBuf->ast_Obj[us_Id].ui_Addr, ui_Len);
		i_Ret = ui_Len;
	}
	
	return i_Ret;
}
/*****************************************************************************/
/**
* @brief  MBuf_Free
*         MBuf池初始化
* @param  uint16_t us_BlockNum：释放块头编号；
* @return void ok
*/
/*****************************************************************************/
uint8_t* GetObjAddr(uint8_t auc_Ram[], uint16_t us_Id)
{
	uint8_t *puc_Ret = NULLS;
	SRamBuf *pst_RamBuf = NULLS;
		
	SOS_Assert(NULLS == auc_Ram);
	
	pst_RamBuf = (SRamBuf *)auc_Ram;	
	
	SOS_Assert(SYS_YES != pst_RamBuf->us_Init);
	
	SOS_Assert(us_Id >= pst_RamBuf->us_ObjSum);

	SOS_Assert(us_Id != pst_RamBuf->ast_Obj[us_Id].us_Id);	
		
	puc_Ret = (uint8_t*)pst_RamBuf->ast_Obj[us_Id].ui_Addr;
	
	return puc_Ret;
}

