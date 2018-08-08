/******************************************************************************/
/**
*@file    W5300.c       
*@brief   source file of device W5300 driver                   
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief  include                                                           */
/*****************************************************************************/
#include  "Typedef.h" 
#ifdef __NET_CARD__
#include  <string.h>
#include  "dev_Gpio.h"
#include  "W5300.h"
#include  "dev_Timer.h"
#include  "dev_Interrupt.h"

/*****************************************************************************/
/**@brief  网络接口信息结构                                                            */
/*****************************************************************************/
SNetInfor   gast_NetInfor[ETHERNET_MAX_NUM][W5300_SOCK_NUM];


/*****************************************************************************/
/**@brief  局部函数声明                                                                                                                */
/*****************************************************************************/
static void  W5300_RST(uint8_t uc_Port);

static  void W5300_INTHandle(uint8_t uc_ChipNum);

static void W5300_SockWrite(uint8_t  uc_ChipNum, uint8_t uc_SockNum, SSockAddr *pst_SockAddr, 
							uint8_t auc_Buf[], uint16_t us_Len); 

static void  W5300_SockSendHandle(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum);

static void W5300_SockRevHandle(uint8_t  uc_ChipNum, uint8_t uc_SockNum);
/*****************************************************************************/
/**
* @brief  uint8_t W5300_Init
*         W5300初始化，初始化网络芯片的本地IP、网关、子网掩码、MAC地址
* @param  IN uint8_t  uc_Port：网络接口，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint32_t  ui_IpAddr：网络本地IP地址
* @return uint8_t：SYS_YES---操作正常，否则宕机
*/
/*****************************************************************************/
uint8_t W5300_Init(IN uint8_t  uc_Port , IN uint32_t  ui_IpAddr,IN uint32_t ui_GateWay, IN uint32_t ui_Mask, IN uint8_t uc_Ver)
{
	volatile uint16_t *pus_Reg = NULLS;

	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);

		/*向中断模块注册中断处理向量*/
	ModINT_Rigster(W5300_INT_START_BIT + uc_Port, uc_Port, W5300_INTHandle);
	
	memset(gast_NetInfor[uc_Port], 0, sizeof(gast_NetInfor[uc_Port]));
	/*复位W5300*/
	W5300_RST(uc_Port);
	/*获取网关寄存器地址进行网关设置*/
	pus_Reg =( volatile uint16_t*)GAR(uc_Port);					
	if (uc_Ver == VERSION1)
	{
		*pus_Reg = ui_IpAddr>>16;
		pus_Reg = pus_Reg + 2;
		*pus_Reg = (ui_IpAddr&0xFF00)|0x0001;	
	}
	else
	{
		*pus_Reg = ui_GateWay>>16;
		pus_Reg = pus_Reg + 2;
		*pus_Reg = ui_GateWay&0xFFFF;
	}
	/*获取SIPR寄存器地址进行本地网络地址设置*/
	pus_Reg = ( volatile uint16_t*)SIPR(uc_Port);
	
	*pus_Reg = ui_IpAddr>>16;
	pus_Reg = pus_Reg + 2;
	*pus_Reg = (uint16_t)(ui_IpAddr&0xFFFF);
	
	/*获取SUBR寄存器地址进行子网掩码设置*/
	pus_Reg = ( volatile uint16_t*)SUBR(uc_Port);
	if (uc_Ver == VERSION1)
	{
		*pus_Reg = (MASK_SUBNET>>16);
	
		pus_Reg = pus_Reg + 2;
	
		*pus_Reg = (uint16_t)(MASK_SUBNET&0xFFFF);		
	}
	else
	{
		*pus_Reg = (ui_Mask>>16);
	
		pus_Reg = pus_Reg + 2;
	
		*pus_Reg = (uint16_t)(ui_Mask&0xFFFF);
	}
	/*获取SHAR寄存器地址进行MAC地址设置*/
	pus_Reg = ( volatile uint16_t*)SHAR(uc_Port);
	
	*pus_Reg = MAC_ADDR_12;
	
	pus_Reg = pus_Reg + 2;
	
	*pus_Reg = ui_IpAddr>>16;
	
	pus_Reg = pus_Reg + 2;
	
	*pus_Reg = (uint16_t)(ui_IpAddr&0xFFFF);
	/*设置RTR值和RCR以确定重发时间*/
	pus_Reg = ( volatile uint16_t*)RTR(uc_Port);
	*pus_Reg = 40;
	pus_Reg = ( volatile uint16_t*)RCR(uc_Port);
	*pus_Reg = 3;

	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint8_t  WD5300_SetSocketUDP
