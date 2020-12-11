/*-------------------------------------------------------------------------*
 * $Workfile	: gain.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Gain of the telephony signals
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/gain.c $
 * 
 * 2     07-08-24 17:03 Chalnot
 * FXO table changed.
 * 
 * 1     07-07-18 15:22 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include	<fract.h>
#include 	"gain.h"
#include 	"O4CV_Const.h"
#include 	"sport1.h"
#include 	"configuration.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

const UINT16 digitalGainTableInterval = 50; // 0.5dbm * 100/dbm = 50

// Gain tables 4-wire
const UINT16 digitalGainTableOffset4w = 40; // -2000(-20dbm) / 50 (interval) = -40

// Input gain from -20dbm to 5dbm in 0.5dbm steps
const INT16 rx_4w_digitalGainTable  [51] = {26520, 25037, 23636, 22314, 21066,
19888, 18775, 17725, 16733, 19888, 18775, 17725, 16733, 31520, 29756, 28092,
26520, 25037, 23636, 22314, 21066, 19888, 18775, 17725, 16733, 31520, 29756,
28092, 26520, 25037, 23636, 22314, 21066, 19888, 18775, 17725, 16733, 31520,
29756, 28092, 26520, 25037, 23636, 22314, 21066, 19888, 18775, 17725, 16733,
15797, 14914 };

// Output gain from -20dbm to 8dbm in 0.5dbm steps
const INT16 tx_4w_digitalGainTable  [57] = {8799, 9320, 9872, 10457, 11077,
11733, 12429, 13165, 13945, 14771, 15647, 11733, 12429, 13165, 13945, 14771,
15647, 11733, 12429, 13165, 13945, 14771, 15647, 11733, 12429, 13165, 13945,
14771, 15647, 11733, 12429, 13165, 13945, 14771, 15647, 11733, 12429, 13165,
13945, 14771, 15647, 11733, 12429, 13165, 13945, 14771, 15647, 11733, 12429,
13165, 13945, 14771, 15647, 16574, 17556, 18596, 19698 };


// Gain tables 2-wire
const UINT16 digitalGainTableOffset2w = 34;	// -1700(-17dbm) / 50 (interval) = -34

// FXO
// Input gain from -17dbm to 4dbm in 0.5dbm steps
const INT16 rx_2w_FXO_digitalGainTable  [43] = {16598, 31339, 29586, 27931, 
26368, 24893, 23501, 22186, 20945, 19774, 18667, 17623, 16637, 31413, 29656, 
27997, 26431, 24953, 23557, 22239, 20995, 19820, 18712, 17665, 16677, 31488, 
29727, 28064, 26494, 25012, 23613, 22292, 21045, 19868, 18756, 17707, 16716, 
31563, 29797, 28130, 26557, 25071, 23669 };

// Output gain from -17dbm to 1dbm in 0.5dbm steps
const INT16 tx_2w_FXO_digitalGainTable  [37] = {3872, 4102, 4345, 4602, 4875, 
5164, 5470, 5794, 6137, 6501, 6886, 7294, 7726, 8184, 8669, 9182, 9726, 10303, 
10913, 11560, 12245, 12970, 13739, 14553, 15415, 16329, 17296, 18321, 19407, 
20557, 21775, 23065, 24431, 25879, 27413, 29037, 30757 };

// FXS
// Input gain from -17dbm to 4dbm in 0.5dbm steps
const INT16 rx_2w_FXS_digitalGainTable  [43] = {31995, 30205, 28515, 26920,
25414, 23993, 22650, 21383, 20187, 19058, 17992, 16985, 32070, 30276, 28583,
26984, 25474, 24049, 22704, 21434, 20235, 19103, 18035, 17026, 32147, 30348,
28651, 27048, 25535, 24107, 22758, 21485, 20283, 19149, 18077, 17066, 32223,
30420, 28719, 27112, 25596, 24164, 22812 };

// Output gain from -17dbm to 1dbm in 0.5dbm steps
const INT16 tx_2w_FXS_digitalGainTable  [37] = {3797, 4022, 4260, 4513, 4780,
5063, 5364, 5681, 6018, 6375, 6752, 7152, 7576, 8025, 8501, 9004, 9538, 10103,
10702, 11336, 12007, 12719, 13473, 14271, 15116, 16012, 16961, 17966, 19030,
20158, 21353, 22618, 23958, 25378, 26881, 28474, 30161 };


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
	INT16	rx_factor[N_CHANNELS_2W];
	INT16	tx_factor[N_CHANNELS_2W];
	UINT16	rx_shifts[N_CHANNELS_2W];
	UINT16	tx_shifts[N_CHANNELS_2W];	
} t_digitalGain;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static t_digitalGain g_digitalGain;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void gain_init(t_O4CVConfig* pConfig, t_4w_Gains* gainSettings4wIf)
  Description:	Sets the gain settings according to the configuration
  Inputs:		pConfig: Pointer to the configuration 
  Outputs:		gainSettings4wIf: Settings for the analog gain
  Return:       none
  Globals:		g_digitalGain: Settings for the digital gain 
  Important:
*******************************************************************************/
void gain_init(t_O4CVConfig* pConfig, t_4w_Gains* gainSettings4wIf)
{
	UINT32 i = 0;
	UINT16 index = 0;
	INT16 tempGain = 0;
	    
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++) 
	{
	    // 2-Wire
	    if(configuration_is2wireInterface(i))
	    {
	        // FXO
	        if(pConfig->channel[i].afInterface.mode == AF_INTERFACE_2_WIRE_PAX)
	        {
	            // Calculate rx_factor and rx_shifts of the digital gain
			    index = (pConfig->channel[i].afInterface.inputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
			    g_digitalGain.rx_factor[i] = rx_2w_FXO_digitalGainTable[index];
			    			    
			    if(pConfig->channel[i].afInterface.inputGain <= -1700)
			    {
			     	g_digitalGain.rx_shifts[i] = 4; 	   
			    }
			    else if(pConfig->channel[i].afInterface.inputGain <= -1100)
			    {
			     	g_digitalGain.rx_shifts[i] = 3;    
			    }
			    else if(pConfig->channel[i].afInterface.inputGain <= -500)
			    {
			     	g_digitalGain.rx_shifts[i] = 2; 	   
			    }
			    else if(pConfig->channel[i].afInterface.inputGain <= 100)
			    {
			     	g_digitalGain.rx_shifts[i] = 1;    
			    }
			    else 
			    {
			     	g_digitalGain.rx_shifts[i] = 0;   
			    }
		     	
			    // Calculate tx_factor and tx_shifts of the digital gain
			    index = (pConfig->channel[i].afInterface.outputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
			    g_digitalGain.tx_factor[i] = tx_2w_FXO_digitalGainTable[index];
			    
			    g_digitalGain.tx_shifts[i] = 0; 

	        }
	        // FXS
	        else
	        {
	            // Calculate rx_factor and rx_shifts of the digital gain
			    index = (pConfig->channel[i].afInterface.inputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
			    g_digitalGain.rx_factor[i] = rx_2w_FXS_digitalGainTable[index];
			    			    
			    if(pConfig->channel[i].afInterface.inputGain <= -1150)
			    {
			     	g_digitalGain.rx_shifts[i] = 3; 	   
			    }
			    else if(pConfig->channel[i].afInterface.inputGain <= -550)
			    {
			     	g_digitalGain.rx_shifts[i] = 2;    
			    }
			    else if(pConfig->channel[i].afInterface.inputGain <= 50)
			    {
			     	g_digitalGain.rx_shifts[i] = 1; 	   
			    }
			    else 
			    {
			     	g_digitalGain.rx_shifts[i] = 0;   
			    }
		     	
			    // Calculate tx_factor and tx_shifts of the digital gain
			    index = (pConfig->channel[i].afInterface.outputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
			    g_digitalGain.tx_factor[i] = tx_2w_FXS_digitalGainTable[index];
			    
			    g_digitalGain.tx_shifts[i] = 0; 	   
	            
	        }
	        
	        
	    }
	    // 4-Wire
	    else	
		{
		        
		 	// Calculate rx_factor and rx_shifts of the digital gain
		    index = (pConfig->channel[i].afInterface.inputGain / digitalGainTableInterval) + digitalGainTableOffset4w;
		    g_digitalGain.rx_factor[i] = rx_4w_digitalGainTable[index];
		    g_digitalGain.rx_shifts[i] = 1;  
		     	
		    // Calculate tx_factor and tx_shifts of the digital gain
		    index = (pConfig->channel[i].afInterface.outputGain / digitalGainTableInterval) + digitalGainTableOffset4w;
		    g_digitalGain.tx_factor[i] = tx_4w_digitalGainTable[index];
		    g_digitalGain.tx_shifts[i] = 1;    
		     	
		    // Calculate rx_gain of the analog gain
		    if(pConfig->channel[i].afInterface.inputGain <= -1600)
		    {
		     	gainSettings4wIf->rx_gain[i] = RX_GAIN_20DB;	   
		    }
		    else if(pConfig->channel[i].afInterface.inputGain <= -1400)
		    {
		     	gainSettings4wIf->rx_gain[i] = RX_GAIN_18DB;	   
		    }
		    else if(pConfig->channel[i].afInterface.inputGain <= -800)
		    {
		     	gainSettings4wIf->rx_gain[i] = RX_GAIN_12DB;	   
		    }
		    else if(pConfig->channel[i].afInterface.inputGain <= -200)
		    {
		     	gainSettings4wIf->rx_gain[i] = RX_GAIN_6DB;	   
		    }
		    else 
		    {
		     	gainSettings4wIf->rx_gain[i] = RX_GAIN_0DB;	   
		    }
		     	
		     	
		    // Calculate tx_gain of the analog gain
		    if(pConfig->channel[i].afInterface.outputGain <= -1500)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_m15DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= -1200)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_m12DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= -900)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_m9DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= -600)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_m6DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= -300)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_m3DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= 0)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_0DB; 	   
		    }
		    else if(pConfig->channel[i].afInterface.outputGain <= 300)
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_p3DB;   
		    }
		    else 
		    {
		     	gainSettings4wIf->tx_gain[i] = TX_GAIN_p6DB;   
		    }

	    		
		} // End if(pConfig->channel[i].afInterface.mode == AF_INTERFACE_4_WIRE)
		
	} // End for (i=0; i<N_CHANNELS_ON_BOARD; i++) 
	
	
	// Service phone
	// Calculate rx_factor and rx_shifts of the digital gain
	index = (pConfig->servicePhone.afInterface.inputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
	g_digitalGain.rx_factor[4] = rx_2w_FXS_digitalGainTable[index];
	    			    
	if(pConfig->servicePhone.afInterface.inputGain <= -1150)
	{
	   	g_digitalGain.rx_shifts[4] = 3; 	   
	}
	else if(pConfig->servicePhone.afInterface.inputGain <= -550)
	{
	 	g_digitalGain.rx_shifts[4] = 2;    
	}
	else if(pConfig->servicePhone.afInterface.inputGain <= 50)
	{
	  	g_digitalGain.rx_shifts[4] = 1; 	   
	}
	else 
	{
	   	g_digitalGain.rx_shifts[4] = 0;   
	}
		     	
	// Calculate tx_factor and tx_shifts of the digital gain
	index = (pConfig->servicePhone.afInterface.outputGain / digitalGainTableInterval) + digitalGainTableOffset2w;
	g_digitalGain.tx_factor[4] = tx_2w_FXS_digitalGainTable[index];
			    
	g_digitalGain.tx_shifts[4] = 0; 	 
	
}


