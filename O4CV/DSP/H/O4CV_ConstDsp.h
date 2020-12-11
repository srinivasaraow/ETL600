/*-------------------------------------------------------------------------*
 * $Workfile	: O4CV_ConstDsp.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : This header file hosts defines which are used on O4CV ONLY.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/H/O4CV_ConstDsp.h $
 * 
 * 42    12-03-30 11:25 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.23"
 * 
 * 41    12-03-29 9:57 Chalnot
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.23 RC4"
 * 
 * 40    12-03-29 9:56 Chalnot
 * O4CV_EFW_SW_IDENTIFICATION changed to "O4CV Emergency FW Version 1.08"
 * 
 * 39    11-08-24 16:40 Chhamaa
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.23 RC3"
 * 
 * 38    11-08-22 10:45 Chhamaa
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.23 RC2"
 * 
 * 37    11-05-10 17:46 Chhamaa
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.23 RC1"
 * 
 * 35    09-12-21 16:48 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.22"
 * 
 * 33    10.04.08 9:50 Chhamaa
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.21"
 * 
 * 32    08-02-20 10:03 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.20"
 * 
 * 31    08-02-19 13:43 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.18"
 * 
 * 30    08-02-15 13:23 Chrobif
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.17"
 * 
 * 29    08-02-11 14:05 Chrobif
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.16"
 * 
 * 27    8.02.08 13:18 Chhamaa
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.15"
 * 
 * 26    6.02.08 13:50 Chhamaa
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.14"
 * 
 * 25    1.02.08 11:31 Chhamaa
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.13"
 * 
 * 24    08-01-29 18:39 Chrobif
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.12"
 * 
 * 22    29.01.08 17:31 Chhamaa
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.11".
 * 
 * 21    07-12-11 16:18 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.10".
 * 
 * 20    07-12-05 8:54 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.09".
 * 
 * 19    07-11-28 12:02 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.08".
 * 
 * 18    07-11-23 16:08 Chalnot
 * O4CV_EFW_SW_IDENTIFICATION changed to "O4CV Emergency FW Version 1.06"
 * 
 * 17    07-11-19 13:13 Chalnot
 * O4CV_EFW_SW_IDENTIFICATION changed to "O4CV Emergency FW Version 1.05"
 * because it was to long.
 * 
 * 16    07-11-19 9:06 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.06"
 * O4CV_EFW_SW_IDENTIFICATION set to "O4CV Emergency Firmware Version
 * 1.05"
 * 
 * 15    07-10-26 16:43 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.06"
 * 
 * 14    07-10-24 12:50 Chalnot
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.05"
 * 
 * 13    07-10-19 15:02 Chdocac
 * changed to version 1.04
 * 
 * 12    07-10-09 16:19 Chdocac
 * O4CV_SW_IDENTIFICATION set to "O4CV Version 1.02"
 * 
 * 11    07-10-05 11:24 Chrobif
 * Hardware version and HMI600 compatibility index modified.
 * 
 * 9     28.09.07 15:01 Chhamaa
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.01"
 * 
 * 8     28.09.07 14:50 Chhamaa
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.00"
 * 
 * 7     07-09-17 10:19 Chalnot
 * O4CV_EFW_SW_IDENTIFICATION added.
 * 
 * 6     07-07-05 10:04 Chrobif
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.00_alpha_0"
 * 
 * 5     07-06-11 15:06 Chrobif
 * O4CV_SW_IDENTIFICATION	 set to "O4CV Version 1.00"
 * 
 * 4     14.05.07 10:04 Chrobif
 * Comment added.
 * 
 * 3     7.05.07 10:17 Chrobif
 * HMI compatibility version modified.
 * 
 * 2     5.04.07 11:30 Chrobif
 * Bug fix in VSS log area.
 *
 * 1     4.04.07 15:08 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef O4CV_CONST_DSP_H
#define O4CV_CONST_DSP_H

/* INCLUDE FILES **************************************************************/
/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// defines for O4CV software identification (SWID)
#define O4CV_SW_IDENTIFICATION					"O4CV Version 1.23" // Note: O4CV_SW_IDENTIFICATION can have at most (O4CV_SW_ID_LENGTH*4-1) characters
#define O4CV_MMI_COMPATIBILITY_VERSION			1100
#define O4CV_HW_VERSION							0
#define O4CV_EFW_SW_IDENTIFICATION				"O4CV Emergency FW Version 1.08"
#define O4CV_EFW_MMI_COMPATIBILITY_VERSION		1100
#define O4CV_EFW_HW_VERSION						0

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
#endif
