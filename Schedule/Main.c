/******************************************************************************/
/**
*@file     main.c        
*@brief   ������                 
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
{	/*�忨��ʼ��*/
	initBoard();
	/*����ģ���ʼ��*/
	InitDriverMod();
	
	AppTaskStartInit();              /*Ӧ�ýӿڳ�ʼ�� 20170808*/
	/*�ж�ģ���ʼ��*/
	ModINT_Init();
	/*����ģ���ʼ��*/
	ExPortMod_Init();                /*�ڴ�س�ʼ�� 20170808*/
	/*���Э��ģ���ʼ��*/
	ProtoMod_Open();                /*���ĸ����� 20170808*/
	/*��ʱ��ģ���ʼ��*/
	initTimer0();			
	while(1)
	{ 	/*�������*/
		ScheduleForInput();
		/*�������*/
		ScheduleForOutput();
		/*Ӧ�õ���*/
		ScheduleForAPP();
	}
}     							       							
/*===========================================================================
  ȫ�����
=============================================================================*/
