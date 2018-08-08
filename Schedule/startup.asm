*
* @file    startup.asm
* @brief   codestart section£¬Disables the watchdog timer
* Branch to code starting point 
* Modify List£º                            
* @author  jdsy
* @version 1.00
* @date    2011.09.02
* @history
*

WD_DISABLE	.set	1	

    .ref _c_int00

    .sect "codestart"

    .if WD_DISABLE == 1
        LB wd_disables       
    .else
        LB _c_int00        
    .endif

    .if WD_DISABLE == 1

	.text
wd_disables:
	EALLOW					
    MOVZ DP, #7029h>>6      
    MOV @7029h, #0068h      
	EDIS					
    LB _c_int00            

    .endif

	.end 

; end of file              