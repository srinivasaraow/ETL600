/*---------------------------------------------------------------------------*
 * $Workfile: SupervisionUC_bus_FE_DSP.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : BB
 * Remarks      :  
 * Purpose      : Supervision of UC bus
 * Copyright    : ABB Switzerland Ltd., 2006
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/FE_DSP/H/SupervisionUC_bus_FE_DSP.h $
 * 
 * 1     4.04.06 18:02 Chmibuh
 * 
 * 3     15.03.06 11:51 Chbrbus
 * Added DMA supervision UC TDM bus: New functions
 * IncrementDMA_ErrorCounter() and SetDMA_ErrorCounterZero(), tested
 * 
 * 2     14.03.06 16:10 Chbrbus
 * Changed definition RestartUC_TDM
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
bool RestartUC_TDM(void);
void IncrementDMA_ErrorCounter(void);
void SetDMA_ErrorCounterZero(void);

#endif