*         W5300Socket设置，设置SOCKET的本地端口号、MTU、设置为UDP模式
* @param  IN uint8_t  uc_Port：网络接口，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr：配置本地SOCK参数
* @return uint8_t：SYS_YES---操作正常，否则宕机
*/
/*****************************************************************************/
uint8_t  WD5300_SetSocketUDP(IN uint8_t uc_Port,IN uint8_t uc_SockNum, IN SSockAddr* pst_SockAddr)
{
	volatile  uint16_t  *ptr = NULLS;
	uint16_t  i = 0;
	uint8_t   uc_Ret = SYS_NO;
	uint16_t  us_Bit = 0x01;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	/* 设置本地端口号*/
	ptr = ( volatile uint16_t*)SN_PORTR(uc_Port, uc_SockNum);
	*ptr= pst_SockAddr->sin_port;

	/* 设置SOCKET最大MTU */
	ptr=( volatile uint16_t*)SN_MSSR(uc_Port, uc_SockNum);
	*ptr = MTU_DEFAULT_W5300;
   	
	/* 设置SOCKET为UDP模式 */
	ptr=( volatile uint16_t*)SN_MR(uc_Port, uc_SockNum);
	*ptr=SN_MR_UDP;

	/* 打开SOCKRT*/
	ptr=( volatile uint16_t*)SN_CR(uc_Port, uc_SockNum);
	*ptr=SN_CR_OPEN;
	
	
	ptr=( volatile uint16_t*)SN_SSR(uc_Port, uc_SockNum);
	i=*ptr;

	if((i&0x00ff)!=SOCK_UDP)		/* If failed to open Socket0, close Socket0 and reurn false */
	{
		ptr=( volatile uint16_t*)SN_CR(uc_Port, uc_SockNum);
		
		*ptr=SN_CR_CLOSE;
		
		uc_Ret = SYS_NO;
		gast_NetInfor[uc_Port][uc_SockNum].uc_Init = SYS_NO;
	}
	else
	{
		gast_NetInfor[uc_Port][uc_SockNum].uc_Init = SYS_YES;
		uc_Ret = SYS_YES;
	}
	
	ptr = ( volatile uint16_t*)SN_IMR(uc_Port, uc_SockNum);
	*ptr = 0;
	*ptr |= (SOCKET_REV_INT_BIT|SOCKET_SENOK_TIMEROUT_BIT);
	
	ptr = ( volatile uint16_t*)IMR(uc_Port);
	*ptr |= (us_Bit<<uc_SockNum);
	
	return uc_Ret;
}

/*****************************************************************************/
/**
* @brief  uint8_t  W5300_SockSnd
*         W5300Sock接口发送
* @param  IN uint8_t  uc_Port：网络接口，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr：目的地址；
* @param  IN uint8_t auc_Buf[]：发送信息
* @param  IN uint16_t us_Len：发送数据长度
* @return uint8_t：SYS_YES---操作正常，否则宕机
*/
/*****************************************************************************/
uint8_t W5300_SockSnd(IN uint8_t  uc_Port, IN uint8_t uc_SockNum, IN SSockAddr *pst_SockAddr,IN uint8_t auc_Buf[], IN uint16_t us_Len)
{
	uint8_t  uc_Tail = 0;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	
	/*如果SOCK接口已初始化，进行发送操作*/
	if (SYS_YES == gast_NetInfor[uc_Port][uc_SockNum].uc_Init)
	{	/*如果长度超出门限或地址为零不进行发送操作*/
		if ((us_Len <= W300_REV_BUF_LEN)&&(pst_SockAddr->sin_addr != 0))
		{	/*临界保护关闭中断*/
			OS_ENTER_CRITICAL();
			/*如果已经启动发送中断则将发送数据写入发送队列中由中断处理程序进行发送，否则直接进行发送操作*/
			if (gast_NetInfor[uc_Port][uc_SockNum].uc_FistSnd == SYS_YES)
			{
				uc_Tail = gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail;
		
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].st_SockAddr = *pst_SockAddr;
			
				memcpy(gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].auc_Buf, auc_Buf, us_Len);
			
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.ast_Buf[uc_Tail].us_Len = us_Len;
			
				gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail 
						= (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Tail + 1)%MAX_W5300_QUEUE_LEN;
			
				if (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Cnt >= MAX_W5300_QUEUE_LEN)
				{
					gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Hdr 
						= (gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
				}
				else
				{
					gast_NetInfor[uc_Port][uc_SockNum].st_SndQueue.uc_Cnt++;
				}
				
			}
			else
			{	/*将发送数据写入芯片发送存储区内*/
				W5300_SockWrite(uc_Port, uc_SockNum, pst_SockAddr, auc_Buf, us_Len);
				gast_NetInfor[uc_Port][uc_SockNum].uc_FistSnd = SYS_YES;
			}
			/*退出临界区*/
			OS_EXIT_CRITICAL();
		}
		else
		{
			
		}	
	}
	return SYS_YES;
}

