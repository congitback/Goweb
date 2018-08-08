/******************************************************************************/
/**
*@file     main.c        
*@brief   主函数                 
*@author  yangbm
*@version V1.0.0
*@date    2011.11.01
*@details v1.0.0  2011.11.01 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief   include                                                    	     */
/*****************************************************************************/ 	
#include  <string.h>
#include  "board.h"
#include  "Driver.h"
#include  "Proto.h"
#include  "AppTask.h"
#include  "Schedule.h"
#include  "dev_Timer.h"
#include  "TL16C754.h"
#include  "W5300.h"
#include  "dev_Interrupt.h"
#include  "ExPort.h"

void main(void)
{	/*板卡初始化*/
	initBoard();
	/*驱动模块初始化*/
	InitDriverMod();
	
	AppTaskStartInit();              /*应用接口初始化 20170808*/
	/*中断模块初始化*/
	ModINT_Init();
	/*外设模块初始化*/
	ExPortMod_Init();                /*内存池初始化 20170808*/
	/*板间协议模块初始化*/
	ProtoMod_Open();                /*打开四个串口 20170808*/
	/*定时器模块初始化*/
	initTimer0();			
	while(1)
	{ 	/*输入调度*/
		ScheduleForInput();
		/*输出调度*/
		ScheduleForOutput();
		/*应用调度*/
		ScheduleForAPP();
	}
}     							       							
/*===========================================================================
  全部完毕
=============================================================================*/
