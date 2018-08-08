/******************************************************************************/
/**
*@file    TL16C754.c      
*@brief   source file of device TL16C754 driver                   
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/

#include  "Typedef.h"
#include  "Queue.h"
#include  "dev_Timer.h"
#include  "TL16C754.h"
#include  "dev_Gpio.h"
#include  "dev_Interrupt.h"
#include  <string.h>

/*****************************************************************************/
/**@brief  外部串口设备结构数据                                                         */
/*****************************************************************************/
static SExUartDev     gst_ExUartDev[MAX_TL16C754_DEV_NUM];

/*****************************************************************************/
/**@brief  外部串口设备接收队列                                                         */
/*****************************************************************************/
SExUartRevBuf  gst_ExUartRevBuf[MAX_TL16C754_DEV_NUM];

/*****************************************************************************/
/**@brief  外部串口设备发送队列                                                         */
/*****************************************************************************/
SExUartSndBuf  gst_ExUartSndBuf[MAX_TL16C754_DEV_NUM];

#ifdef __NET_CARD__
/*****************************************************************************/
/**@brief  外部串口设备波特率设置数据                                                   */
/*****************************************************************************/
static const  uint32_t   gst_ExUartBaud[MAX_TL16C754_DEV_NUM] =
							{BAUD_TL16C754_CHAN1, BAUD_TL16C754_CHAN2, BAUD_TL16C754_CHAN3, BAUD_TL16C754_CHAN4};

/*****************************************************************************/
/**@brief   外部串口设备配置， 1~(MAX_TL16C754_DEV_NUM-1)有效，0未用                               */
/*****************************************************************************/
static const  uint8_t    gst_ExUartConf[MAX_TL16C754_DEV_NUM] = 
							{ EXUART_CONFIG_CHAN1, EXUART_CONFIG_CHAN2, EXUART_CONFIG_CHAN3, EXUART_CONFIG_CHAN4};
#else
/*****************************************************************************/
/**@brief  外部串口设备波特率设置数据 1~(MAX_TL16C754_DEV_NUM-1)有效，0未用                  */
/*****************************************************************************/
static const  uint32_t   gst_ExUartBaud[MAX_TL16C754_DEV_NUM] =
							{BAUD_TL16C754_CHAN1, BAUD_TL16C754_CHAN2, BAUD_TL16C754_CHAN3, BAUD_TL16C754_CHAN4,
							 BAUD_TL16C754_CHAN5, BAUD_TL16C754_CHAN6, BAUD_TL16C754_CHAN7, BAUD_TL16C754_CHAN8};
/*****************************************************************************/
/**@brief  外部串口设备配置， 1~(MAX_TL16C754_DEV_NUM-1)有效，0未用                                */
/*****************************************************************************/
static const  uint8_t    gst_ExUartConf[MAX_TL16C754_DEV_NUM] = 
							{EXUART_CONFIG_CHAN1, EXUART_CONFIG_CHAN2, EXUART_CONFIG_CHAN3, EXUART_CONFIG_CHAN4,
							 EXUART_CONFIG_CHAN5, EXUART_CONFIG_CHAN6, EXUART_CONFIG_CHAN7, EXUART_CONFIG_CHAN8};
#endif
/*****************************************************************************/
/**@brief  局部函数声明                                                                                                                */
/*****************************************************************************/							
static void  setDivisorLatchTL16C754(IN uint8_t uc_ChanNum, IN uint16_t us_Value);

static void setXOFF1andXON1TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue);

static void setXOFF2andXON2TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue);

static void setEFRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);

static void setTCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);	

static void setTLRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);						

static void setFCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);	

static void  setIERTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value);

static void  dev_ExUartINTHandle(IN uint8_t uc_Minor);	

static  void  dev_ExUartRevHanle(IN uint8_t  uc_Minor);	

static  void  dev_ExUartSndHanle(IN uint8_t  uc_Minor); 

 /*****************************************************************************/