/*****************************************************************************/
/**
* @brief  uint8_t  W5300_SockRev
*         W5300Sock接口数据读取
* @param  IN uint8_t  uc_Port：网络接口，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @param  OUT uint32_t*  pui_Addr：接收数据的源地址；
* @param  OUT uint8_t auc_Buf[]：数据存储区
* @param  IN uint16_t us_Len：数据存储区长度
* @return uint16_t：返回读取数据长度
*/
/*****************************************************************************/
uint16_t   W5300_SockRev(IN uint8_t  uc_Port,IN uint8_t uc_SockNum, OUT uint32_t*  pui_Addr, 
						OUT uint8_t auc_Buf[],IN uint16_t us_Len)
{   /*从w5300中一次读取一帧的数据到auc_Buf中，并得到读取的数据长度和数据的IP地址 20170808*/
	uint16_t  us_Ret = 0;
	uint8_t   us_Hdr = 0;
	
	SOS_Assert (uc_Port >= ETHERNET_MAX_NUM);        /*uc_Port 使用的是哪块网卡 20170808*/
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);      /*W5300的Socket编号 20170808*/
	
	SOS_Assert (pui_Addr == NULLS)                 /*接受数据源地址RemountIP 20170808*/
	/*如果SOCK接口已初始化，可进行数据读取*/
	if (gast_NetInfor[uc_Port][uc_SockNum].uc_Init == SYS_YES)
	{	/*临界保护关闭中断*/
		OS_ENTER_CRITICAL();
		/*接收队列中是否有数据，如有进行读取*/
		if (gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Cnt > 0)
		{
			us_Hdr = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr;
			/*接收存储区长度是否满足队列中数据包的长度，如满足则读取数据否则返回数据长度为0*/
			if (us_Len > gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len)
			{
				memcpy(auc_Buf,gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].auc_Buf,
						gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len );
						
				us_Ret = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len;
				
				*pui_Addr = gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].ui_Addr;
				
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.ast_Buf[us_Hdr].us_Len = 0;
				
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr = (gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
				gast_NetInfor[uc_Port][uc_SockNum].st_RevQueue.uc_Cnt--;
			}
			else
			{
				*pui_Addr = 0;
				us_Ret = 0;
			}
		}
		else
		{
			*pui_Addr = 0;
			us_Ret = 0;
		}
		/*退出临界区*/
		OS_EXIT_CRITICAL();
	}
	else
	{/*网卡错误退出 20170808*/
		*pui_Addr = 0;
		us_Ret = 0;
	}	
	return us_Ret;	
}

/*****************************************************************************/
/**
* @brief  W5300_SockSendHandle
*         发送处理
* @param  IN uint8_t  uc_ChipNum：芯片号，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @param  OUT uint32_t*  pui_Addr：接收数据的源地址；
* @param  OUT uint8_t auc_Buf[]：数据存储区
* @param  IN uint16_t us_Len：数据存储区长度
* @return uint16_t：返回读取数据长度
*/
/*****************************************************************************/
static void  W5300_SockSendHandle(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum)
{
	uint8_t   uc_Hdr = 0;
	SSockAddr st_SockAddr ;
	uint16_t  us_Len = 0;
	
	SOS_Assert (uc_ChipNum >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	/*发送队列中是否有数据，有提取数据进行进行发送，否则置首次发送为无*/
	if (gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Cnt > 0)	
	{
		uc_Hdr = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr;
		us_Len = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].us_Len;
		st_SockAddr = gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].st_SockAddr;
		/*将发送数据写入芯片SOCK接口存储区内*/
		W5300_SockWrite(uc_ChipNum, uc_SockNum, &st_SockAddr,
						gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.ast_Buf[uc_Hdr].auc_Buf, us_Len);
						
		gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr 
					= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;
		gast_NetInfor[uc_ChipNum][uc_SockNum].st_SndQueue.uc_Cnt--;	
	}
	else
	{	
		gast_NetInfor[uc_ChipNum][uc_SockNum].uc_FistSnd = SYS_NO;
	}
}

