/*-------------------------------------------------------------------------*
 * $Workfile: send_message.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/send_message.c $
 * 
 * 3     26.04.05 8:45 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 2     28.01.05 11:49 Ats
 * Exchange TLC
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
#include "../../h/ofdm_cfg_types.h"

//============================================

int segment ( "seg_ext_pmco" ) Send_Message_to_Master(int *buffer, int len, e_TDMServiceMsgType mtype)
{
   int                i, block_len;
   unsigned           mes_crc;
   t_TDMServiceHeader *Message_Header_Ptr;
   
   void Write_ControlTX_Service_Buffer(unsigned *, int);
   
   if (len > sizeof(mes_tx)-6) len = sizeof(mes_tx)-6;
   block_len = len+6; //6 is size of header and tail

   Message_Header_Ptr = (t_TDMServiceHeader *)mes_tx;

   Message_Header_Ptr->startChar = TDM_SERVICE_START_CHAR;
   Message_Header_Ptr->destAddr = e_TDMServiceAddr_Master;
   Message_Header_Ptr->messageType = mtype;
   Message_Header_Ptr->messageSize = len;
   
   if (len != 0) vcopy_EE((float *)&mes_tx[sizeof(t_TDMServiceHeader)], (float *)buffer,len);

   // Calculate checksum
   // In CRC: ADDR_FIELD+TYPE_FIELD+SIZE_FIELD+USER_DATA_FIELD, i.e 3+USER_DATA_SIZE
   mes_crc = vsum_i_E((int *)&Message_Header_Ptr->destAddr,Message_Header_Ptr->messageSize + 3);

   mes_tx[block_len-2] = mes_crc;
   mes_tx[block_len-1] = TDM_SERVICE_END_CHAR;

   //while( !ControlTX_Service_Write_Is_Updated ); 
   if (ControlTX_Service_Write_Is_Updated!=0)
   {
   	ControlTX_Service_Write_Is_Updated = 0;
   	
   	if ( block_len > ControlTX_Service_Buffer_Length-ControlTX_Service_Number_of_Words ) return 1;

   	//Write_ControlTX_Service_Buffer(mes_tx,block_len);
   	for (i=0; i<block_len; i++) {
     ControlTX_Service_Buffer[ControlTX_Service_Write_Ind++] = mes_tx[i];
     if (ControlTX_Service_Write_Ind == sizeof(ControlTX_Service_Buffer) ) ControlTX_Service_Write_Ind = 0;
   	}
	ControlTX_Service_Delivered_Words = block_len;
   	
   }
   else
   {
   wait_ControlTX_ctr++;
   }
   return 0;		
}
