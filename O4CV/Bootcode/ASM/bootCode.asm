/*-------------------------------------------------------------------------*
 * $Workfile	: bootCode.asm  $
 * Part of      : ETL600 / O4CV
 * Language     : ASM
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Boot code which is called by the boot loader
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/Bootcode/ASM/bootCode.asm $
 * 
 * 6     11-04-08 15:29 Chalnot
 * Flash times set to max (default values)
 * SDRAM refresh rate corrected
 * 
 * 3     08-04-22 17:24 Chalnot
 * Reset bootTries if Em Fw is loaded.
 * Watchdog time set to 1s.
 * 
 * 2     07-11-07 9:42 Chalnot
 * Switch alarm led on.
 * 
 * 1     07-09-26 16:45 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/*-----------------------ASSEMBLER DIRECTIVES------------------------------*/

/*-----------------------GLOBAL CONSTANTS----------------------------------*/

#include <defBF538.h>

#define EM_FW_ADDR 0x20010000
#define APPL_ADDR 0x20200000
#define COUNTER_ADDR 0x203F0000

#define UNSUCCESSFUL_FLASH_ACCESS 5

/*-----------------------IMPORTED DATA-------------------------------------*/                       

/*-----------------------EXPORTED DATA-------------------------------------*/

/*-----------------------LOCAL DATA DEFINITIONS DM-------------------------*/

/*-----------------------LOCAL DATA DEFINITIONS PM-------------------------*/
                   

/********************************************************************************
* Function			:	bootCode                  		
*                                                                               
* Calling Registers :	R1		Value of the counter                	
*		        		R2		Several uses
*						R3		Several uses
*						R4		Pointer to application firmware
*						R5		Counter for unsuccessful flash access
*						R6		Temporary value
*						P0 		Pointer to register	
*						P1 		Loop value
*	      	
* Result Registers  : 	R0		Pointer to code to load   			
*		        		    								
* Global Variables	:   None                                             	
*						      			        
*                       				                        
* Purpose: Initializes the PLL and the EBIU and sets the external pointer to the
* application or the emergency firmware dependent on the variable bootTries.
* bootTries is read from the flash and incremented. If it is larger than 3 the
* external pointer is set to the emergency firmware and bootTries is set 0. 
* All registers are restored except R0.                                                                      
* 			                	
*********************************************************************************/ 


.SECTION program;


	//Store Register onto stack
	[--SP] = ASTAT;
	[--SP] = RETS;
	[--SP] = (r7:0);
	[--SP] = (p5:0);
		
						
	// Init PLL	
	P0.H = HI(PLL_DIV);
	P0.L = LO(PLL_DIV);
	R2 = 0x0004;
	W[P0] = R2;
	SSYNC;

	P0.H = HI(PLL_CTL);
	P0.L = LO(PLL_CTL);
	R2 = 0x2800;
	CLI R3;
	W[P0] = R2;
	IDLE;
	STI R3;
	SSYNC;
	
		
	// Init EBIU for SDRAM	
	P0.H = HI(EBIU_SDRRC);
	P0.L = LO(EBIU_SDRRC);
	R2 = 0x0798;
	W[P0] = R2;
	SSYNC;
		
	P0.H = HI(EBIU_SDBCTL);
	P0.L = LO(EBIU_SDBCTL);
	R2 = 0x0011;
	W[P0] = R2;
	SSYNC;
		
	P0.L = LO(EBIU_SDGCTL);
	P0.H = HI(EBIU_SDGCTL);
	R2.L = 0x998d;
	R2.H = 0x0091;
	[P0] = R2;	
	SSYNC;

	
	// Init EBIU for Flash
	P0.H = HI(EBIU_AMGCTL);
	P0.L = LO(EBIU_AMGCTL);
	R2.L = 0x00ff;	
	W[P0] = R2;
	SSYNC;
			
	P0.H = HI(EBIU_AMBCTL0);
	P0.L = LO(EBIU_AMBCTL0);
	R2.H = 0xFFC2;
	R2.L = 0xFFC2;	
	[P0] = R2;
	SSYNC;
	
	P0.H = HI(EBIU_AMBCTL1);
	P0.L = LO(EBIU_AMBCTL1);
	R2.H = 0xFFC2;
	R2.L = 0xFFC2;	
	[P0] = R2;
	SSYNC;

	
	// Set alarm led on
	P0.H = HI(UART0_LCR);
	P0.L = LO(UART0_LCR);
	R2 = W[P0] (Z);	
	R3 = 0x0040;
	R2 = R2 | R3;	
	W[P0] = R2;	
	SSYNC;	

#ifdef RDY_LED_ON	

	// PC0,PC1,PC4-PC9 - GPIO
	P0.H = HI(PORTCIO_FER);
	P0.L = LO(PORTCIO_FER);
	R2 = 0x03F3;
	W[P0] = R2;	
	SSYNC;	

	// PC0,PC1,PC4-PC9 - output
	P0.H = HI(PORTCIO_DIR);
	P0.L = LO(PORTCIO_DIR);
	R2 = 0x03F3;
	W[P0] = R2;	
	SSYNC;	

	// Input buffer Disable 
	P0.H = HI(PORTCIO_INEN);
	P0.L = LO(PORTCIO_INEN);
	R2 = 0x0000;
	W[P0] = R2;
	SSYNC;

	// Set rdy led on
	P0.H = HI(PORTCIO);
	P0.L = LO(PORTCIO);
	R2 = 0x01A0;
	W[P0] = R2;	
	SSYNC;
	