/*****************************************************************************/
/**
* @brief  W5300_SockSendHandle
*         发送处理
* @param  IN uint8_t  uc_ChipNum：芯片号，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @param  IN SSockAddr st_SockAddr：发送数据的目的地址；
* @param  IN uint8_t auc_Buf[]：发送数据
* @param  IN uint16_t us_Len：数据长度
* @return void
*/
/*****************************************************************************/
static void W5300_SockWrite(IN uint8_t  uc_ChipNum,IN uint8_t uc_SockNum,IN SSockAddr *pst_SockAddr,IN uint8_t auc_Buf[],IN uint16_t us_Len) 
{
	volatile uint16_t *ptr = NULLS;
	uint16_t i = 0U; 
	uint16_t us_Data = 0U;
	uint16_t us_Sum = 0U;
	
	SOS_Assert (uc_ChipNum >= ETHERNET_MAX_NUM);
	
	SOS_Assert (uc_SockNum >= W5300_SOCK_NUM);
	
	SOS_Assert (pst_SockAddr == NULLS);
	/*获取目的IP寄存器地址*/
	ptr = ( volatile uint16_t*)SN_DIPR(uc_ChipNum, uc_SockNum);
	/*设置目的IP寄存器*/
	*ptr = pst_SockAddr->sin_addr>>16;
	ptr = ptr +2;
	*ptr = (uint16_t)(pst_SockAddr->sin_addr&0XFFFF);
	/*设置目的端口号*/
	ptr=( volatile uint16_t*)SN_DPORTR(uc_ChipNum, uc_SockNum);
	
	*ptr= pst_SockAddr->sin_port;
	/*按16bit对齐*/
	if(us_Len&0x0001)
	{
		us_Sum=(us_Len+1)/2;
	}
	else
	{
		us_Sum=us_Len/2;
	}
	/*获取SOCKET发送FIFO寄存器地址*/
	ptr=( volatile uint16_t*)SN_TX_FIFOR(uc_ChipNum, uc_SockNum);
	
	for(i=0; i<us_Sum;i++)
	{
		us_Data = (auc_Buf[2*i]<<8)|(auc_Buf[2*i + 1]);
		*ptr = us_Data;
	}
	/*设置发送数据长度*/
	ptr=( volatile uint16_t*)SN_TX_WRSR(uc_ChipNum, uc_SockNum);
	*ptr =0;
	ptr = ptr  + 2;
	*ptr = us_Len;
	/*启动发送*/
	ptr=( volatile uint16_t*)SN_CR(uc_ChipNum, uc_SockNum);
	*ptr= SN_CR_SEND;
	
	return ;
}



/*****************************************************************************/
/**
* @brief  W5300_SockRevHandle
*         接收处理，读取接收BUF
* @param  IN uint8_t  uc_ChipNum：芯片号，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @param  IN uint8_t uc_SockNum:SOCKET号，0- W5300_SOCK_NUM
* @return void
*/
/*****************************************************************************/
static void W5300_SockRevHandle(uint8_t  uc_ChipNum, uint8_t uc_SockNum)
{
	volatile uint16_t *ptr = NULLS;
	uint16_t us_Sum = 0;
	uint16_t rx_size =  0;
	uint16_t us_Len = 0;
	uint16_t i = 0;
	uint8_t  uc_Tail = 0;
	uint16_t us_Data = 0;
	uint16_t us_Cnt = 0;
	uint8_t  uc_AlFlag = SYS_NO;
	/*如果SOCK接口已初始化，可进行数据读取*/
	if (gast_NetInfor[uc_ChipNum][uc_SockNum].uc_Init == SYS_YES)
	{	/*获取接收状态寄存器地址读取接收数据长度*/
		ptr=( volatile uint16_t*)SN_RX_RSR(uc_ChipNum, uc_SockNum);
				
		us_Sum=*ptr;
		ptr = ptr + 2;
		us_Sum=*ptr;
		/*如果长度满足最小长度进行解析数据区，一个数据区内可能有多个数据帧，读取退出条件为数据区数据全部读取完成，*/
		while( us_Sum >= 8)
		{	/*获取接收FIFO寄存器地址*/				
			ptr=( volatile uint16_t*)SN_RX_FIFOR(uc_ChipNum, uc_SockNum);
			uc_Tail = gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail;
			/*读取接收数据源地址*/	
			gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr = *ptr;
			gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr 
					= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].ui_Addr<<16)|*ptr;
			/*获取接收数据帧长度*/
			us_Data = *ptr;	/*此处为读取端口号，端口信息忽略不用*/
			rx_size = *ptr;
			/*16bit对齐*/
			if(rx_size&0x0001)
			{
				us_Len = (rx_size+1)/2; 
				uc_AlFlag = SYS_YES;
				//us_Len = us_Len -1;
			}
			else
			{
				us_Len = rx_size/2;
				uc_AlFlag = SYS_NO;
			}
			/*如果接收数据满足接收队列BUF长度要求进行数据读取，否则进行空读取操作*/
			if (rx_size < W300_REV_BUF_LEN)
			{	us_Cnt = 0;
				for(i=0; i<us_Len; i++)
				{	/*读取数据将16比特数据转化为8比特数据存储*/
					us_Data = *ptr ;
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].auc_Buf[us_Cnt] = us_Data>>8;
					us_Cnt++;
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].auc_Buf[us_Cnt] = us_Data&0xFF;
					us_Cnt++;
				}
				/*记录读取数据帧长度*/
				if (uc_AlFlag == SYS_YES)
				{
					us_Cnt = us_Cnt -1;
				}
				else
				{
					
				}
				gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.ast_Buf[uc_Tail].us_Len = us_Cnt;
				gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail 
						= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Tail + 1)%MAX_W5300_QUEUE_LEN;
				/*如果接收队列满，则删除老数据帧*/	
				if(gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Cnt>= MAX_W5300_QUEUE_LEN)
				{
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Hdr 
								= (gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Hdr + 1)%MAX_W5300_QUEUE_LEN;						
				}
				else
				{
					gast_NetInfor[uc_ChipNum][uc_SockNum].st_RevQueue.uc_Cnt++;
				}
			}
			else
			{	/*读取空操作*/
				for(i=0; i<us_Len; i++)
				{
					us_Data = *ptr ;
				}
			}
			
			us_Sum = us_Sum - rx_size - 8;
		}
		ptr=( volatile uint16_t*)SN_CR(uc_ChipNum, uc_SockNum);		/* Set RECV command */
		*ptr=SN_CR_RECV;
	}
}

