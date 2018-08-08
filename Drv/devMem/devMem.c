/**
 * @file   Mem.c                                                   
 * @brief                                                                                                     
 *            
 * @writer                                       
 * @version  1.00
 * @date     2012.04.03
 * @history
  * @modify list :
 */
 
 #include  "devMem.h"
 
 /**
 * @brief  内存检测
 *
 * @param  void
 * @param  void		
 * @return SYS_OK---内存正确，SYS_ERR---内存故障
 * @see OK
*/
 uint8_t  MemCheck(void)
 {
 	uint32_t ui_Count = 0;
 	uint8_t uc_Ret = 0;
 	uint16_t  aa;

 //	pus_Mem = (uint16_t*)MEM_BASE_ADDR;
 	
 	for (ui_Count = 0; ui_Count < 0X50; ui_Count++)	
 	{
 		aa = 0xAA00;
 
 		if (aa != 0xAA00)
 		{
 			uc_Ret = SYS_ERR;
 			break;
 		}
 		else
 		{
 			uc_Ret = SYS_OK;	
 		}
 		aa++;
 	}
 	return uc_Ret;
}

/**
 * @brief  检测内存总线
 * @param  volatile uint16_t * pus_Addr：检测起始地址
 * @param  		
 * @return SYS_OK---内存正确，SYS_ERR---内存故障
 * @see OK
*/
uint16_t MemTestDataBus( volatile uint16_t * pus_Addr )
{
    uint16_t us_Pattern = 0;

    for (us_Pattern = 1; us_Pattern != 0; us_Pattern <<= 1)
    {
        *pus_Addr = us_Pattern;                               /*在固定地址写入数据 20170504*/

        if (*pus_Addr != us_Pattern)                          /*从固定地址读出数据 20170504*/
        {
            return (us_Pattern);
        }
    }
    return (SYS_OK);
}

/**
 * @brief  检测内存总线
 * @param  IN volatile uint16_t* pus_BaseAddr：检测内存起始地址
 * 		   IN uint16_t us_Bytes:检测内存区域大小
 * @param  		
 * @return SYS_OK---内存正确，SYS_ERR---内存故障
 * @see OK
*/
uint16_t * MemTestAddressBus(IN volatile uint16_t* pus_BaseAddr,IN uint32_t ui_Bytes)
{
    uint32_t ui_AddreMask = 0;
    uint32_t ui_Offset = 0;
    uint16_t us_TestOffset = 0;
    uint16_t us_Pattern = 0;
    uint16_t us_AntiPattern = 0;

    ui_AddreMask = (ui_Bytes/sizeof(uint16_t) - 1);
    us_Pattern   = (uint16_t)0xAAAA;
    us_AntiPattern = (uint16_t)0x5555;

    for (ui_Offset = 1; (ui_Offset & ui_AddreMask) != 0; ui_Offset <<= 1)
    {
        pus_BaseAddr[ui_Offset] = us_Pattern;
    }
    
    us_TestOffset = 0;
    pus_BaseAddr[us_TestOffset] = us_AntiPattern;

    for (ui_Offset = 1; (ui_Offset & ui_AddreMask) != 0; ui_Offset <<= 1)
    {

        if (pus_BaseAddr[ui_Offset] != us_Pattern)
        {
            return ((uint16_t*) &pus_BaseAddr[ui_Offset]);
        }
    }

    pus_BaseAddr[us_TestOffset] = us_Pattern;

    for (us_TestOffset = 1; (us_TestOffset & ui_AddreMask) != 0; us_TestOffset <<= 1)
    {
        pus_BaseAddr[us_TestOffset] = us_AntiPattern;

        if (pus_BaseAddr[0] != us_Pattern)
        {
            return ((uint16_t *) &pus_BaseAddr[us_TestOffset]);
        }

        for (ui_Offset = 1; (ui_Offset & ui_AddreMask) != 0; ui_Offset <<= 1)
        {
            if ((pus_BaseAddr[ui_Offset] != us_Pattern) && (ui_Offset != us_TestOffset))
            {
                return ((uint16_t *) &pus_BaseAddr[us_TestOffset]);
            }
        }

        pus_BaseAddr[us_TestOffset] = us_Pattern;
    }

    return ((void*)0);
}

/**
 * @brief  检测内存区正确性
 * @param  IN volatile uint16_t* pus_BaseAddr：检测内存起始地址
 * 		   IN uint16_t us_Bytes:检测内存区域大小
 * @param  		
 * @return SYS_OK---内存正确，SYS_ERR---内存故障
 * @see OK
*/
uint16_t* MemTestDevice( volatile uint32_t* pui_BaseAddr, uint32_t ui_Bytes )   
{
    uint32_t ui_Offset = 0;
    uint32_t ui_Words = 0;
    uint32_t ui_Pattern = 0;
    uint32_t ui_Antipattern = 0;

    ui_Words = ui_Bytes/sizeof(uint32_t);

    for (ui_Pattern = 1, ui_Offset = 0; ui_Offset < ui_Words; ui_Pattern++, ui_Offset++)
    {
        pui_BaseAddr[ui_Offset] = ui_Pattern;
    }

    for (ui_Pattern = 1, ui_Offset = 0; ui_Offset < ui_Words; ui_Pattern++, ui_Offset++)
    {
        if (pui_BaseAddr[ui_Offset] != ui_Pattern)
        {
            return ((uint16_t *) &pui_BaseAddr[ui_Offset]);
        }
        ui_Antipattern = ~ui_Pattern;
        pui_BaseAddr[ui_Offset] = ui_Antipattern;
    }

    for (ui_Pattern = 1, ui_Offset = 0; ui_Offset < ui_Words; ui_Pattern++, ui_Offset++)
    {
        ui_Antipattern = ~ui_Pattern;
        
        if (pui_BaseAddr[ui_Offset] != ui_Antipattern)
        {
            return ((uint16_t *) &pui_BaseAddr[ui_Offset]);
        }
    }

    for (ui_Pattern = 1, ui_Offset = 0; ui_Offset < ui_Words; ui_Pattern++, ui_Offset++)
    {
        pui_BaseAddr[ui_Offset] = 0x0;
    }
    return ((void*)0);
}