/**
* @brief  uint8_t dev_ExUartOpen
*        	完成串口数据结构初始化和硬件初始化
* @param  IN uint8_t  uc_Minor: 设备编号，表示同类设备排序 
* @param  IN int8_t sc_OpFlag：打开选项，未用 
* @return uint8_t : SYS_YES----成功, SYS_NO----失败
*/
/*****************************************************************************/
uint8_t dev_ExUartOpen(IN uint8_t  uc_Minor,  IN int8_t sc_OpFlag)							
{
	volatile uint8_t*  puc_LCR = NULLS;
	volatile uint8_t*  puc_MCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t   uc_Ret = SYS_OK;
	uint16_t  us_Baud = 0U;

	/* 入参判断 */
 	SOS_Assert( 0U == uc_Minor );
 		
 	SOS_Assert( uc_Minor > MAX_TL16C754_DEV_NUM );
 	uc_Minor = uc_Minor -1;
	/* 寄存器地址计算 */
	ui_Addr = ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;     /*需要打开的当前设备的地址计算 20170808*/
 	puc_LCR = (volatile uint8_t*)(ui_Addr);
 	*puc_LCR = gst_ExUartConf[uc_Minor];                                              /*在该地址写入打开设备的值 20170808*/
 	
 	ui_Addr = ADDR_BASE_TL16C754 + uc_Minor *SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
 	
 	if ( 0 == gst_ExUartBaud[uc_Minor] )
 	{
 		uc_Ret = SYS_ERR;
 	}
 	else
 	{
 		ModINT_Rigster(uc_Minor, uc_Minor, dev_ExUartINTHandle);                 /*理解为中断处理函数，在中断向量表中注册中断函数 20170808*/
 		puc_MCR = (volatile uint8_t*)(ui_Addr);
		*puc_MCR |= 0x008;
 		us_Baud = (CRYSTAL_FREQUENCY_TL16C754/SEL_CLOCK_TL16C754)/(gst_ExUartBaud[uc_Minor]*16)	;
 	
 		/* 设置串口频率 */
 		setDivisorLatchTL16C754(uc_Minor,us_Baud);           /*相应串口波特率 20170808*/
 		
 		setEFRTL16C754(uc_Minor, 0);
 	
 		setXOFF1andXON1TL16C754(uc_Minor, 0,0);
 	
 		setXOFF2andXON2TL16C754(uc_Minor, 0,0);
 	
 		setTCRTL16C754(uc_Minor, 0);
 	
 		setTLRTL16C754(uc_Minor, 0);
 	
 		setFCRTL16C754(uc_Minor, 0x07);
 		
 		
 	
 		/* 初始化数据结构数据 */
 		memset(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall, 0, sizeof(gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall));
 		
 		Queue_Init(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr,QUEUESMALL_LEN_MAX);
 	
 		memset(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge, 0, sizeof(gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge)); 	
 		
 		Queue_Init(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, QUEUELARGE_LEN_MAX);
 		
 		gst_ExUartDev[uc_Minor].us_ErrCode = 0;
 		gst_ExUartDev[uc_Minor].ui_ErrCtr = 0;
 		gst_ExUartRevBuf[uc_Minor].ui_Timer = getTick();
 		gst_ExUartRevBuf[uc_Minor].uc_State = SYS_ERR;
 		gst_ExUartDev[uc_Minor].uc_Init = SYS_OK;
 		
 		setIERTL16C754(uc_Minor, 0x01);                     /*打开中断  20170808*/
 	}
 	return uc_Ret;	
} 

