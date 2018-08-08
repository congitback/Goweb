/******************************************************************************/
/**
*@file    dev_Intrupts.h       
*@brief   中断模块寄存器数据结构定义                     
*@author  yangbm
*@version V1.0.0
*@date    2009.03.11
*@details v1.0.0  2009.03.11 yangbm create */
/******************************************************************************/
/*****************************************************************************/
/**@brief   macro                                                    	     */
/*****************************************************************************/
#define   EINT       asm(" clrc INTM")
#define   DINT       asm(" setc INTM")
#define   ERTM       asm(" clrc DBGM")
#define   DRTM       asm(" setc DBGM")

#define   EALLOW     asm(" EALLOW")
#define   EDIS       asm(" EDIS")
#define   ESTOP0     asm(" ESTOP0")
#define   XINT1CR    *((volatile uint16_t*)0x007070)
#define   VECT_XINT1  ((volatile PINT*)0x00000D46)  
#define   PIEACKX    *((volatile uint16_t*)0x000CE1)
#define   PIECTRL	 *((volatile uint16_t*)0x000CE0) 
#define   PIEIER1S   *((volatile uint16_t*)0x000CE2)
#define   PIEIFR1S   *((volatile uint16_t*)0x000CE3)
#define   ENPIES     *((volatile uint16_t*)0x000CE0)
#define   IN
#define   OUT
#define   INOUT
#define   PIEACK_GROUP1   0x0001;
#define   PIEACK_GROUP2   0x0002;
#define   PIEACK_GROUP3   0x0004;
#define   PIEACK_GROUP4   0x0008;
#define   PIEACK_GROUP5   0x0010;
#define   PIEACK_GROUP6   0x0020;
#define   PIEACK_GROUP7   0x0040;
#define   PIEACK_GROUP8   0x0080;
#define   PIEACK_GROUP9   0x0100;
#define   PIEACK_GROUP10  0x0200;
#define   PIEACK_GROUP11  0x0400;
#define   PIEACK_GROUP12  0x0800;
#define   M_INT1  0x0001
#define   M_INT2  0x0002
#define   M_INT3  0x0004
#define   M_INT4  0x0008
#define   M_INT5  0x0010
#define   M_INT6  0x0020
#define   M_INT7  0x0040
#define   M_INT8  0x0080
#define   M_INT9  0x0100
#define   M_INT10 0x0200
#define   M_INT11 0x0400
#define   M_INT12 0x0800
#define   M_INT13 0x1000
#define   M_INT14 0x2000

#define   MASK_0  0x00000001
#define   MASK_1  0x00000002
#define   MASK_2  0x00000004
#define   MASK_3  0x00000008
#define   MASK_4  0x00000010
#define   MASK_5  0x00000020
#define   MASK_6  0x00000040
#define   MASK_7  0x00000080
#define   MASK_8  0x00000100
#define   MASK_9 0x00000200
#define   MASK_10 0x00000400
#define   MASK_11 0x00000800
#define   MASK_12 0x00001000
#define   MASK_13 0x00002000
#define   MASK_14 0x00004000
#define   MASK_15 0x00008000
#define   MASK_16 0x00010000
#define   MASK_17 0x00020000
#define   MASK_18 0x00040000
#define   MASK_19 0x00080000
#define   MASK_20 0x00100000
#define   MASK_21 0x00200000
#define   MASK_22 0x00400000
#define   MASK_23 0x00800000
#define   MASK_24 0x01000000
#define   MASK_25 0x02000000
#define   MASK_26 0x04000000
#define   MASK_27 0x08000000
#define   MASK_28 0x10000000
#define   MASK_29 0x20000000
#define   MASK_30 0x40000000
#define   MASK_31 0x80000000
#define    IMS_INT_SOURCE    (*((volatile uint8_t*)0x2000L))					/*中断状态寄存器地址*/
#define    INTERRUPT_SRC_NUM         8											/*中断源数量*/
/*****************************************************************************/
/**@brief   中断处理函数类型                                                     	     */
/*****************************************************************************/
typedef  void(*FucVect)(uint8_t uc_Param);
/*****************************************************************************/
/**@brief   中断处理结构                                                    	     */
/*****************************************************************************/
typedef  struct  tagInterruptVector
{
	uint8_t  uc_Init;			/*初始化标志*/
	uint8_t  uc_Param;			/*中断处理函数输入参数*/
    FucVect  pFn_Vector;		/*中断处理函数*/	
}SInterruptVector;


/*****************************************************************************/
/**@brief   中断向量类型                                                     	     */
/*****************************************************************************/
typedef interrupt void(*PINT)(void);

extern cregister volatile unsigned int IFR;
extern cregister volatile unsigned int IER;

extern  void ModINT_Init(void);

extern  void ModINT_Rigster(IN uint8_t uc_Port,IN uint8_t uc_Param,IN FucVect pFn_Vect)  ;

extern void ModINT_PollHandle(void);
