/*-------------------------------------------------------------------------*
 * $Workfile	: speechFilter.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Speech filter for the telephony signals
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/speechFilter.c $
 * 
 * 1     07-07-18 16:18 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include	<math.h>
#include	<gao_iir_api.h>
#include 	"speechFilter.h"
#include 	"O4CV_Const.h"
#include 	"sport1.h"
#include 	"configuration.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

const UINT16	numberOfSOS [8] = {5, 5, 5, 5, 5, 5, 5, 4};

#pragma section("L1_data_b")
const INT16 	coeffs2000  [25] = {
0x0164, 0x027E, 0x0164, 0xCA21, 0x10D6, 
0x4000, 0x3AD5, 0x4000, 0xE148, 0x23D3, 
0x4000, 0x19A1, 0x4000, 0xF42B, 0x3368, 
0x4000, 0x0C69, 0x4000, 0xFD3E, 0x3B2D, 
0x4000, 0x0802, 0x4000, 0x0086, 0x3EC6 };

#pragma section("L1_data_b")
const INT16 	coeffs2200  [25] = {
0x020D, 0x03CA, 0x020D, 0xD3A7, 0x0DE3, 
0x4000, 0x4991, 0x4000, 0xF043, 0x2301, 
0x4000, 0x2C44, 0x4000, 0x0652, 0x335D, 
0x4000, 0x1FF3, 0x4000, 0x1093, 0x3B38, 
0x4000, 0x1BC2, 0x4000, 0x1458, 0x3ECA };

#pragma section("L1_data_b")
const INT16 	coeffs2400  [25] = {
0x0306, 0x05B5, 0x0306, 0xDE15, 0x0B58, 
0x4000, 0x5627, 0x4000, 0x0024, 0x22B8, 
0x4000, 0x3D63, 0x4000, 0x18A7, 0x339C, 
0x4000, 0x3274, 0x4000, 0x239C, 0x3B5F, 
0x4000, 0x2EA9, 0x4000, 0x27AA, 0x3ED5 };

#pragma section("L1_data_b")
const INT16 	coeffs2600  [25] = {
0x04AD, 0x04AD, 0x0000, 0xF11D, 0x0000, 
0x4000, 0x6F26, 0x4000, 0xFE90, 0x1605, 
0x4000, 0x5696, 0x4000, 0x220F, 0x2D57, 
0x4000, 0x4910, 0x4000, 0x335C, 0x38FC, 
0x4000, 0x4404, 0x4000, 0x39FA, 0x3E3A };

#pragma section("L1_data_b")
const INT16 	coeffs2800  [25] = {
0x06D3, 0x06D3, 0x0000, 0xF6CE, 0x0000, 
0x4000, 0x741C, 0x4000, 0x0E5E, 0x1669, 
0x4000, 0x61DD, 0x4000, 0x33A1, 0x2E76, 
0x4000, 0x5753, 0x4000, 0x447C, 0x3991, 
0x4000, 0x534C, 0x4000, 0x4AD8, 0x3E63 };

#pragma section("L1_data_b")
const INT16 	coeffs3000  [25] = {
0x09EF, 0x09EF, 0x0000, 0xFD5E, 0x0000, 
0x4000, 0x7804, 0x4000, 0x1F82, 0x17EE, 
0x4000, 0x6B41, 0x4000, 0x44B9, 0x3018, 
0x4000, 0x6396, 0x4000, 0x5433, 0x3A52, 
0x4000, 0x6099, 0x4000, 0x59F0, 0x3E97 };

#pragma section("L1_data_b")
const INT16 	coeffs3200  [25] = {
0x0E6F, 0x0E6F, 0x0000, 0x0520, 0x0000, 
0x4000, 0x7B07, 0x4000, 0x31F7, 0x1AEC, 
0x4000, 0x72D3, 0x4000, 0x54DF, 0x3243, 
0x4000, 0x6DBD, 0x4000, 0x6216, 0x3B39, 
0x4000, 0x6BB9, 0x4000, 0x66E6, 0x3ED4 };

#pragma section("L1_data_b")
const INT16 	coeffs3400  [20] = {
0x15BF, 0x2B3E, 0x15BF, 0x2DD8, 0x0E52, 
0x4000, 0x7B2C, 0x4000, 0x59BF, 0x2D7F, 
0x4000, 0x7785, 0x4000, 0x6B06, 0x39E3, 
0x4000, 0x75DC, 0x4000, 0x7106, 0x3E87 };


#define		MAX_NBR_SOS			5			// Max number of SOS  
#define		STATE_PER_SOS		2			// States per SOS


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

#pragma section("L1_data_a")
static		fract16	g_xbuffRx[N_CHANNELS_ON_BOARD][MAX_NBR_SOS*STATE_PER_SOS];		// FOR OPTIMAL PERFORMANCE, 

#pragma section("L1_data_a")
static		fract16	g_ybuffRx[N_CHANNELS_ON_BOARD][MAX_NBR_SOS*STATE_PER_SOS];		// COEFFS[] AND _BUFF[] MUST 

#pragma section("L1_data_a")
static		fract16	g_xbuffTx[N_CHANNELS_ON_BOARD][MAX_NBR_SOS*STATE_PER_SOS];		// BE PLACED IN DIFF. MEM.

#pragma section("L1_data_a")
static		fract16	g_ybuffTx[N_CHANNELS_ON_BOARD][MAX_NBR_SOS*STATE_PER_SOS];	
																				
static		iir_df1_05_state	g_fstateRx[N_CHANNELS_ON_BOARD];	
static		iir_df1_05_state	g_fstateTx[N_CHANNELS_ON_BOARD];	

static 		bool g_isSpeechFilterEnabled[N_CHANNELS_ON_BOARD];

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void speechFilter_init(t_O4CVConfig* pConfig)
  Description:	Sets the speech filter settings according to the configuration
  Inputs:		pConfig: Pointer to the configuration 
  Outputs:		none
  Return:       none
  Globals:		g_isSpeechFilterEnabled: Shows if speech filter is enabled 
  				g_fstateRx: State of the iir input filter. Containing x-buffer, 
  				y-buffer, coefficients and the nr of SOS
  				g_fstateTx: State of the iir output filter. Containing x-buffer, 
  				y-buffer, coefficients and the nr of SOS
  Important:
*******************************************************************************/
void speechFilter_init(t_O4CVConfig* pConfig)
{
    UINT32 i = 0;
    
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++) 
	{
        // Set x & y buffers
	    g_fstateRx[i].x	= g_xbuffRx[i];
		g_fstateRx[i].y	= g_ybuffRx[i];
		g_fstateTx[i].x	= g_xbuffTx[i];
		g_fstateTx[i].y	= g_ybuffTx[i];
	    
		// Check if speech filter is enabled in the config
		if(pConfig->channel[i].afInterface.settings & AF_INTERFACE_FILTER_EN)
		{
			g_isSpeechFilterEnabled[i] = true;	
		}
		else
		{
		 	g_isSpeechFilterEnabled[i] = false;	   
		}
	
		// Set cut off freq to default if out of range
		if(pConfig->channel[i].afInterface.filterInd > 7)
		{
		    pConfig->channel[i].afInterface.filterInd = 7;
		}
		
		// Set the number of SOS according to the config
		g_fstateRx[i].k	= numberOfSOS[pConfig->channel[i].afInterface.filterInd];
		g_fstateTx[i].k	= numberOfSOS[pConfig->channel[i].afInterface.filterInd];
		
		// Set the coeffs according to the config
	    switch(pConfig->channel[i].afInterface.filterInd)
	    {   
	        // fg = 2000Hz 
	    	case 0:	g_fstateRx[i].c	= coeffs2000;
	    			g_fstateTx[i].c	= coeffs2000;
	    			break;
	    			
	    	// fg = 2200Hz 		
	    	case 1:	g_fstateRx[i].c	= coeffs2200;
	    			g_fstateTx[i].c	= coeffs2200;
	    			break;
	    			
	    	// fg = 2400Hz 		
	    	case 2:	g_fstateRx[i].c	= coeffs2400;
	    			g_fstateTx[i].c	= coeffs2400;
	    			break;
	    			
	    	// fg = 2600Hz 		
	    	case 3:	g_fstateRx[i].c	= coeffs2600;
	    			g_fstateTx[i].c	= coeffs2600;
	    			break;
	    			
	    	// fg = 2800Hz 		
	    	case 4:	g_fstateRx[i].c	= coeffs2800;
	    			g_fstateTx[i].c	= coeffs2800;
	    			break;
	    			
	    	// fg = 3000Hz 		
	    	case 5:	g_fstateRx[i].c	= coeffs3000;
	    			g_fstateTx[i].c	= coeffs3000;
	    			break;
	    					
	    	// fg = 3200Hz 		
	    	case 6:	g_fstateRx[i].c	= coeffs3200;
	    			g_fstateTx[i].c	= coeffs3200;
	    			break;
	    			
	    	// fg = 3400Hz 		
	    	case 7:	g_fstateRx[i].c	= coeffs3400;
	    			g_fstateTx[i].c	= coeffs3400;
	    			break;
	    			
	    	default: 	
					break;
	
	    }
	
	}
	    
}


