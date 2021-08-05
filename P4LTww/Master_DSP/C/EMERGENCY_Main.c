/*-------------------------------------------------------------------------*
 * $Workfile: EMERGENCY_Main.c $
 * Part of	: ETL600 
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/EMERGENCY_Main.c $
 * 
 * 5     22.11.05 19:47 Chmibuh
 * IO_BUFFER_READ added
 * 
 * 4     22.11.05 16:02 Chmibuh
 * 
 * 3     22.11.05 15:59 Chmibuh
 * clean up
 * 
 * 2     21.07.05 12:06 Chmibuh
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 3     11.12.04 18:59 Chmibuh
 * 
 * 2     6.12.04 18:38 Chmibuh
 * 
 * 1     2.12.04 15:56 Chmibuh
 * initial
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <signal.h>
#include <def21161.h>
asm("#include <def21161.h>");

#include "basetype.h"
#include "master_dsp_main.h"
#include "constant.h"
#include "Timer.h"
#include "Init_Master_DSP.h"
#include "IO_HW_BUFFER.H"


#include "diag.h"
#include "bridge.h"
#include "dl_rs232.h"


#include "msgHandler.h"

#include "eprommed.h"
#include "tdm_int_types.h"


#include "SPI_functions.h"
#include "rtc_M41ST95.h"
#include "SPI_access.h"   



/* LOCAL CONSTANT DECLARATIONS ***********************************************/


/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/


/* LOCAL FUNCTIONS ***********************************************************/


/* IMPLEMENTATION ************************************************************/


#define STATE_READY_LED				1
#define STATE_WARNING_LED			2
#define STATE_LINK_LED				3
#define STATE_HW_LED				4
#define STATE_SYS_LED				5
#define STATE_P4LT_LED				6

void signal_emergencyLEDs(void)
{
	static unsigned long delayWait = 200;
	static unsigned long stateLED = STATE_READY_LED;

	if(delayWait-- == 0)
	{
		delayWait = 200;
		switch(stateLED)
		{
		case STATE_READY_LED:
			IO_BUFFER_WRITE &= ~(READY_LED);
			IO_BUFFER_WRITE |= (P4LT_AL_LED);
			stateLED++;
			break;
		case STATE_WARNING_LED:
			IO_BUFFER_WRITE &= ~(WARNING_LED);
			IO_BUFFER_WRITE |= (READY_LED);
			stateLED++;
			break;
		case STATE_LINK_LED:
			IO_BUFFER_WRITE &= ~(LINK_AL_LED);
			IO_BUFFER_WRITE |= (WARNING_LED);
			stateLED++;
			break;
		case STATE_HW_LED:
			IO_BUFFER_WRITE &= ~(HW_AL_LED_DSP);
			IO_BUFFER_WRITE |= (LINK_AL_LED);
			stateLED++;
			break;
		case STATE_SYS_LED:
			IO_BUFFER_WRITE &= ~(SYS_AL_LED);
			IO_BUFFER_WRITE |= (HW_AL_LED_DSP);
			stateLED++;
			break;
		case STATE_P4LT_LED:
			IO_BUFFER_WRITE &= ~(P4LT_AL_LED);
			IO_BUFFER_WRITE |= (SYS_AL_LED);		
			stateLED = STATE_READY_LED;
			break;
		default:
			stateLED = STATE_READY_LED;
			break;
		}
	}
}



void main(void)
{
	unsigned long testForReset;

	Init_DSP_Register();	// this must be the first function call!
	
	*(long*)(MSGR3) = 0;	// MSGR3 will be used for reset, see MAINCASE_4

	// Add HW-ID, Memory Check, ect.
	IO_BUFFER_READ = *(long*)IO_BUFFER_DSP;									// As from now read/mask HW-ID from IO_BUFFER_READ

	configuration_init(); 
	
	initialize_DSP();

	RestartTimer10k();
	IR_TimerCounter = 1;
	
	bridge_init();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
///////////////////////         Watchdog activated !!!!!!!!       /////////////////////////////////////////////////		
	transferRTC_Byte(WRITE_RTC_REG_WatchDog, 0x8A);	// init RTC for Watchdog 2 sec
	*(long*)(MSGR3) = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

	
    // main loop 
    while (1) 
	{
		static unsigned long	mainCaseCounter;

		// repetiton rate = 10.667 kHz
		while(IR_TimerCounter && IR_BlockCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		RestartTimer10k();
		IR_BlockCounter = 1;
		IR_TimerCounter = 1;

		{
			// make a 5.33kHz task
			static BOOL toggle = FALSE;

			if(toggle = !toggle)
			{
				// serve the HMI port
				ToDataLinkLayerRS232();           

				// serve the FT1.2 bridge
				bridge_activate(); 
			}
		}

		FromDataLinkLayerRS232();

		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 1.067 kHz

			case MAINCASE_0:
			
				// repetiton rate = 1.067 kHz	
				
				// ***************************************
				// ***         read IO-Buffer          ***
				// ***************************************
				IO_BUFFER_READ = *(long*)IO_BUFFER_DSP ;

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				// ***************************************
				// ***       write to IO-Buffer        ***
				// ***************************************
				*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2:

				eprom_activate(); // ran with 300Hz in ETL500
			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_3:

				signal_emergencyLEDs();

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
				// message RESET_BOARD will set MSGR3 to 1000. Because delayed messages are not supported by Emergency FW.
				testForReset = *(long*)(MSGR3);
				if(testForReset)
				{
					*(long*)(MSGR3) = --testForReset;	// decrement reset counter
					if(testForReset < 5)
					{
						while(1);						// Force a reset by stopping watchdog access
					}
				}

				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_5:
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_6:
							
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_7:
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_8:

				ApplicationLayer(); 				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_9:
					
				mainCaseCounter = 0;
				break;
			default:
				mainCaseCounter = 0;
				break;
		} // end switch(mainCaseCounter)
   } // end while(1)

} // end main
