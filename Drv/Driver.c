/******************************************************************************/
/**
*@file    Driver.c          
*@brief   ������Դ�ļ�              
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
/**@brief  �����б�                                                                    */
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
*         ��ʼ������ģ�飬����ģ�������б�У��ֵ
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
*         ������򿪽ӿ�
* @param  IN uint8_t uc_Major�����豸��;
* @param  IN uint8_t  uc_Minor:���豸��;
* @param  IN int8_t sc_OpFlag:��������������չ�ã���δ��
* @param  OUT uint16_t *pus_Err��������Ϣ����
* @return uint8_t��SYS_YES/SYS_NO
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
*         ������д�ӿ�
* @param  IN uint8_t uc_Major�����豸��;
* @param  IN uint8_t  uc_Minor:���豸��;
* @param   IN char_t *pc_Buf��д������
* @param  IN int32_t ul_Len��д�����ݳ���
* @param  OUT uint16_t *pus_Err��������Ϣ����
* @return uint8_t��SYS_YES/SYS_NO
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
*         ��������ӿ�
* @param  IN uint8_t uc_Major�����豸��;
* @param  IN uint8_t  uc_Minor:���豸��;
* @param  OUT char_t *pc_Buf����������
* @param  IN int32_t ul_Len������������
* @param  OUT uint16_t *pus_Err��������Ϣ����
* @return uint8_t��SYS_YES/SYS_NO
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
*         ��������ƽӿ�
* @param  IN uint8_t uc_Major�����豸��;
* @param  IN uint8_t  uc_Minor:���豸��;
* @param  IN uint32_t ul_Arg����������
* @param  INOUT uint16_t  *pus_Data����Ϣ���롢���
* @param  OUT uint16_t *pus_Err��������Ϣ����
* @return uint8_t��SYS_YES/SYS_NO
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
*         ������У��
* @param  IN uint8_t uc_Major�����豸��
* @return uint32_t��������
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
*         ��������մ���           //�жϴ���ͨ��״̬ ��������û�н��ܵ���ȷ���ݣ��ж�Ϊͨ���ж�20170808
* @param  void
* @return void
*/
/*****************************************************************************/
void  dev_DrvRevSchedule(void)
{
	dev_ExUartCheck();
}

/* end */