/*****************************************************************************
  Function:		void gain_apply(INT16* buffer, UINT16 channel, UINT16 rxTx)
  Description:	Multiplies the values in the buffer with the factor and 2^shifts
  Inputs:		buffer: Buffer with 240 samples
  				channel: Shows which of the 5 channels should be gained  
  				rxTx: Shows if the buffer is from the input or the output
  Outputs:		buffer: Buffer with 240 samples 
  Return:   	none    
  Globals:		g_digitalGain: Settings for the digital gain 
  Important:
*******************************************************************************/
#pragma optimize_for_speed
void gain_apply(INT16* buffer, UINT16 channel, UINT16 rxTx)
{
    INT16 tempFactor = 0;
    UINT16 tempShifts = 0;
    UINT32 i = 0;
    INT16 tempBuffer [BUFFER_SIZE_2W_IF];

    if(rxTx == gain_Rx)
    {
     	tempFactor = g_digitalGain.rx_factor[channel];  
     	tempShifts = g_digitalGain.rx_shifts[channel]; 
    }
    else
    {
      	tempFactor = g_digitalGain.tx_factor[channel];  
      	tempShifts = g_digitalGain.tx_shifts[channel];  
    }
        
    for (i = 0; i < BUFFER_SIZE_2W_IF; i++) 
    {
    	tempBuffer[i] = shl_fr1x16( multr_fr1x16(buffer[i], tempFactor), tempShifts);
	}
	
	for (i = 0; i < BUFFER_SIZE_2W_IF; i++) 
	{
    	buffer[i] = tempBuffer[i];
	}
	
}




