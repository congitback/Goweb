/******************************************************************************/
/**
*@file     Schedule.c        
*@brief   调度处理                 
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#include "Typedef.h"
#include  "Schedule.h"
#include  "dev_Timer.h"
#include  "Driver.h"
#include  "board.h"
#include  "ExPort.h"
#include  "Proto.h"
#include  "AppTask.h" 
#include  "dev_Interrupt.h"

/*****************************************************************************/
/**
* @brief  void ScheduleForInput
*         输入调度
* @param  void
* @return Vvoid
*/
/*****************************************************************************/
void   ScheduleForInput(void)
{	/*驱动层调度*/
	dev_DrvRevSchedule();        /*判断串口的通信状态，以自身能否连续接受作为判断依据 20170808*/    /*方案都不需要修改 20170808*/
	/*外设模块接收处理*/
	ExPortMod_RevHandle();      /*从网卡接受数据存入固定内存中 20170808*/
	/*板间协议模块接收处理*/
	ProtoMod_RevHandle();      /*串口循环调度接受，数据按照帧格式存储到gast_ProtoFrame中 20170808*/
	
	ModINT_PollHandle();       /*中断调用  20170808*/
}

/*****************************************************************************/
/**
* @brief  void ScheduleForOutput
*         输出调度
* @param  void
* @return Vvoid
*/
/*****************************************************************************/
void  ScheduleForOutput(void)
{
	
}
/*****************************************************************************/
/**
* @brief  void ScheduleForAPP
*         应用调度
* @param  void
* @return Vvoid
*/
/*****************************************************************************/
void  ScheduleForAPP(void)
{	/*运行灯闪操作*/
	FlashRunLed();
	/*应用业务执行*/
	AppTaskRunHandle();
}
