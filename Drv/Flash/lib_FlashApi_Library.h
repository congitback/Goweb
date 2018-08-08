/* @file    A802_FlashApi_Library.h
 * @brief   FlahApi_Library Í·ÎÄ¼þ
 * @writer  yangbm                        
 * @version 1.00
 * @date    
 * @history 
*/

#ifndef FLASHAPI_LIBRARY
#define FLASHAPI_LIBRARY


#ifdef __cplusplus
extern "C" {
#endif


#define STATUS_SUCCESS                        0   
#define STATUS_FAIL                          0xAA

#define STATUS_FAIL_CSM_LOCKED               10

#define STATUS_FAIL_REVID_INVALID            11
    
#define STATUS_FAIL_ADDR_INVALID             12
 
#define STATUS_FAIL_INCORRECT_PARTID         13
 
#define STATUS_FAIL_API_SILICON_MISMATCH     14

#define STATUS_FAIL_NO_SECTOR_SPECIFIED      20
#define STATUS_FAIL_PRECONDITION             21
#define STATUS_FAIL_ERASE                    22
#define STATUS_FAIL_COMPACT                  23
#define STATUS_FAIL_PRECOMPACT               24

#define STATUS_FAIL_PROGRAM                  30
#define STATUS_FAIL_ZERO_BIT_ERROR           31

#define STATUS_FAIL_VERIFY                   40

#define STATUS_BUSY                999    



#define FLASH_ADDR_PROGM  0x3EC000
#define FLASH_RANGE_PROGM 40959L  
#define  BLOCK_SIZ  FLASH_RANGE_PROGM + 1L 

#define SECTORA   (uint16_t)0x0001
#define SECTORB   (uint16_t)0x0002
#define SECTORC   (uint16_t)0x0004
#define SECTORD   (uint16_t)0x0008
#define SECTORE   (uint16_t)0x0010
#define SECTORF   (uint16_t)0x0020
#define SECTORG   (uint16_t)0x0040
#define SECTORH   (uint16_t)0x0080
#define SECTORI   (uint16_t)0x0100
#define SECTORJ   (uint16_t)0x0200

#define SECTOR_PROGRAM  (SECTORJ)


#define SECTOR_F2811  (SECTORA|SECTORB|SECTORC|\
                       SECTORD|SECTORE|SECTORF|\
                       SECTORG|SECTORH|SECTORI|\
                       SECTORJ)


typedef struct
{
    uint16_t  FirstFailAddr;
    uint16_t  ExpectedData;
    uint16_t  ActualData;
}FLASH_ST;

#define Flash_Erase(a,b)        Flash2812_Erase(a,b)
#define Flash_Program(a,b,c,d)  Flash2812_Program(a,b,c,d)
#define Flash_Verify(a,b,c,d)   Flash2812_Verify(a,b,c,d)
#define Flash_ToggleTest(a,b,c) Flash2812_ToggleTest(a,b,c)
#define Flash_DepRecover()      Flash2812_DepRecover()
#define Flash_APIVersion()      Flash2812_APIVersion()
#define Flash_APIVersionHex()   Flash2812_APIVersionHex()
//

//
extern uint16_t  Flash_Erase(IN uint16_t SectorMask, IN FLASH_ST *FEraseStat);

//
extern uint16_t  Flash_Program(IN volatile uint16_t *FlashAddr, IN volatile uint16_t *BufAddr, IN uint32_t Length, IN FLASH_ST *FProgStatus);

//
extern uint16_t  Flash_Verify(IN volatile uint16_t *StartAddr, IN volatile uint16_t *BufAddr, IN uint32_t Length, IN FLASH_ST *FVerifyStat);

//
extern void  Flash_ToggleTest(IN volatile uint16_t *MuxReg,IN volatile uint16_t *ToggleReg, uint16_t Mask);

//
extern uint16_t  Flash_DepRecover();

//
extern uint16_t  Flash_APIVersionHex();

//
extern float Flash_APIVersion();

//
extern uint32_t Flash_CPUScaleFactor;

//
extern void (*Flash_CallbackPtr) ();

//

//


extern uint16_t Flash28_API_LoadStart;
extern uint16_t Flash28_API_LoadEnd;
extern uint16_t Flash28_API_RunStart;

extern FLASH_ST EraseStatus;
extern FLASH_ST ProgStatus;
extern FLASH_ST VerifyStatus; 

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif // -- end FLASH2812_API_LIBRARY_H 

// --------- END OF FILE ----------------------------------