/*****************************************************************************/
/**
* @brief  uint8_t dev_ExUartWrite
*         外部串口模块写接口，用于上层将发送数据写入驱动层发送队列 
* @param  IN uint8_t uc_Minor： 次设备号，代表设备次序 
* @param  IN char_t  auc_Buf[]:发送数据
* @param  IN nt32_t i_Len：发送数据长度
* @return SYS_YES---写操作成功 ,SYS_NO---写操作失败
*/
/*****************************************************************************/
uint8_t dev_ExUartWrite( IN uint8_t uc_Minor, IN char_t ac_Buf[], IN int32_t i_Len)
{
	uint8_t   uc_Ret = SYS_OK;
	volatile   uint8_t   *puc_IER = NULLS;
	/* 入参检查 */
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);
	
	SOS_Assert(NULLS == ac_Buf);
	uc_Minor = uc_Minor -1;
	if ( SYS_OK == gst_ExUartDev[uc_Minor].uc_Init )
	{	
		puc_IER = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor *SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR);
		/*临界保护关闭中断*/
		OS_ENTER_CRITICAL();
		/* 将发送数据帧写入发送队列中 */
		uc_Ret = Queue_Write(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, (uint8_t*)ac_Buf,  i_Len);
		/*使能串口发送中断*/
		*puc_IER |= TL16C754_SND_INT_ENABLE;
		
		if (uc_Ret == SYS_OK)
		{		
		}
		else
		{	
			gst_ExUartDev[uc_Minor].ui_ErrCtr++;
		}
		/*临界保护退出打开中断*/
		OS_EXIT_CRITICAL();
	}
	else
	{
		uc_Ret = SYS_ERR;	
	}

	return uc_Ret;
} 

