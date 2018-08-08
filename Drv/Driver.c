/******************************************************************************/
/**
*@file    Driver.c          
*@brief   驱动层源文件              
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
 #include  "Typedef.h"
 #include  "Driver.h"
 #include  "TL16C754.h"
 #include  "Crc.h"
 #include <string.h>

/*****************************************************************************/
/**@brief  驱动列表                                                                    */
/*****************************************************************************/
SDriverMod  g_stDriverLst[MAX_DRIVER_TYPE + 1] = 
 					{
 						{
 							SYS_ERR,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							0	
 						},
 						{
 							SYS_ERR,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							0	
 						},
 						{
 							SYS_YES,
 							SYS_YES,
 							dev_ExUartOpen,
 							dev_ExUartWrite,
 							dev_ExUartRead,
 							dev_ExUartIoctl,
 							0	
 						},
 						{
 							SYS_YES,
 							dev_ExUartOpen,
 							dev_ExUartWrite,
 							dev_ExUartRead,
 							dev_ExUartIoctl,
 							NULLS,
 							0		
 						},
 						{
 							SYS_ERR,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							0		
 						},
 						{
 							SYS_ERR,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							0	
 						},
 						{
 							SYS_ERR,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							NULLS,
 							0	
 						}
 					};

/*****************************************************************************/
/**
* @brief  InitDriverMod
*         初始化驱动模块，生成模块驱动列表校验值
* @param  void
* @return void
*/
/*****************************************************************************/
 uint8_t  InitDriverMod(void)
 {
 	uint8_t  uc_Count = 0;
 	
 	for (uc_Count = 0; uc_Count < (MAX_DRIVER_TYPE + 1); uc_Count++)
 	{
 		g_stDriverLst[uc_Count].us_Crc = getCrc16((uint16_t*)&g_stDriverLst[uc_Count], 
 												  (sizeof(g_stDriverLst[uc_Count]) - 1));	
 	}
 	/*add Crc*/	
 	return SYS_OK;	
 }
 

/*****************************************************************************/
/**
* @brief   uint8_t dev_Open
*         驱动层打开接口
* @param  IN uint8_t uc_Major：主设备号;
* @param  IN uint8_t  uc_Minor:次设备号;
* @param  IN int8_t sc_OpFlag:操作参数用于扩展用，暂未用
* @param  OUT uint16_t *pus_Err：错误信息返回
* @return uint8_t：SYS_YES/SYS_NO
*/
/*****************************************************************************/
 uint8_t dev_Open(IN uint8_t uc_Major, IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag, OUT uint16_t *pus_Err)
 {
 	uint8_t uc_Ret = SYS_OK;
 	
 	SOS_Assert( 0 == uc_Major );
 	
 	SOS_Assert(uc_Major > MAX_DRIVER_TYPE);
 	
 	SOS_Assert(NULLS == pus_Err );
 	
 	if (SYS_OK == g_stDriverLst[uc_Major].uc_Init)
 	{
 		if (NULLS != g_stDriverLst[uc_Major].Open)
 		{
 			uc_Ret = g_stDriverLst[uc_Major].Open(uc_Minor, sc_OpFlag );
 		}
 		else
 		{
 			uc_Ret = SYS_ERR;	
 		}
 	}
 	else
 	{
 		uc_Ret = SYS_ERR;			
 	}

 	return uc_Ret;
 }
 
/*****************************************************************************/
/**
* @brief   uint8_t dev_Write
*         驱动层写接口
* @param  IN uint8_t uc_Major：主设备号;
* @param  IN uint8_t  uc_Minor:次设备号;
* @param   IN char_t *pc_Buf：写入数据
* @param  IN int32_t ul_Len：写入数据长度
* @param  OUT uint16_t *pus_Err：错误信息返回
* @return uint8_t：SYS_YES/SYS_NO
*/
/*****************************************************************************/
 uint8_t dev_Write(IN uint8_t uc_Major, IN uint8_t uc_Minor, IN char_t *pc_Buf, IN int32_t ul_Len, 
 					OUT  uint16_t  *pus_Err)
 {
  	uint8_t uc_Ret = SYS_ERR;
 	
 	SOS_Assert( 0 == uc_Major );
 	
 	SOS_Assert(uc_Major > MAX_DRIVER_TYPE);
 	
 	SOS_Assert(NULLS == pus_Err );
 	
 	SOS_Assert(NULLS == pc_Buf);
 	
 	if (SYS_OK == g_stDriverLst[uc_Major].uc_Init)
 	{
 		if (NULLS != g_stDriverLst[uc_Major].Write)
 		{
			 uc_Ret = g_stDriverLst[uc_Major].Write(uc_Minor, pc_Buf, ul_Len);			
 		}
 		else
 		{
			uc_Ret = SYS_ERR ;			
 		}	
 	}
 	else
 	{
		uc_Ret = SYS_ERR; 		
 	}	

 	return uc_Ret;		
 }
 

