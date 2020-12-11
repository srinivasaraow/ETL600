/*-------------------------------------------------------------------------*
 * $Workfile: init_control_services.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/init_control_services.c $
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


void segment ( "seg_ext_pmco" ) Init_Control_Services()
{
 
// TDM RX Control Channel buffer variables
 ControlRX_Service_Write_Ind = 0;
 ControlRX_Service_Read_Ind = 0;
 ControlRX_Service_Buffer_Ptr = ControlRX_Service_Buffer;
 ControlRX_Service_Buffer_Length = sizeof(ControlRX_Service_Buffer);

 ControlRX_Service_Buffer_Overrun = 0;
 ControlRX_Service_Message_Ready = 0;
 ControlRX_Service_Busy_Flag = 0;
 ControlRX_Service_Corrupted_Message = 0;
 ControlRX_Service_Number_of_Messages = 0;
 ControlRX_Service_Number_of_Words = 0;
 ControlRX_Service_Read_Is_Updated = 1;
 ControlRX_Service_Delivered_Words = 0;
 ControlRX_Service_Secondary_Start = 0;
 
// TDM RX Control Channel buffer variables
 ControlTX_Service_Write_Ind = 0;
 ControlTX_Service_Read_Ind = 0;
 ControlTX_Service_Buffer_Ptr = ControlTX_Service_Buffer;
 ControlTX_Service_Buffer_Length = sizeof(ControlTX_Service_Buffer);
 
 ControlTX_Service_Write_Is_Updated = 1;
 ControlTX_Service_Delivered_Words = 0;

 ControlTX_Service_Buffer_Empty = 1;
 ControlTX_Service_Number_of_Words = 0;


}