/*****************************************************************************/
/**
* @brief  void W5300_INTHandle
*         网络芯片中断处理
* @param  IN uint8_t  uc_ChipNum：芯片号，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @return void
*/
/*****************************************************************************/
static  void W5300_INTHandle(uint8_t uc_ChipNum)
{
	volatile uint16_t*   puc_IRReg = NULLS;	
	volatile uint16_t*   puc_SockIRReg = NULLS;
	volatile uint16_t    us_IR = 0;
	volatile uint16_t    us_SockIR = 0;
	uint16_t    us_Mask = 0x01;
	uint8_t     uc_Cnt = 0;
	/*获取中断标志寄存器，读取中断标志信息*/
	puc_IRReg = ( volatile uint16_t*)IR(uc_ChipNum);
	us_IR = *puc_IRReg;
	/*扫描网络芯片所有SOCK口确定发生中断的SOCKET进行操作*/
	for(uc_Cnt = 0; uc_Cnt < W5300_SOCK_NUM; uc_Cnt++)
	{
		if (us_IR&us_Mask)
		{	/*读取SOCKET中断标志寄存器*/
			puc_SockIRReg = ( volatile uint16_t*)SN_IR(uc_ChipNum, uc_Cnt);
			us_SockIR = *puc_SockIRReg;
			/*如果是超时中断或发生中断执行发生处理*/
			if(us_SockIR&SOCKET_SENOK_TIMEROUT_BIT)
			{	/*清除中断标志位*/
				*puc_SockIRReg = SOCKET_SENOK_TIMEROUT_BIT;
					
				W5300_SockSendHandle(uc_ChipNum, uc_Cnt);
			}
			/*如果是接收中断执行接收处理*/
			if (us_SockIR&SOCKET_REV_INT_BIT)
			{
				W5300_SockRevHandle(uc_ChipNum, uc_Cnt);
				/*清除中断标志位*/
				*puc_SockIRReg = SOCKET_REV_INT_BIT;
			}
		}
		us_Mask = us_Mask<<1;
	}
}
/*****************************************************************************/
/**
* @brief  void W5300_INTHandle
*         网络芯片复位
* @param  IN uint8_t uc_Port：接口号，0-ETHERNET_MAX_NUM，对应不同的网络芯片
* @return void
*/
/*****************************************************************************/
static void  W5300_RST(IN uint8_t uc_Port)
{
	uint16_t us_Bit = 0;
	
	us_Bit = W5300_RST_BIT<<uc_Port;
	
	asm(" EALLOW");
	REG_GPIO_GPBMUX &= (~us_Bit);
	REG_GPIO_GPBDIR |= us_Bit;	
	asm(" EDIS");
	
	REG_GPIO_GPBDAT |= us_Bit;
	
	delay10Us(10);
	
	REG_GPIO_GPBDAT &= (~us_Bit);
	
	
	delay10Us(20);
	
	REG_GPIO_GPBDAT |= us_Bit;
}
#endif