/*****************************************************************************/
/**
* @brief  int32_t dev_ExUartRead
*         外部串口模块读接口，用于上层读取驱动层接收队列中数据 
* @param  IN uint8_t uc_Minor： 次设备号，代表设备次序 
* @param  OUT char_t auc_Buf[]:数据返回存储区
* @param  IN int32_t i_Len：存储区长度
* @return int32_t：-1---错误，>=0---读取的数据长度
*/
/*****************************************************************************/
int32_t dev_ExUartRead(IN uint8_t uc_Minor, OUT char_t ac_Buf[],IN int32_t i_Len)
{
	int32_t  i_Ret = -1; 
	/* 入参检查 */
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);

	SOS_Assert(NULLS == ac_Buf);	
	uc_Minor = uc_Minor -1;
	if (SYS_OK == gst_ExUartDev[uc_Minor].uc_Init)
	{
		if (SYS_OK == gst_ExUartRevBuf[uc_Minor].uc_State)
		{	/*临界保护关闭中断*/
			OS_ENTER_CRITICAL();
			
			/* 读取串口接收队列数据 */
			i_Ret = Queue_Read(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr, (uint8_t*)ac_Buf, i_Len); 
			/*临界保护退出打开中断*/
			OS_EXIT_CRITICAL();
		}
		else
		{
			i_Ret =  -1;		
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
* @brief  uint8_t  dev_ExUartIoctl
*         外部串口控制操作接口，用于上层获取设备状态、错误状态 
* @param  IN uint8_t uc_Minor： 次设备号，代表设备次序  
* @param  IN uint32_t ui_Arg:   提取参数 ，TL16C754_DATA_VALID---获取接收队列数据有效性，
* 								TL16C754_COM_STATE---获取通信状态，TL16C754_ERR_CODE---获取错误码
* @param  OUT uint16_t *pus_Data:状态返回
* @return uint8_t SYS_YES---控制操作成功 ,SYS_NO---控制操作失败
*/
/*****************************************************************************/
uint8_t  dev_ExUartIoctl(IN uint8_t uc_Minor, IN uint32_t i_Arg, uint16_t *pus_Data)
{
	uint8_t uc_Ret = SYS_OK;
	
	SOS_Assert(0U == uc_Minor);
	
	SOS_Assert(uc_Minor > MAX_TL16C754_DEV_NUM);

	SOS_Assert(NULLS == pus_Data);	
	uc_Minor = uc_Minor -1;
	switch(i_Arg)
	{
		case TL16C754_DATA_VALID: /* 获取接收队列数据有效性 */
		{
			*pus_Data = gst_ExUartRevBuf[uc_Minor].uc_State;
			break;	
		}	
		case  TL16C754_ERR_CODE:/* 获取错误码 */
		{	
			*pus_Data = gst_ExUartDev[uc_Minor].us_ErrCode;
			break;	
		}
		default:
		{
			uc_Ret = SYS_ERR;
			break;	
		}
	}
	
	return uc_Ret;		
}

/*****************************************************************************/
/**
* @brief  uint8_t  dev_ExUartCheck
*         外部串口模块检测，检测所有串门通信状态 
* @param  void 
* @return uint8_t： SYS_YES---操作成功 ,SYS_NO---操作失败
*/
/*****************************************************************************/
uint32_t  dev_ExUartCheck(void)   /*2s 接受 时间限定 20170808*/
{
	uint8_t  uc_Count = 0U;
	uint32_t ui_ErrCode = 0U;
	
	for (uc_Count = 0; uc_Count < MAX_TL16C754_DEV_NUM; uc_Count++)
	{
		if (SYS_OK == gst_ExUartDev[uc_Count].uc_Init)
		{	
			if (SYS_OK == IsTimeOut(TIMER_OVER_COM_TL16C754, gst_ExUartRevBuf[uc_Count].ui_Timer))
			{
				gst_ExUartRevBuf[uc_Count].uc_State = SYS_ERR;	
			}
			else
			{	
			}
		}
		else
		{
			gst_ExUartRevBuf[uc_Count].uc_State = SYS_ERR;	
		}
	}	
	return ui_ErrCode;
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartINTHandle
*         串口中断处理，完成串口中断状态扫描、数据接收与发送
* @param  IN uint8_t uc_Minor：次设备号 
* @return void
*/
/*****************************************************************************/
static void  dev_ExUartINTHandle(IN uint8_t uc_Minor)
{
	volatile uint8_t* puc_IIR = NULLS;
	volatile uint8_t   uc_INTFlag = 0;
	
	if (SYS_OK == gst_ExUartDev[uc_Minor].uc_Init)
	{	/*读取中断标志寄存器*/
		puc_IIR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor * SPACE_ADDR_CHANNEL + IIR_RELATIVE_ADDR);
		uc_INTFlag = *puc_IIR;
		/*如有中断进入处理,退出条件是所有中断事件处理完成，中断标志位为零*/
		while((uc_INTFlag&MASK_INT_TLC754) != 0)
		{	/*查询是否是接收中断*/
			if ((uc_INTFlag&RHR_INTERRUPT)==RHR_INTERRUPT)
			{	/*串口数据接收处理*/
				dev_ExUartRevHanle(uc_Minor);	
			}
			/*查询是否是发送中断*/
			if ((uc_INTFlag & THR_INTERRUPT)==THR_INTERRUPT)
			{	/*串口数据发送处理*/
				dev_ExUartSndHanle(uc_Minor);
			}
			uc_INTFlag = *puc_IIR;
		}
	}
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartRevHanle
*         串口接收处理，完成串口FIFO读取
* @param  IN uint8_t uc_Minor：次设备号 
* @return void
*/
/*****************************************************************************/
static  void  dev_ExUartRevHanle(IN uint8_t  uc_Minor)
{
	volatile uint8_t*  puc_LSR = NULLS;
	volatile uint8_t*  puc_RHR = NULLS;
	volatile uint8_t   uc_RHRState = 0;
	uint8_t   auc_Data[TH_REV_DATA_CNT_TL16C754];
	int32_t   i_Len = 0;
	/*获取线性状态寄存器地址*/
	puc_LSR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + LSR_RELATIVE_ADDR); 
	/*获取接收寄存器地址*/		
	puc_RHR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + RHR_RELATIVE_ADDR);
	/*读取接收状态*/	
	uc_RHRState = (*puc_LSR) & MASK_RXHOLD_STATE_LSR;
	i_Len = 0;
	/*如果有接收FIFO有数据进行读取，读取退出条件是数据读取完成或数据接收缓存满*/
	while((MASK_RXHOLD_STATE_LSR == uc_RHRState)&&(i_Len < TH_REV_DATA_CNT_TL16C754))
	{	
		auc_Data[i_Len] = *puc_RHR&0XFF;
		i_Len++;
		uc_RHRState = (*puc_LSR)&MASK_RXHOLD_STATE_LSR;
	}
	/*如读取数据将数据写入接收队列中*/
	if ( i_Len > 0 )
	{	
		if( SYS_OK == Queue_Write(&gst_ExUartRevBuf[uc_Minor].st_RevQueueSmall.st_Hdr
								, auc_Data , i_Len))
		{
			gst_ExUartRevBuf[uc_Minor].ui_Timer= getTick();
			gst_ExUartRevBuf[uc_Minor].uc_State = SYS_OK;
		}
		else
		{
		
		}				
	}			
}

/*****************************************************************************/
/**
* @brief  void  dev_ExUartSndHanle
*         串口发数处理，完成串口数据写入发送FIFO中
* @param  IN uint8_t uc_Minor：次设备号 
* @return void
*/
/*****************************************************************************/
static  void  dev_ExUartSndHanle(uint8_t  uc_Minor) 
{
	volatile uint8_t* puc_THR = NULLS;
	volatile uint8_t* puc_IER = NULLS;
	uint8_t   auc_Data[TH_REV_DATA_CNT_TL16C754];
	uint8_t   uc_Cnt = 0;
	int32_t   i_Ret = 0;
	/*获取串口发送地址*/
	puc_THR = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + THR_RELATIVE_ADDR); 
	/*从发送队列中读取数据，每次读取TH_SND_DATA_NUM_TL16C754长度*/		
	i_Ret = Queue_Read(&gst_ExUartSndBuf[uc_Minor].st_SndQueueLarge.st_Hdr, auc_Data, TH_SND_DATA_NUM_TL16C754);
	/*如读到数据将其写入发送FIFO中，否则关闭发送中断*/			
	if (i_Ret > 0)
	{
		for (uc_Cnt = 0; uc_Cnt < i_Ret; uc_Cnt++)
		{						
			*puc_THR = auc_Data[uc_Cnt];
		}
	}
	else
	{
		puc_IER = (volatile uint8_t*)(ADDR_BASE_TL16C754 + uc_Minor*SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR);
		*puc_IER &= (~TL16C754_SND_INT_ENABLE);
	}	
}
/*****************************************************************************/
/**
* @brief  void  setDivisorLatchTL16C754
*         设置TL16C754串口锁相环寄存
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint16_t us_Value：写入值
* @return void
*/
/*****************************************************************************/
static void  setDivisorLatchTL16C754(IN uint8_t uc_ChanNum, IN uint16_t us_Value)  
{                           /*功能函数，用于设置TL16C754的想也难怪串口的波特率 20170808*/
	volatile uint8_t *puc_DLL = NULLS;
	volatile uint8_t *puc_DLH = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + DLL_RELATIVE_ADDR;
	puc_DLL = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + DLH_RELATIVE_ADDR;
	puc_DLH = (volatile uint8_t*)(ui_Addr);
		
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR |= 0x80U;
	*puc_DLL = us_Value&0xffU;
	*puc_DLH = (us_Value>>8)&0xffU;
	*puc_LCR = uc_Tmp;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setXOFF1andXON1TL16C754
*         设置TL16C754串口XOFF1寄存器和XON1寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_XoffValue：XOFF1寄存器写入值
* @param  IN uint8_t uc_XonValue：XON1寄存器写入值
* @return void
*/
/*****************************************************************************/
static void setXOFF1andXON1TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue)
{
	volatile uint8_t *puc_XOFF1 = NULLS;
	volatile uint8_t *puc_XON1 = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XOFF1_RELATIVE_ADDR;
	puc_XOFF1 = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + XON1_RELATIVE_ADDR;
	puc_XON1 = (volatile uint8_t*)(ui_Addr);	
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_XOFF1 = uc_XoffValue;
	*puc_XON1 = uc_XonValue;
	*puc_LCR = uc_Tmp;
	
	return;
}
/*****************************************************************************/
/**
* @brief  void setXOFF2andXON2TL16C754
*         设置TL16C754串口XOFF1寄存器和XON1寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_XoffValue：XOFF2寄存器写入值
* @param  IN uint8_t uc_XonValue：XON2寄存器写入值
* @return void
*/
/*****************************************************************************/
static void setXOFF2andXON2TL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_XoffValue, IN uint8_t uc_XonValue)
{
	volatile uint8_t *puc_XOFF2 = NULLS;
	volatile uint8_t *puc_XON2 = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t  ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + XOFF2_RELATIVE_ADDR;
	puc_XOFF2 = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XON2_RELATIVE_ADDR;
	puc_XON2 = (volatile uint8_t*)(ui_Addr);	
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_XOFF2 = uc_XoffValue;
	*puc_XON2 = uc_XonValue;
	*puc_LCR = uc_Tmp;

	return;
}
/*****************************************************************************/
/**
* @brief  void setEFRTL16C754
*         设置TL16C754串口EFR寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value：写入值
* @return void
*/
/*****************************************************************************/
static void setEFRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_Tmp = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + XOFF2_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);

	uc_Tmp = *puc_LCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_Value;
	*puc_LCR = uc_Tmp;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setTCRTL16C754
