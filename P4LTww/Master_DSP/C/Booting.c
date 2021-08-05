/*-------------------------------------------------------------------------*
 * $Workfile: Booting.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/Booting.c $
 * 
 * 18    20.10.05 17:08 Chmibuh
 * comment corrected
 * 
 * 17    5.10.05 17:47 Chchlee
 * Booting: fit into template
 * 
 * 16    28.09.05 14:46 Chchlee
 * isInDebugMode() moved to IO_BUFFER.H
 * 
 * 15    21.07.05 10:00 Chchlee
 * DEBUG_FLAG_MASK moved to IO_HW_BUFFER.H
 * 
 * 14    29.06.05 15:20 Chmibuh
 * 
 * 13    13.06.05 20:28 Chmibuh
 * start-up LED sequence changed
 * 
 * 12    6.06.05 14:50 Chmibuh
 * 
 * 11    3.06.05 10:31 Chmibuh
 * Serie Number exchange
 * 
 * 10    20.05.05 15:42 Chmibuh
 * 
 * 9     19.05.05 21:14 Chmibuh
 * Atmel communication added
 * 
 * 8     19.05.05 15:27 Chmibuh
 * 
 * 7     18.05.05 15:57 Chmibuh
 * order of slave DSP booting changed - NSK DSP first
 * 
 * 6     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 5     12.05.05 18:37 Chmibuh
 * check UC DSP added
 * 
 * 4     15.02.05 12:05 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem) 
 * 
 * 3     27.01.05 13:07 Chmibuh
 * Comment changed Debug Flag is now Flag 7
 * 
 * 2     20.01.05 16:01 Chmibuh
 * Adapt DEBUG_FLAG to Prototype 2 HW
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 11    19.11.04 15:42 Maag01
 * Waiting time in checkSlaveDSPforCorrectBoot() for FE DSP increased
 * 
 * 10    3.11.04 9:05 Buhl01
 * 
 * 9     15.09.04 10:52 Buhl01
 * 
 * 8     27.08.04 14:57 Buhl01
 * Watchdog activation added - set to 2 seconds
 * 
 * 7     20.08.04 15:49 Buhl01
 * 
 * 6     19.08.04 17:04 Buhl01
 * shortened dummy mem check EXIT_WAIT
 * 
 * 5     18.08.04 13:24 Buhl01
 * 
 * 4     13.08.04 12:03 Buhl01
 * 
 * 3     2.08.04 19:09 Buhl01
 * 
 * 2     2.08.04 18:55 Buhl01
 * 
 * 1     2.08.04 16:52 Buhl01
 * initial
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "spi_dsp.h"
#include "rtc_M41ST95.h"
#include "SPI_access.h"   // for reestablishSPI
#include "IO_HW_BUFFER.H"
#include "MemoryCheck.h"	// for CRC check result of Master DSP itself
#include "status.h"
#include "dsp_micro_common.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define KERNEL_SIZE_IN_WORD32 		384		//256 Instr. * 1.5
#define FINAL_INIT_LENGTH			386   	//256 Instr. * 1.5 + 2 Init Words

#define INSTR_48_MULTI				3

//ADSP-21161 Loader TAGs
#define FINAL_INIT          		0x0000
#define ZERO_DM16           		0x0001
#define ZERO_DM32           		0x0002
#define ZERO_DM40           		0x0003
#define INIT_DM16           		0x0004
#define INIT_DM32           		0x0005
#define INIT_DM40           		0x0006
#define ZERO_PM16           		0x0007
#define ZERO_PM32           		0x0008
#define ZERO_PM40           		0x0009
#define ZERO_PM48           		0x000A
#define INIT_PM16           		0x000B
#define INIT_PM32           		0x000C
#define INIT_PM40           		0x000D
#define INIT_PM48           		0x000E
#define ZERO_DM64           		0x000F
#define INIT_DM64           		0x0010
#define ZERO_PM64           		0x0011
#define INIT_PM64           		0x0012
#define INIT_PM8_EXT           		0x0013
#define INIT_PM16_EXT           	0x0014
#define INIT_PM32_EXT           	0x0015
#define INIT_PM48_EXT           	0x0016
#define ZERO_PM8_EXT           		0x0017
#define ZERO_PM16_EXT           	0x0018
#define ZERO_PM32_EXT           	0x0019
#define ZERO_PM48_EXT           	0x001A

#define TAG_OFFSET_HEADER			2
#define TAG_HEADER_SIZE				3

#define MACIGWORD_FOR_BOOT_CHECK		0x12345678

#define EXIT_WAIT_FOR_NSD_DSP		20			
#define EXIT_WAIT_FOR_AFE_DSP		20
#define EXIT_WAIT_FOR_NSK_DSP		40
#define EXIT_WAIT_FOR_OFDM_DSP		40
#define EXIT_WAIT_FOR_UC_DSP		20

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/

#ifdef DO_NOT_BOOT_THE_DSPs		// this is defined with -noSlaveBoot switch via Tools menu and batch file 
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_NSD_DSP[100];    //Dummy arrays
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_OFDM_DSP[100];
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_NSK_DSP[100];
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_AFE_DSP[100];
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_UC_DSP[100];
#else
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_AFE_DSP[] = { 
		#include "AFE_SlaveSPI32include.ldr" 
		};
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_NSD_DSP[] = { 
		#include "NSD_SlaveSPI32include.ldr" 
		};
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_NSK_DSP[] = { 
		#include "NSK_SlaveSPI32include.ldr" 
		};
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_OFDM_DSP[] = { 
		#include "OFDM_SlaveSPI32include.ldr" 
		};
	const unsigned long dm SEGMENT_EXT_SLAVE_CODE loader_file_UC_DSP[] = { 
		#include "UC_SlaveSPI32include.ldr" 
		};
#endif  //DO_NOT_BOOT_THE_DSPs


/* LOCAL FUNCTIONS ***********************************************************/

