/*-------------------------------------------------------------------------*
 * $Workfile: get_configuration.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/get_configuration.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 1     28.09.04 18:02 Maag01
 * Initial version
*/
#include <stdlib.h>


#include "../../h/tdm/ofdm_tdm_externs.h"
#include "../../h/ofdm_externs.h"
#include "../../h/util/util.h"
#include "../../h/OFDM_CFG_Types.h"
#include "../../h/TDM_Service.H"


//============================================

int segment ( "seg_ext_pmco" ) Get_Configuration(t_OFDM_Config *config)
{

    int Get_Message_from_Master(int *buffer);
    int Send_Message_to_Master(int *buffer, int len, e_TDMServiceMsgType mtype);
    
	// wait for configuration message from Master DSP
	while ( (mlen_rx=Get_Message_from_Master((int *)mes_rx)) == 0);
	
	Message_Header_Ptr = (t_TDMServiceHeader *)mes_rx;

    // Test TDM Master Control Channel flags
          
    if ( ControlRX_Service_Buffer_Overrun == 1 ) exit(1);

    if ( ControlRX_Service_Corrupted_Message == 1 ) exit(1);
          
    if ( ControlRX_Service_Secondary_Start == 1 ) exit(1);          
    
#if 0
 asm("#include <def21161.h>");
 asm("dm(CP1A)=m5;");
 asm("dm(CP3A)=m5;");
 asm("dm(SPCTL0) = m5;");
 asm("dm(SPCTL2) = m5;");    
 asm("nop;");
#endif    

    if ( Message_Header_Ptr->messageType == e_TDMServiceMsgType_Cfg_Send ) {

      // Configuration Message from Master is received
      if ( Message_Header_Ptr->messageSize == sizeof(t_OFDM_Config)) {

		 // Calculate checksum
         // In CRC: ADDR_FIELD+TYPE_FIELD+SIZE_FIELD+USER_DATA_FIELD, i.e 3+USER_DATA_SIZE
         mes_crc = vsum_i_E((int *)&Message_Header_Ptr->destAddr,Message_Header_Ptr->messageSize + 3);

         if (mes_crc == mes_rx[mlen_rx-2]) {
         	   // CRC is OK: reply ACK (e_TDMServiceMsgType_Ack with 0 length user packet)
         	   Send_Message_to_Master(NULL, 0, e_TDMServiceMsgType_Ack);
        	   vcopy_EE((float *)config, (float *)&mes_rx[sizeof(t_TDMServiceHeader)],sizeof(t_OFDM_Config));
			 } else {
        	   // Bad CRC: reply NACK (e_TDMServiceMsgType_Nack with 0 length user packet)
         	   Send_Message_to_Master(NULL, 0, e_TDMServiceMsgType_Nack);
         	   return 1;
         	 }
      }

    } else {
      // Non Configuration Message: reply NACK reply NACK (e_TDMServiceMsgType_Nack with 0 length user packet)
       Send_Message_to_Master(NULL, 0, e_TDMServiceMsgType_Nack);
	  return 1;		   
    }

	return 0;
}
