
MEMORY
{
    PAGE 0:       
        H0SARAM1    : origin = 0x3F8000, length = 0x000500    
        FLASH_AB    : origin = 0x3EE000, length = 0x009F80     
        FLASH_CD    : origin = 0x3EC00C, length = 0x001990         
        BEGIN_FLASH : origin = 0x3F7FF6, length = 0x000002     
        APPSTART    : origin = 0x3EC000, length = 0x00000A
        RESET       : origin = 0x3FFFC0, length = 0x000002 
    PAGE 1 :   
        M0SARAM     : origin = 0x000000, length = 0x000800       
        L0SARAM     : origin = 0x008000, length = 0x002000    
        H0SARAM     : origin = 0x3F8500, length = 0x001500 
        SRAM6       : origin =0x80000 ,  length = 0x40000
	    DRAMBUFRAM  : origin = 0x100000, length = 0x0fAA
}

SECTIONS
{
	.text         :> FLASH_AB,         PAGE = 0
	.QText      : LOAD = FLASH_AB,     PAGE = 0  
				  RUN = H0SARAM, PAGE = 1 
			 	  LOAD_START(_QText_LoadStart),
			 	  LOAD_END(_QText_LoadEnd),
             	  RUN_START(_QText_RunStart)
    {  	
		Crc.obj(.text)
        Queue.obj(.text)
        AppTask.obj(.text)
       /* TL16C754.obj(.text)*/
        ExPort.obj(.text)
        Proto.obj(.text)
    }
    .QBss             :> M0SARAM,          PAGE = 1
    {
		Crc.obj(.ebss)       
    } 
   	.reset            :> RESET,            PAGE = 0, TYPE = DSECT 
  	.cinit            :> FLASH_CD,         PAGE = 0
   	.const            :> FLASH_CD,         PAGE = 0
   	.econst           :> FLASH_CD,         PAGE = 0      
   	.pinit            :> FLASH_CD,         PAGE = 0
   	.switch           :> FLASH_CD,         PAGE = 0
    SwVerSec          :> APPSTART,         PAGE = 0
   	.bss              :> SRAM6,            PAGE = 1
   	.ebss             :> SRAM6,            PAGE = 1              
   	.cio              :> H0SARAM ,         PAGE = 1
   	.stack            :> L0SARAM,          PAGE = 1
   	.sysmem           :> H0SARAM1,          PAGE = 0
   	.esysmem          :> H0SARAM1,          PAGE = 0
    sectorDram        :> DRAMBUFRAM ,      PAGE = 1 
    codestart         :> BEGIN_FLASH,      PAGE = 0           
    secureRamFuncs    : LOAD = FLASH_CD,   PAGE = 0                
                      RUN = H0SARAM1,      PAGE = 0 
                      LOAD_START(_Flash_Funcs_LoadStart),
                      LOAD_END(_Flash_Funcs_LoadEnd),
                      RUN_START(_Flash_Funcs_RunStart)
}