static void SEGMENT_EXT_CODE wait_for_Spi(unsigned long setWait);
static void SEGMENT_EXT_CODE bootSlaveDSP(const unsigned long *source_ptr, unsigned long loaderFileSize, unsigned long whichSlave);
static unsigned long SEGMENT_EXT_CODE checkSlaveDSPforCorrectBoot(unsigned long whichSlave, unsigned long whichLED, unsigned long setExitWait);

/* IMPLEMENTATION ************************************************************/

static void SEGMENT_EXT_CODE wait_for_Spi(unsigned long setWait)
{
	unsigned long tempWait;

	for(tempWait = setWait;tempWait;tempWait--)
	{
		asm("nop;");
		asm("nop;");
	}
}


static void SEGMENT_EXT_CODE bootSlaveDSP(const unsigned long *source_ptr, unsigned long loaderFileSize, unsigned long whichSlave)
{	
	//Data Block header variables
	unsigned long dataWordCount, dataBlockTAG, spi_settings, emergencyLimit, temp, start_once;
	const unsigned long *local_source_ptr = source_ptr;
	
	unsigned long tempWait = 0;
	
	spi_settings = SPI_SETTINGS_FOR_DSP_BOOT; 	// Perpare SPI REG
	
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	//Transfer Bootkernel in one shot
	*(long *)SPITX = *(local_source_ptr++);		// load first two Data in FIFO
	*(long *)SPITX = *(local_source_ptr++);
	

#ifdef P4LW_PTTY2		
	*(long*)(IOFLAG) &= whichSlave;				// select Slave
	*(long *)SPICTL = spi_settings;				// start transfer
#else
	*(long *)SPICTL = (spi_settings | whichSlave);				// start transfer
#endif
	
	dataWordCount = KERNEL_SIZE_IN_WORD32 - 2;	// two data are already in FIFO  !!!!!!!!!!!!!!!!!!!!!!!!
    while(dataWordCount--)
    {    
		*(long *)SPITX = *(local_source_ptr++);

		while(!((*(long *)SPISTAT & (RXS0))))	// wait for data shifted in
		{
			asm("nop;");
		}    
		temp = *(long *)SPIRX;					// get dummy data from recveive reg - don't use
    } 
    temp = *(long *)SPIRX;						// get dummy data from recveive reg - don't use
	while(!((*(long *)SPISTAT & (RXS0))))		// wait for data shifted in
	{
		asm("nop;");
	}    
	temp = *(long *)SPIRX;						// get dummy data from recveive reg - don't use

	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	*(long*)(IOFLAG) |= DESELECT_SPI_SLAVE;		// deselect Slave (uP) P4LT	
// End of Kernel
///////////////////////////////////////////
// Start of data
	for(tempWait = 10000;tempWait;tempWait--)	// short break, DSP restarts
	{
		asm("nop;");
		asm("nop;");
	}

	start_once = 1;
	emergencyLimit = (loaderFileSize + 10) - KERNEL_SIZE_IN_WORD32;
	do
	{
		dataWordCount = *(local_source_ptr);
		dataBlockTAG  = *(local_source_ptr + TAG_OFFSET_HEADER);
		if(dataWordCount == 0)
		{
			dataBlockTAG = FINAL_INIT;
		}
		// load first two Data in FIFO
		*(long *)SPITX = *(local_source_ptr++);		// here always 			dataWordCount
		*(long *)SPITX = *(local_source_ptr++);		//						dataWordAdress
		if(start_once)
		{

#ifdef P4LW_PTTY2			
			*(long*)(IOFLAG) &= whichSlave;			// select Slave
			*(long *)SPICTL = spi_settings;			// start transfer
#else
			*(long *)SPICTL = (spi_settings | whichSlave);			// start transfer		
#endif
			
			start_once = 0;
		}
		
		*(long *)SPITX = *(local_source_ptr++);		// Header End			dataBlockTAG

		switch(dataBlockTAG)
		{
			case (FINAL_INIT):
				emergencyLimit = 0;
				dataWordCount = FINAL_INIT_LENGTH;
				break;
			case (ZERO_DM16): case (ZERO_DM32):	 case (ZERO_DM40): case (ZERO_PM16): case (ZERO_PM32):
			 case (ZERO_PM40): case (ZERO_PM48): case (ZERO_DM64): case (ZERO_PM64): case (ZERO_PM8_EXT):
			 case (ZERO_PM16_EXT): case (ZERO_PM32_EXT): case (ZERO_PM48_EXT): 		
				dataWordCount *= 10;
				while(dataWordCount--)				// Zero Tags -> wait because slave needs time to write 0s
				{
					asm("nop;");
				}				
				dataWordCount = 0;					// transmit nothing
				break;
			case (INIT_DM16): case (INIT_DM32):	case (INIT_DM40): case (INIT_PM16): case (INIT_PM32):
			 case (INIT_PM40): case (INIT_DM64): case (INIT_PM64): case (INIT_PM8_EXT):
			 case (INIT_PM16_EXT): case (INIT_PM32_EXT):
				// do not change dataWordCount
				asm("nop;");
				break;					
			case (INIT_PM48):	case (INIT_PM48_EXT):
				dataWordCount = dataWordCount * INSTR_48_MULTI;  // this modification is necessary because of loader output
				break;				
			default:
				emergencyLimit = 0;      			//force an exit
				dataWordCount = 0;
				break;
		} //end switch
		
		while(dataWordCount--)
		{    
			*(long *)SPITX = *(local_source_ptr++);
			while(!((*(long *)SPISTAT & (RXS0))))	// wait for data shifted in
			{
				asm("nop;");
			}    
			temp = *(long *)SPIRX;					// get dummy data from recveive reg - don't use		 
		}
						
	}while(local_source_ptr < (source_ptr + emergencyLimit));   
	// if loaderfile is corrupt we need an exit condition.
	
	for(tempWait = 2000;tempWait;tempWait--)	
	{
		asm("nop;");
	}

	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	*(long*)(IOFLAG) |= DESELECT_SPI_SLAVE;		// deselect Slave 
	asm("nop;");
}




