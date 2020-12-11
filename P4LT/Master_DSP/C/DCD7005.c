/*-------------------------------------------------------------------------*
 * $Workfile: DCD7005.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/DCD7005.c $
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 2     21.07.04 19:10 Buhl01
 * 
 * 1     21.07.04 14:57 Buhl01
 *-------------------------------------------------------------------------*/

#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "spi_dsp.h"
#include "SPI_access.h"
#include "DCD7005.h"

void setDCD7005(void)
{
	sendSPIbyte(REG0_CDC7005, CS_SPI_7005, SPI_SETTINGS_FOR_7005);
	sendSPIbyte(REG1_CDC7005, CS_SPI_7005, SPI_SETTINGS_FOR_7005);
	sendSPIbyte(REG2_CDC7005, CS_SPI_7005, SPI_SETTINGS_FOR_7005);
}
