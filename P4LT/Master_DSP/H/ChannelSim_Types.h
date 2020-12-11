/*-------------------------------------------------------------------------*
 * $Workfile: ChannelSim_Types.h $
 * Part of	: ETL600 / RF channel simulator GUI
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	:	Defines RF channel simulator types used by target (P4LT)
 *				and HMI600
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/HMI600/H/ChannelSim_Types.h $
 * 
 * 3     7.01.05 17:19 Chhesch5
 * 
 * 2     12.11.04 8:09 Schnyd01
 * 
 * 1     12.11.04 8:03 Schnyd01
 * initial checkin
 * 
 *-------------------------------------------------------------------------*/


#ifndef	_ChannelSim_Types
#define	_ChannelSim_Types

// RF channel simulator parameter number and value data type 
// used for RF channel simulator (CS) get (CS_GET) and set (CS_SET) services
// - CS_GET used for data received from CS
// - CS_SET	used for data sent to CS
typedef struct {
	long	lparNumber;		// parameter number
	float	fparValue;		// parameter value
} t_ChannelSimGetSetData;



#endif                

