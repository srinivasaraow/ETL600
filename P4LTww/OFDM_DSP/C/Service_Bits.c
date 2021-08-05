/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: Service_Bits.c $
 * COPYRIGHT   : 2005 TL Consulting GmbH
 * DESCRIPTION : Service Bit Handler
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Service_Bits.c $
 * 
 * 1     28.01.05 11:51 Ats
 * Exchange TLC (New Files)
 * 
 *
 * Initial version
 *       15.01.2005 PL
 *****************************************************************************/
 

//========================================

void segment ( "seg_ext_pmco" ) Set_Service_Bit(int *OutBit, int position, int max_pos, int value)
{
	
      if  ( position < max_pos ) {

        //move pointer to position
		OutBit += position;

		switch (value)
		 {
			case 0: 
				*OutBit = 0;
			break;

			case 1:
				*OutBit = 1;
			break;

			default:
				// Error: value must be zero or one, do nothing
			break;
		 }
         
      }

}

int segment ( "seg_ext_pmco" ) Get_Service_Bit(int *InBit, int position, int max_pos)
{
	unsigned int value;
	
      if  ( position < max_pos ) {

        //move pointer to position
		InBit += position;

		value = *InBit;
		
		if (value == 0) return 0;
		else if (value == 1) return 1;
		else return -1;
		         
      }
      else return -1;

}

