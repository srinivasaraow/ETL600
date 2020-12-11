/*-------------------------------------------------------------------------*
 * $Workfile: get_message.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/get_message.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 1     28.09.04 18:02 Maag01
 * Initial version
*/
#include <stdlib.h>

#include "../../h/p4lt_tdm/tdm_bus_io.h"
#include "../../h/p4lt_tdm/p4lt_tdm_defs.h"

#include "../../h/tdm/ofdm_tdm_externs.h"
#include "../../h/ofdm_externs.h"


//============================================

int segment ( "seg_ext_pmco" ) Get_Message_from_Master(int *buffer)
{
	int i;
	
   if (ControlRX_Service_Number_of_Messages == 0 ) return 0;
   while( !ControlRX_Service_Read_Is_Updated ); 
   buffer[0] = ControlRX_Service_Buffer[ControlRX_Service_Read_Ind++];
   if (ControlRX_Service_Read_Ind == sizeof(ControlRX_Service_Buffer) ) ControlRX_Service_Read_Ind = 0;
   buffer[1] = ControlRX_Service_Buffer[ControlRX_Service_Read_Ind++];
   if (ControlRX_Service_Read_Ind == sizeof(ControlRX_Service_Buffer) ) ControlRX_Service_Read_Ind = 0;
   buffer[2] = ControlRX_Service_Buffer[ControlRX_Service_Read_Ind++];
   if (ControlRX_Service_Read_Ind == sizeof(ControlRX_Service_Buffer) ) ControlRX_Service_Read_Ind = 0;
   buffer[3] = ControlRX_Service_Delivered_Words = ControlRX_Service_Buffer[ControlRX_Service_Read_Ind++];
   if (ControlRX_Service_Read_Ind == sizeof(ControlRX_Service_Buffer) ) ControlRX_Service_Read_Ind = 0;
   ControlRX_Service_Delivered_Words += 6;
   for (i=4; i<ControlRX_Service_Delivered_Words; i++) {
     buffer[i] = ControlRX_Service_Buffer[ControlRX_Service_Read_Ind++];
     if (ControlRX_Service_Read_Ind == sizeof(ControlRX_Service_Buffer) ) ControlRX_Service_Read_Ind = 0;
   }
   ControlRX_Service_Read_Is_Updated = 0;
   
   return ControlRX_Service_Delivered_Words;		
}
