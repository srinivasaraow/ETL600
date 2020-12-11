/*---------------------------------------------------------------------------*
 * $Workfile: SupervisionUC_bus_UC_DSP.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : BB
 * Remarks      :  
 * Purpose      : Supervision of UC bus
 * Copyright    : ABB Switzerland Ltd., 2006
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/UC_DSP/H/SupervisionUC_bus_UC_DSP.h $
 * 
 * 1     4.04.06 17:31 Chmibuh
 * 
 * 1     14.03.06 10:24 Chbrbus
 * 
 * 1     10.03.06 16:03 Chbrbus
 * 
 *---------------------------------------------------------------------------*/

#ifndef SUPERVISIONUC_BUS_FE_DSP_H
#define SUPERVISIONUC_BUS_FE_DSP_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/

void CheckUpconverterBusOnFE_DSP(volatile long *rxBufferPtr);
void RestartUC_TDM(void);

#endif