/*****************************************************************************
  Function:		void speechFilter_apply(INT16* bufferIn, INT16* bufferOut, 
  				UINT16 channel, INT16 rxTx)
  Description:	filters the telephony signal with an iir filter.
  				calls the iir filter function from gao.
  Inputs:		bufferIn: Input buffer with 240 samples
  				channel: Shows which of the 5 channels should be filtered  
  				rxTx: Shows if the filter is at the in- or output
  Outputs:		bufferOut: Output buffer with 240 samples
  Return:   	none    
  Globals:		g_isSpeechFilterEnabled: Shows if speech filter is enabled 
  				g_fstateRx: State of the iir input filter. Containing x-buffer, 
  				y-buffer, coefficients and the nr of SOS
  				g_fstateTx: State of the iir output filter. Containing x-buffer, 
  				y-buffer, coefficients and the nr of SOS
  Important:
*******************************************************************************/
#pragma optimize_for_speed
void speechFilter_apply(INT16* bufferIn, INT16* bufferOut, UINT16 channel, INT16 rxTx)
{
    UINT32 i = 0;
    
    if(g_isSpeechFilterEnabled[channel])
    {
	
        // Call the gao iir filter function to filter 240 samples (30ms)
        if(rxTx == speechFilter_Rx)
        {
			for (i = 0; i < BUFFER_SIZE_2W_IF; i++) 
			{							
				iir_df1_05 (&bufferIn[i], &bufferOut[i], 1, &g_fstateRx[channel]);	
			}
        }
        else
        {
         	for (i = 0; i < BUFFER_SIZE_2W_IF; i++) 
			{							
				iir_df1_05 (&bufferIn[i], &bufferOut[i], 1, &g_fstateTx[channel]);	
			}   
			
        }
        
    }
    
}




