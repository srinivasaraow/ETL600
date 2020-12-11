/*-------------------------------------------------------------------------*
 * $Workfile: intDec.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : D. Cachin 
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/intDec.h $
 * 
 * 1     4.07.07 11:50 Chhamaa
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef INT_DEC_H
#define INT_DEC_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void intDec_init(void);
void intDec_interpolate(INT16 *pBufferIn, INT16 *pBufferOut, INT16 channel);
void intDec_decimate(INT16 *pBufferIn, INT16 *pBufferOut, INT16 channel);


#endif
