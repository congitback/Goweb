/******************************************************************************/
/**
*@file    dev_Xintf.c     
*@brief   外部总线控制                   
*@author  yangbm
*@version V1.0.0
*@date    2009.8.24
*@details v1.0.0 2009.8.24 yangbm create
*/
/******************************************************************************/
#include  "Typedef.h"
#include  "dev_Xintf.h"


/*****************************************************************************/
/**
* @brief  void initXintfRegs
*         外部数据总线初始化
* @param  void
* @return void
*/
/*****************************************************************************/
void initXintfRegs(void)
{
	/*bit 31-19                 0's:     reserved
	  bit 18-16(XTIMCLK)        000:     select the fundamental clock for the timing of lead, active and trail switching operations XTIMCLK = SYSCLKOUT/1
	  bit 15-12                 0's:     reserved
	  bit 11(HOLDAS)           read:     reflects the current state of the XHOLDA output signal
	  bit 10(HOLDS)            read:     reflects the current state of the XHOLD input signal
	  bit 9(HOLD)              read:     grants a request to an external device that drives the XHOLD input signal and the XHOLDA output signal
	  bit 8(MP/MC)                0:     Microcomputer state (XINTF ZONE 7 disabled, Boot ROM enabled).
	  bit 7-6(WLEVEL)          read:     The current number of writes buffered are detectable
	  bit 5-4                    00:     reserved
	  bit 3(CLKOFF)               0:     XCLKOUT is enabled
	  bit 2(CLKMODE)              0:     XCLKOUT is equal to XTIMCLK
	  bit 1-0(Write Buffer Depth)00:     No write buffering. The CPU will be stalled until the write completes on the XINTF
	*/  
	REG_XINTF_XINTCNF2 = 0X10000;
	/*
	bit 31-23                 0's:     reserved
	bit 22(X2TIMING)            1:     the scaling factor of the XRDLEAD, XRDACTIVE, XRDTRAIL, XWRLEAD, XWRACTIVE, and XWRTRAIL values for the zone is 2:1
	bit 21-18                 0's:     reserved
	bit 17-16(XSIZE)           11:     These two bits must always be written to as 1, 1.
	bit 15(READYMODE)           1:     XREADY input is asynchronous for the zone,This bit is ignored if XREADY is not sampled (USEREADY = 0).
	bit 14(USEREADY)            0:     The XREADY signal is ignored when accesses are made to the zone.
	bit 13-12(XRDLEAD)         11:     the read cycle lead period  is 6 XTIMCLK cycles.
	bit 11-9(XRDACTIVE)       111:     the read cycle active wait-state period is 14 XTIMCLK cycles.
	bit 8-7(XRDTRAIL)          11:     the read cycle trail period is 6 XTIMCLK cycles.
	bit 6-5(XWRLEAD)           11:     the write cycle lead period is 6 XTIMCLK cycles.
	bit 4-2(XWRACTIVE)        111:     the write cycle active wait-state period is 14 XTIMCLK cycles
	bit 1-0(XWRTRAIL)          11:     the write cycle trail period is 6 XTIMCLK cycles
	*/
   	REG_XINTF_XTIMING6 = 0x43BDFB;

	//REG_XINTF_XBANK = 0x0000;

    REG_XINTF_XTIMING2 = 0x394A9;

    REG_XINTF_XTIMING0 = 0x43BFFF;//0x43BDFB;
}