/*****************************************************************************/
/**
* @brief  int32_t dev_Read
*         驱动层读接口
* @param  IN uint8_t uc_Major：主设备号;
* @param  IN uint8_t  uc_Minor:次设备号;
* @param  OUT char_t *pc_Buf：读出数据
* @param  IN int32_t ul_Len：数据区长度
* @param  OUT uint16_t *pus_Err：错误信息返回
* @return uint8_t：SYS_YES/SYS_NO
*/
/*****************************************************************************/
int32_t dev_Read(IN uint8_t uc_Major, IN uint8_t uc_Minor, OUT char_t *pc_Buf,  
				 IN int32_t i_Len, OUT uint16_t  *pus_Err)
{
  	int32_t i_Ret = -1;
 	
 	SOS_Assert( 0 == uc_Major );
 	
 	SOS_Assert(uc_Major > MAX_DRIVER_TYPE);
 	
 	SOS_Assert(NULLS == pus_Err );
 	
 	SOS_Assert(NULLS == pc_Buf);

 	if (SYS_OK == g_stDriverLst[uc_Major].uc_Init)
 	{
 		if (NULLS != g_stDriverLst[uc_Major].Read)
 		{
			 i_Ret = g_stDriverLst[uc_Major].Read(uc_Minor, pc_Buf, i_Len);			
 		}
 		else
 		{
			i_Ret = -1 ;			
 		}	
 	}
 	else
 	{
		i_Ret = -1; 		
 	}	

 	return i_Ret;		
}

/*****************************************************************************/
/**
* @brief  uint8_t dev_Ioctl
*         驱动层控制接口
* @param  IN uint8_t uc_Major：主设备号;
* @param  IN uint8_t  uc_Minor:次设备号;
* @param  IN uint32_t ul_Arg：操作参数
* @param  INOUT uint16_t  *pus_Data：信息输入、输出
* @param  OUT uint16_t *pus_Err：错误信息返回
* @return uint8_t：SYS_YES/SYS_NO
*/
/*****************************************************************************/
uint8_t dev_Ioctl(IN uint8_t uc_Major , IN uint8_t uc_Minor, IN uint32_t ul_Arg, INOUT uint16_t  *pus_Data, OUT uint16_t  *pus_Err)
{
	uint8_t uc_Ret = SYS_OK;		
 
 	SOS_Assert( 0 == uc_Major );

 	SOS_Assert(uc_Major > MAX_DRIVER_TYPE);
 	
 	SOS_Assert(NULLS == pus_Err );

 	SOS_Assert(NULLS == pus_Data);

	if (SYS_OK == g_stDriverLst[uc_Major].uc_Init)
 	{
 		if (NULLS != g_stDriverLst[uc_Major].Ioctl)
 		{
			 uc_Ret = g_stDriverLst[uc_Major].Ioctl(uc_Minor,ul_Arg, pus_Data);			
 		}
 		else
 		{
			uc_Ret = SYS_ERR;			
 		}	
 	}
 	else
 	{
 		uc_Ret = SYS_ERR;	
 	}
	
 	return uc_Ret;	 		
}

/*****************************************************************************/
/**
* @brief  uint32_t  dev_check
*         驱动层校验
* @param  IN uint8_t uc_Major：主设备号
* @return uint32_t：错误码
*/
/*****************************************************************************/
uint32_t  dev_check(IN uint8_t uc_Major)
{
	uint32_t ui_Ret = 0;		
 	
 	SOS_Assert( 0 == uc_Major );
 	
 	SOS_Assert(uc_Major > MAX_DRIVER_TYPE);

 	if ( getCrc16((uint16_t*)&g_stDriverLst[uc_Major], 
 		 (sizeof(g_stDriverLst[uc_Major]) - 1)) !=  g_stDriverLst[uc_Major].us_Crc)
 	{
 			/*stop*/
 	}
 	else
 	{
 		if (SYS_OK == g_stDriverLst[uc_Major].uc_Init)
 		{
 			if (NULLS != g_stDriverLst[uc_Major].Check)
 			{
			 	ui_Ret = g_stDriverLst[uc_Major].Check();			
 			}
 			else
 			{
				ui_Ret = 0;			
 			}
 		}
 		else
 		{
 			ui_Ret = 0;	
 		}
 	}	

 	return ui_Ret;	 		
}

/*****************************************************************************/
/**
* @brief  void  dev_DrvRevSchedule
*         驱动层接收处理           //判断串口通信状态 超过两秒没有接受到正确数据，判断为通信中断20170808
* @param  void
* @return void
*/
/*****************************************************************************/
void  dev_DrvRevSchedule(void)
{
	dev_ExUartCheck();
}

/* end */
