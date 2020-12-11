/*-------------------------------------------------------------------------*
 * $Workfile	: epromLow.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Low level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/epromLow.h $
 * 
 * 1     07-04-17 10:38 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef _EPROMLOW
#define	_EPROMLOW

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// Eprom Sector Addresses for an AM29LV320DB (Bottom Boot)
#define SECTOR_SA0			0x00000000		// 8 KBytes   
#define SECTOR_SA1			0x00002000		// 8 KBytes    
#define SECTOR_SA2			0x00004000		// 8 KBytes    
#define SECTOR_SA3			0x00006000		// 8 KBytes    
#define SECTOR_SA4			0x00008000		// 8 KBytes    
#define SECTOR_SA5			0x0000A000		// 8 KBytes    
#define SECTOR_SA6			0x0000C000		// 8 KBytes    
#define SECTOR_SA7			0x0000E000		// 8 KBytes   
#define SECTOR_SA8			0x00010000		// 64 KBytes 
#define SECTOR_SA9			0x00020000		// 64 KBytes
#define SECTOR_SA10			0x00030000		// 64 KBytes
#define SECTOR_SA11			0x00040000		// 64 KBytes
#define SECTOR_SA12			0x00050000		// 64 KBytes
#define SECTOR_SA13			0x00060000		// 64 KBytes
#define SECTOR_SA14			0x00070000		// 64 KBytes
#define SECTOR_SA15			0x00080000		// 64 KBytes
#define SECTOR_SA16			0x00090000		// 64 KBytes
#define SECTOR_SA17			0x000A0000		// 64 KBytes
#define SECTOR_SA18			0x000B0000		// 64 KBytes
#define SECTOR_SA19			0x000C0000		// 64 KBytes
#define SECTOR_SA20			0x000D0000		// 64 KBytes
#define SECTOR_SA21			0x000E0000		// 64 KBytes
#define SECTOR_SA22			0x000F0000		// 64 KBytes
#define SECTOR_SA23			0x00100000		// 64 KBytes
#define SECTOR_SA24			0x00110000		// 64 KBytes
#define SECTOR_SA25			0x00120000		// 64 KBytes
#define SECTOR_SA26			0x00130000		// 64 KBytes
#define SECTOR_SA27			0x00140000		// 64 KBytes
#define SECTOR_SA28			0x00150000		// 64 KBytes
#define SECTOR_SA29			0x00160000		// 64 KBytes
#define SECTOR_SA30			0x00170000		// 64 KBytes
#define SECTOR_SA31			0x00180000		// 64 KBytes
#define SECTOR_SA32			0x00190000		// 64 KBytes
#define SECTOR_SA33			0x001A0000		// 64 KBytes
#define SECTOR_SA34			0x001B0000		// 64 KBytes
#define SECTOR_SA35			0x001C0000		// 64 KBytes
#define SECTOR_SA36			0x001D0000		// 64 KBytes
#define SECTOR_SA37			0x001E0000		// 64 KBytes
#define SECTOR_SA38			0x001F0000		// 64 KBytes
#define SECTOR_SA39			0x00200000		// 64 KBytes
#define SECTOR_SA40			0x00210000		// 64 KBytes
#define SECTOR_SA41			0x00220000		// 64 KBytes
#define SECTOR_SA42			0x00230000		// 64 KBytes
#define SECTOR_SA43			0x00240000		// 64 KBytes
#define SECTOR_SA44			0x00250000		// 64 KBytes
#define SECTOR_SA45			0x00260000		// 64 KBytes
#define SECTOR_SA46			0x00270000		// 64 KBytes
#define SECTOR_SA47			0x00280000		// 64 KBytes
#define SECTOR_SA48			0x00290000		// 64 KBytes
#define SECTOR_SA49			0x002A0000		// 64 KBytes
#define SECTOR_SA50			0x002B0000		// 64 KBytes
#define SECTOR_SA51			0x002C0000		// 64 KBytes
#define SECTOR_SA52			0x002D0000		// 64 KBytes
#define SECTOR_SA53			0x002E0000		// 64 KBytes
#define SECTOR_SA54			0x002F0000		// 64 KBytes
#define SECTOR_SA55			0x00300000		// 64 KBytes
#define SECTOR_SA56			0x00310000		// 64 KBytes
#define SECTOR_SA57			0x00320000		// 64 KBytes
#define SECTOR_SA58			0x00330000		// 64 KBytes
#define SECTOR_SA59			0x00340000		// 64 KBytes
#define SECTOR_SA60			0x00350000		// 64 KBytes
#define SECTOR_SA61			0x00360000		// 64 KBytes
#define SECTOR_SA62			0x00370000		// 64 KBytes
#define SECTOR_SA63			0x00380000		// 64 KBytes
#define SECTOR_SA64			0x00390000		// 64 KBytes   
#define SECTOR_SA65			0x003A0000		// 64 KBytes    
#define SECTOR_SA66			0x003B0000		// 64 KBytes    
#define SECTOR_SA67			0x003C0000		// 64 KBytes	
#define SECTOR_SA68			0x003D0000		// 64 KBytes	
#define SECTOR_SA69			0x003E0000		// 64 KBytes	
#define SECTOR_SA70			0x003F0000		// 64 KBytes	

#define SECTOR_SIZE_8K		0x00002000		// Sector 8 KBytes
#define SECTOR_SIZE_64K		0x00010000		// Sector 64 KBytes

#define EPROM_SIZE			0x00400000		// Sector 64 KBytes
 
/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef enum
{
	e_eraseSectorStatus_Ok,
	e_eraseSectorStatus_Timeout,
	e_eraseSectorStatus_AwaitErase
}e_eraseSectorStatus;

typedef enum
{
	e_EpromWriteStatus_Ok,
	e_EpromWriteStatus_AwaitWrite,
	e_EpromWriteStatus_Timeout
}e_EpromWriteStatus;

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void epromLow_resetEprom(void);
bool epromLow_eraseSector(UINT32 sectorAddress);
e_eraseSectorStatus epromLow_eraseSectorStatus(UINT32 sectorAddress);
bool epromLow_readWordFromEprom (UINT16* pSource, UINT16* pDestination);
bool epromLow_writeWordToEprom(UINT16 wordValue, UINT16* pDestination, bool isBlockingWrite);
e_EpromWriteStatus epromLow_writeWordToEpromStatus(UINT16 wordValue, UINT16* pDestination);

 
#endif // _EPROMLOW 

