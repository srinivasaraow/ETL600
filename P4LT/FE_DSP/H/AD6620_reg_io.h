/*-------------------------------------------------------------------------*
 * $Workfile: AD6620_reg_io.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Declaration of function prototypes
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/H/AD6620_reg_io.h $ 
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 2     18.06.04 13:38 Maag01
 * 
 * 1     28.05.04 13:30 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

#ifndef AD6620_REG_IO_H
#define AD6620_REG_IO_H

void AD6620_HW_Reset(void);
int Set_AD6620_MCR(int data);
int Set_AD6620_NCO_CR(int data);
int Set_AD6620_NCO_FREQ_CR(int data);
int Set_AD6620_NCO_Frequency(int data);
int Set_AD6620_NCO_Phase(int data);
int Set_AD6620_CIC2_Scale(int data);
int Set_AD6620_CIC2_Decimation(int data);
int Set_AD6620_CIC5_Scale(int data);
int Set_AD6620_CIC5_Decimation(int data);
int Set_AD6620_RCF_Scale(int data);
int Set_AD6620_RCF_Decimation(int data);
int Set_AD6620_RCF_Offset(int data);
int Set_AD6620_RCF_Ntaps(int data);
void Set_AD6620_RESERVED();
int Set_AD6620_RCF_Coef(unsigned int addr,int coef);
int AD6620_Init_MSCRM();
int AD6620_Init_Test(unsigned int);
int AD6620_Start();
int AD6620_SRESET();

#endif