static unsigned long SEGMENT_EXT_CODE checkSlaveDSPforCorrectBoot(unsigned long whichSlave, unsigned long whichLED, unsigned long setExitWait)
{
	unsigned long localExitWait, tempWait, spiRed;
	unsigned long localSetExitWait;
	localExitWait = 0;
	localSetExitWait = setExitWait;
	do
	{
		IO_BUFFER_WRITE ^= whichLED;
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
		spiRed = readSPIword(whichSlave, SPI_SETTINGS_FOR_DSP);
		for(tempWait = 2000000;tempWait;tempWait--)
		{
			asm("nop;");
			asm("nop;");
		}
		localExitWait++;
	}while((spiRed != MACIGWORD_FOR_BOOT_CHECK) && (localExitWait < localSetExitWait));

	if(localExitWait > (localSetExitWait - 2))
	{
		IO_BUFFER_WRITE &= ~(whichLED);			 // lite on LED if Slave did not answer correct (or to late) 
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
		return(1);
	}
	else
	{
		IO_BUFFER_WRITE |= whichLED;
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
		return(0);
	}
}


void SEGMENT_EXT_CODE respondToAtmel(BOOL bootSuccessful)
{
	unsigned long byteCount;
	unsigned long serieNo[20];
	t_P4LT_Status  * g_p_StatusData;

	set_uC_TaskMode();

	if(bootSuccessful)
	{
		sendSPIbyte(FIRST_BYTE_DSP_BOOT_OK, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(SECOND_BYTE_DSP_BOOT_OK, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(THIRD_BYTE_DSP_BOOT_OK, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(FOURTH_BYTE_DSP_BOOT_OK, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
	}
	else
	{
		sendSPIbyte(FIRST_BYTE_DSP_BOOT_FAILD, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(SECOND_BYTE_DSP_BOOT_FAILD, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(THIRD_BYTE_DSP_BOOT_FAILD, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
		sendSPIbyte(FOURTH_BYTE_DSP_BOOT_FAILD, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
		wait_for_Spi(10000);
	}

	for(byteCount = 0;byteCount<16;byteCount++)
	{
		serieNo[byteCount] = ((readSPIword(CS_SPI_MIC, SPI_SETTINGS_FOR_MIC)) & 0xFF) ;
		wait_for_Spi(10000);
	}

	g_p_StatusData       = status_getPStatus();

	if(serieNo[0] != 0xFF)	// serie No = 0xFF if not programmed properly
	{
		g_p_StatusData->system.serieNumber[0] = ((serieNo[0] & MASK_8LSB) << SB24); 
		g_p_StatusData->system.serieNumber[0] |= ((serieNo[1] & MASK_8LSB) << SB16);  
		g_p_StatusData->system.serieNumber[0] |= ((serieNo[2] & MASK_8LSB) << SB8);  
		g_p_StatusData->system.serieNumber[0] |= (serieNo[3] & MASK_8LSB);  

		g_p_StatusData->system.serieNumber[1] = ((serieNo[4] & MASK_8LSB) << SB24); 
		g_p_StatusData->system.serieNumber[1] |= ((serieNo[5] & MASK_8LSB) << SB16); 
		g_p_StatusData->system.serieNumber[1] |= ((serieNo[6] & MASK_8LSB) << SB8);  
		g_p_StatusData->system.serieNumber[1] |= (serieNo[7] & MASK_8LSB);   

		g_p_StatusData->system.serieNumber[2] = ((serieNo[8] & MASK_8LSB) << SB24);  
		g_p_StatusData->system.serieNumber[2] |= ((serieNo[9] & MASK_8LSB) << SB16);  
		g_p_StatusData->system.serieNumber[2] |= ((serieNo[10] & MASK_8LSB) << SB8);   
		g_p_StatusData->system.serieNumber[2] |= (serieNo[11] & MASK_8LSB);  
	}
	else					// serie No was not progarmmed properly -> set to 0 (ascii)
	{
		g_p_StatusData->system.serieNumber[0] = 0x30;
		g_p_StatusData->system.serieNumber[1] = 0x30;
		g_p_StatusData->system.serieNumber[2] = 0x30;
	}

	g_p_StatusData->system.fw_version_atmel = ((serieNo[12] & MASK_8LSB) << SB8);	// Major version
	g_p_StatusData->system.fw_version_atmel |= (serieNo[13] & MASK_8LSB);			// Minor version
}




void SEGMENT_EXT_CODE booting_bootAllDSPs(void)
{
	unsigned long tempWait;
	unsigned long rebootSystem;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	rebootSystem = 0;

	if(!isInDebugMode())
	{

		for(tempWait = 10000;tempWait;tempWait--)
		{
			asm("nop;");
		}
		bootSlaveDSP(&loader_file_NSK_DSP[0], sizeof(loader_file_NSK_DSP), CS_SPI_DSP_NSK);
		IO_BUFFER_WRITE |= (P4LT_AL_LED);								// Turn off P4LT Alarm Led
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer		
		for(tempWait = 10000;tempWait;tempWait--)
		{
			asm("nop;");
		}
		bootSlaveDSP(&loader_file_AFE_DSP[0], sizeof(loader_file_AFE_DSP), CS_SPI_DSP_AFE);	
		IO_BUFFER_WRITE &= ~(P4LT_AL_LED);								// Turn on P4LT Alarm Led
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer
		for(tempWait = 10000;tempWait;tempWait--)
		{
			asm("nop;");
		}
		bootSlaveDSP(&loader_file_NSD_DSP[0], sizeof(loader_file_NSD_DSP), CS_SPI_DSP_TP);
		IO_BUFFER_WRITE |= (P4LT_AL_LED);								// Turn off P4LT Alarm Led
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer
		for(tempWait = 10000;tempWait;tempWait--)
		{
			asm("nop;");
		}
		bootSlaveDSP(&loader_file_OFDM_DSP[0], sizeof(loader_file_OFDM_DSP), CS_SPI_DSP_OFDM);
		IO_BUFFER_WRITE &= ~(P4LT_AL_LED);								// Turn on P4LT Alarm Led
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer
		for(tempWait = 10000;tempWait;tempWait--)
		{
			asm("nop;");
		}
		bootSlaveDSP(&loader_file_UC_DSP[0], sizeof(loader_file_UC_DSP), CS_SPI_DSP_AFE2);	
		IO_BUFFER_WRITE |= (P4LT_AL_LED);								// Turn off P4LT Alarm Led
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer
		for(tempWait = 400000;tempWait;tempWait--)
		{
			asm("nop;");
		}

		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_AFE, READY_LED, EXIT_WAIT_FOR_AFE_DSP);	
	
		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_TP, WARNING_LED, EXIT_WAIT_FOR_NSD_DSP);	

		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_NSK, LINK_AL_LED, EXIT_WAIT_FOR_NSK_DSP);
		getPiggyBackInfoFromNSK_DSP();

		// not implemented at OFDM DSP
	//	rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_OFDM, HW_AL_LED_DSP, EXIT_WAIT_FOR_OFDM_DSP);		
	//	dummy check for OFDM DSP	///////////////////////////////////////////////////////////////	
		IO_BUFFER_WRITE ^= HW_AL_LED_DSP;
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
		for(tempWait = 2000000;tempWait;tempWait--)
		{
			asm("nop;");
			asm("nop;");
		}
		IO_BUFFER_WRITE |= HW_AL_LED_DSP;
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
	///////////////////////////////////////////////////////////////////////////////////////////////
		
		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_AFE2, SYS_AL_LED, EXIT_WAIT_FOR_UC_DSP);
		
		rebootSystem |= memoryCheckDSP_CRC_Alarm();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
///////////////////////         Watchdog activated !!!!!!!!       /////////////////////////////////////////////////		
		transferRTC_Byte(WRITE_RTC_REG_WatchDog, 0x8A);	// init RTC for Watchdog 2 sec
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

	}
	else  // Debug Mode shortened EXIT_WAIT_FOR_....
	{
		for(tempWait = 5000000;tempWait;tempWait--)  // give the slave DSPs a competitive edge
		{
			asm("nop;");
			asm("nop;");
		}

		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_AFE, READY_LED, 10);	
	
		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_TP, WARNING_LED, 4);	

		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_NSK, LINK_AL_LED, 4);
		getPiggyBackInfoFromNSK_DSP();

		rebootSystem |= checkSlaveDSPforCorrectBoot(CS_SPI_DSP_AFE2, SYS_AL_LED, 4);
	}


	if(rebootSystem)	// CRC failure from slave DSP detected!!!
	{
		IO_BUFFER_WRITE &= ~(HW_AL_LED_DSP|LINK_AL_LED|SYS_AL_LED|P4LT_AL_LED|WARNING_LED|READY_LED);// lite on all LEDs if reboot 
		*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer

		transferRTC_Byte(WRITE_RTC_REG_WatchDog, 0x00);	// Disable Watchdog, this will be done by Atmel
		
		if(!isInDebugMode()) //Flag7 = 0 if JtagBoard with J1 closed is plugged, else Flag7 = high 
		{
			respondToAtmel(FALSE);						// Booting was not succesful
			while(1);									// Wait, uP will reset DSPs
		}
		else
		{
			respondToAtmel(TRUE);						// Debug Mode, simulate Booting was succesful
		}
	}
	else				// Booting OK!!!
	{
		respondToAtmel(TRUE);		// Booting was succesful
	}


	reestablishSPI();		// workaround for DSP anomaly 47

	for(tempWait = 10000;tempWait;tempWait--)
	{
		asm("nop;");
		asm("nop;");
	}
}