*         设置TL16C754串口TCR寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value：写入值
* @return void
*/
/*****************************************************************************/
static void setTCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_MCR = NULLS;
	volatile uint8_t *puc_TCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	uint8_t uc_TmpMCR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);

	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
	puc_MCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + TCR_RELATIVE_ADDR;
	puc_TCR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;	
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
	
	uc_TmpMCR = *puc_MCR;
	
	*puc_MCR |= 0x40U; 
	*puc_TCR = uc_Value;
	*puc_MCR = uc_TmpMCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;

	return;
}


/*****************************************************************************/
/**
* @brief  void setTCRTL16C754
*         设置TL16C754串口TLR寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value：写入值
* @return void
*/
/*****************************************************************************/
static void setTLRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_MCR = NULLS;
	volatile uint8_t *puc_TLR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	uint8_t uc_TmpMCR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + MCR_RELATIVE_ADDR;
	puc_MCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + TLR_RELATIVE_ADDR;
	puc_TLR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
		
	uc_TmpMCR = *puc_MCR;
	*puc_MCR |= 0x40U;
	*puc_TLR = uc_Value;
	
	*puc_MCR = uc_TmpMCR;
	*puc_LCR = 0xBF;
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;
	
	return;
}

/*****************************************************************************/
/**
* @brief  void setFCRTL16C754
*         设置TL16C754串口FCR寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value：写入值
* @return void
*/
/*****************************************************************************/
static void setFCRTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_EFR = NULLS;
	volatile uint8_t *puc_LCR = NULLS;
	volatile uint8_t *puc_FCR = NULLS;
	uint32_t ui_Addr = 0U;
	uint8_t uc_TmpLCR = 0U;
	uint8_t uc_TmpEFR = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + EFR_RELATIVE_ADDR;
	puc_EFR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + LCR_RELATIVE_ADDR;
	puc_LCR = (volatile uint8_t*)(ui_Addr);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum*SPACE_ADDR_CHANNEL + FCR_RELATIVE_ADDR;
	puc_FCR = (volatile uint8_t*)(ui_Addr);
	
	uc_TmpLCR = *puc_LCR;
	*puc_LCR = 0xBF;
	
	uc_TmpEFR = *puc_EFR;
	*puc_EFR |= 0x10U;
	*puc_LCR = 0x00;
	*puc_FCR = uc_Value;
	*puc_LCR = 0xBF;
	
	*puc_EFR = uc_TmpEFR;
	*puc_LCR = uc_TmpLCR;

	return;
}
/*****************************************************************************/
/**
* @brief  void setIERTL16C754
*         设置TL16C754串口中断使能寄存器
* @param  IN uint8_t uc_ChanNum：串口号，0- (MAX_TL16C754_DEV_NUM -1)
* @param  IN uint8_t uc_Value：写入值
* @return void
*/
/*****************************************************************************/
static void  setIERTL16C754(IN uint8_t uc_ChanNum, IN uint8_t uc_Value)
{
	volatile uint8_t *puc_IER = NULLS;
	uint32_t  ui_Addr = 0U;
	
	SOS_Assert(uc_ChanNum >= MAX_TL16C754_DEV_NUM);
	
	ui_Addr = ADDR_BASE_TL16C754 + uc_ChanNum *SPACE_ADDR_CHANNEL + IER_RELATIVE_ADDR;
	puc_IER = (volatile uint8_t*)(ui_Addr);

	*puc_IER = uc_Value;
	
	return;
}
