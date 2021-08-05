/*-------------------------------------------------------------------------*
 * $Workfile: Status.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/Status.c $ 
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 3     27.12.04 13:11 Chchlee
 * status_getPTestSwitches added
 * 
 * 2     15.11.04 8:05 Leeb02
 * 
 * 1     15.11.04 7:33 Leeb02
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "diag.h"
#include "status.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/


static t_P4LT_Status        g_StatusData;
static t_FastStatusTelegram g_LocalFastStatusData [N_APLC_CHANNELS];
static t_FastStatusTelegram g_RemoteFastStatusData[N_APLC_CHANNELS];
static t_TestPlcLinkData    g_TestSwitches;

static BOOL bInitCalled = FALSE;

/* LOCAL FUNCTIONS ***********************************************************/


/* IMPLEMENTATION ************************************************************/

void SEGMENT_EXT_CODE status_init(void)
{

	bInitCalled = TRUE; // allow getter function to access status

}

t_P4LT_Status * status_getPStatus(void)
{
	ASSERT(bInitCalled);
	return &g_StatusData;
}

t_FastStatusTelegram * status_getLocalFastStatus(void)
{
	ASSERT(bInitCalled);
	return g_LocalFastStatusData;
}

t_FastStatusTelegram * status_getRemoteFastStatus(void)
{
	ASSERT(bInitCalled);
	return g_RemoteFastStatusData;
}

t_TestPlcLinkData    * status_getPTestSwitches(void)
{
	ASSERT(bInitCalled);
	return &g_TestSwitches;
}