#endif

	// Wait for 1s
	P1.H = 0x1DCD;
	P1.L = 0x6500;
	LSETUP(delay_loop1, delay_loop1) LC0=P1;
	delay_loop1: nop;

	// Increment counter
	P0.H = HI(COUNTER_ADDR);		
	P0.L = LO(COUNTER_ADDR);
	R1 = W[P0] (Z);
	R2 = 0x0001;			
	R6 = R1 + R2;	
	R1 = R6;
	
	// Set ext pointer to appl fw
	R0.H = HI(APPL_ADDR);
	R0.L = LO(APPL_ADDR);
	R4.H = HI(EM_FW_ADDR);
	R4.L = LO(EM_FW_ADDR);

	// If counter > 3 then set ext pointer to em fw
	R2 = ~0x0003;					 
	R3 = R6 & R2;
	CC = R3 == 0x0000;
	IF CC JUMP APPL_FW;
	R0 = R4;
	R1 = 0x0000;	

APPL_FW:
	
	// Reset unsuccessful flash access counter
	R5 = UNSUCCESSFUL_FLASH_ACCESS; 

ERASE_BEGIN:
	
	// Erase counter in flash
	P0.H = 0x2000;
	P0.L = 0x0000;
	R2 = 0x00AA;
	W[P0+0x0AAA] = R2;
	R2 = 0x0055;
	W[P0+0x0554] = R2;
	R2 = 0x0080;
	W[P0+0x0AAA] = R2;
	R2 = 0x00AA;
	W[P0+0x0AAA] = R2;
	R2 = 0x0055;
	W[P0+0x0554] = R2;
	P0.H = HI(COUNTER_ADDR);	
	P0.L = LO(COUNTER_ADDR);
	R2 = 0x0030;
	W[P0] = R2;

	// Wait for 1us
	P1.H = 0x0000;
	P1.L = 0x0190;
	LSETUP(delay_loop2, delay_loop2) LC0=P1;
	delay_loop2: nop;
		
	// Wait for Status
ERASE_STATUS:
	
	R2 = W[P0] (Z);					// Test if bit 7 (Polling bit) is 1
	CC = BITTST(R2,7);
	IF CC JUMP ERASE_SUCCESSFUL; 
	CC = BITTST(R2,5);				// Test if bit 5 (Exceeded Timing limit) is set
	IF !CC JUMP ERASE_STATUS; 
	R2 = W[P0] (Z);					// Test if bit 7 (Polling bit) is 1
	CC = BITTST(R2,7);
	IF CC JUMP ERASE_SUCCESSFUL; 
HW_ERROR:
	R3 = 0;
	CC = R5 == R3;	
	IF CC JUMP HW_ERROR;			// Endless loop if 5 unsuccesful tries
	R3 = 1;
	R5 = R5 - R3;
	JUMP ERASE_BEGIN; 				// Try again	
	
ERASE_SUCCESSFUL:

	// Reset unsuccessful flash access counter
	R5 = UNSUCCESSFUL_FLASH_ACCESS; 

WRITE_BEGIN:
	
	// Store counter to flash
	P0.H = 0x2000;
	P0.L = 0x0000;
	R2 = 0x00AA;
	W[P0+0x0AAA] = R2;
	R2 = 0x0055;
	W[P0+0x0554] = R2;
	R2 = 0x00A0;
	W[P0+0x0AAA] = R2;
	P0.H = HI(COUNTER_ADDR);		
	P0.L = LO(COUNTER_ADDR);
	W[P0] = R1;
	
	// Wait for 1us
	P1.H = 0x0000;
	P1.L = 0x0190;
	LSETUP(delay_loop3, delay_loop3) LC0=P1;
	delay_loop3: nop;
	
	// Wait for Status
WRITE_STATUS:

	R2 = W[P0] (Z);
	R3 = R2 ^ R1;					// Test if bit 7 (Polling bit) is correct
	CC = BITTST(R3,7);
	IF !CC JUMP WRITE_SUCCESSFUL; 
	CC = BITTST(R2,5);				// Test if bit 5 (Exceeded Timing limit) is set
	IF !CC JUMP WRITE_STATUS;
	R2 = W[P0] (Z);
	R3 = R2 ^ R1;					// Test if bit 7 (Polling bit) is correct
	CC = BITTST(R3,7);
	IF !CC JUMP WRITE_SUCCESSFUL; 
HW_ERROR2:	
	R3 = 0;
	CC = R5 == R3;	
	IF CC JUMP HW_ERROR2;			// Endless loop if 5 unsuccesful tries
	R3 = 1;
	R5 = R5 - R3;
	JUMP WRITE_BEGIN; 				// Try again

WRITE_SUCCESSFUL:

	// Start watchdog with a time of 1s		
	P0.H = HI(WDOG_CNT);
	P0.L = LO(WDOG_CNT);
	R2.H = 0x0773;
	R2.L = 0x5940;	
	[P0] = R2;
	SSYNC;
	
	P0.H = HI(WDOG_CTL);
	P0.L = LO(WDOG_CTL);
	R2 = 0x0000;	
	W[P0] = R2;	
	SSYNC;
	

	// Restore register from stack except R0
	(p5:0) = [SP++];  		
	(r7:1) = [SP++];		//----->DO NOT RESTORE R0<-------
	RETS = [SP++];			//Modify SP by one for R0 case
	RETS = [SP++];			//Pop off the real value of RETS
	ASTAT = [SP++];

	RTS;

