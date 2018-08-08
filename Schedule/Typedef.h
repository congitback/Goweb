#ifndef __TYPEDEF_H_
#define __TYPEDEF_H_
/******************************************************************************/
/* FileName£º  Typedef.h                                                      */
/* Description£º define the variant define                                    */
/* Platform£º  none                                                           */
/* Copyright£ºCopyright 1993-2009 Beijing. RichSun. Co.. Ltd.				  */
/*            All rights reserved											  */
/******************************************************************************/
/* Version£ºv1.0.0                Creater£ºyaoxw    Data:2011-9-20            */
/* History:																      */
/*	Sep 20, yaoxw	created													  */
/*                                                                            */
/******************************************************************************/
#include  "Config.h"
typedef    char char_t;
typedef    signed char bool_t;
typedef    signed char int8_t;
typedef    signed int int16_t;  
typedef    signed long int32_t;  
typedef    signed long long int64_t;  
typedef    unsigned int uint8_t;  
typedef    unsigned int uint16_t;
typedef    unsigned long uint32_t;
typedef    unsigned long long uint64_t;

typedef    float  float32_t;
typedef    double float64_t;
typedef    long  double float128_t;
typedef    uint8_t *		      LPBYTE;
#define	   IN		 	
#define	   OUT				 
#define	   INOUT
#define	   SYS_ERR		 0x55
#define    SYS_OK		 0xAA
#define    SYS_YES      0xAA
#define    SYS_NO       0x55
#define    TRUE		 1
#define    FALSE		 0
#define    NULLS ((void*)0)
#define    LOCAL static 
#define    LOBYTE(wData)  ((wData)&0xFFU)
#define	   HIBYTE(wData)  ((((uint16_t)(wData))>>8)&0xFFU)
#define    OS_ENTER_CRITICAL() asm(" setc INTM")
#define    OS_EXIT_CRITICAL()  asm(" clrc INTM")

extern void SOS_Assertion_Failed (char_t  * const File_Name, const uint32_t  ui_Line, uint32_t ui_Arg);

#define SOS_Assert(expr)                            \
if ((expr))                                       \
{                                                    \
    SOS_Assertion_Failed ((char *) __FILE__, (uint32_t)__LINE__, 0); \
}
#endif /*__TYPEDEF_H_*/
