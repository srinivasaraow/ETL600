/*-------------------------------------------------------------------------*
 * $Workfile: rtc_M41ST95.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : Idle counter functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/rtc_M41ST95.h $ 
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 3     18.10.04 17:47 Buhl01
 * more user RAM defines
 * 
 * 2     13.05.04 15:38 Buhl01
 *-------------------------------------------------------------------------*/


#ifndef RTC_M41ST95_H
#define RTC_M41ST95_H

#define READ_RTC_REG_Sec0			0x0000
#define READ_RTC_REG_Sec1			0x0001
#define READ_RTC_REG_Min			0x0002
#define READ_RTC_REG_Hour			0x0003
#define READ_RTC_REG_Day_of_Week	0x0004
#define READ_RTC_REG_Date			0x0005
#define READ_RTC_REG_Month			0x0006
#define READ_RTC_REG_Year			0x0007
#define READ_RTC_REG_Control		0x0008
#define READ_RTC_REG_WatchDog		0x0009
#define READ_RTC_REG_AlarmMonth		0x000A
#define READ_RTC_REG_AlarmDate		0x000B
#define READ_RTC_REG_AlarmHour		0x000C
#define READ_RTC_REG_AlarmMin		0x000D
#define READ_RTC_REG_AlarmSec		0x000E
#define READ_RTC_REG_Flags			0x000F
#define READ_RTC_REG_Reserved0		0x0010
#define READ_RTC_REG_Reserved1		0x0011
#define READ_RTC_REG_Reserved2		0x0012
#define READ_RTC_REG_SQW			0x0013
#define READ_RTC_REG_USER_RAM_0		0x0014	// Used for RTC Magic Byte
#define READ_RTC_REG_USER_RAM_1		0x0015
#define READ_RTC_REG_USER_RAM_2		0x0016
#define READ_RTC_REG_USER_RAM_3		0x0017	// Used for RTC Magic Byte
#define READ_RTC_REG_USER_RAM_4		0x0018
#define READ_RTC_REG_USER_RAM_5		0x0019
#define READ_RTC_REG_USER_RAM_6		0x0020	// Used for RTC Magic Byte
#define READ_RTC_REG_USER_RAM_7		0x0021
#define READ_RTC_REG_USER_RAM_8		0x0022
#define READ_RTC_REG_USER_RAM_9		0x0023	// Used for RTC Magic Byte
#define READ_RTC_REG_USER_RAM_10	0x0024	
#define READ_RTC_REG_USER_RAM_11	0x0025
#define READ_RTC_REG_USER_RAM_12	0x0026
#define READ_RTC_REG_USER_RAM_13	0x0027
#define READ_RTC_REG_USER_RAM_14	0x0028
#define READ_RTC_REG_USER_RAM_15	0x0029
//// from Address 0x0014 to 0x003F is read USER RAM - 44 Bytes
#define READ_RTC_REG_USER_RAM_43	0x003F

////// WRITE ADDRRESS = READ ADDRESS except MSB (8. Bit) = 1 for WRITE; MSB = 0 for READ
#define WRITE_RTC_REG_Sec0			0x0080
#define WRITE_RTC_REG_Sec1			0x0081
#define WRITE_RTC_REG_Min			0x0082
#define WRITE_RTC_REG_Hour			0x0083
#define WRITE_RTC_REG_Day_of_Week	0x0084
#define WRITE_RTC_REG_Date			0x0085
#define WRITE_RTC_REG_Month			0x0086
#define WRITE_RTC_REG_Year			0x0087
#define WRITE_RTC_REG_Control		0x0088
#define WRITE_RTC_REG_WatchDog		0x0089
#define WRITE_RTC_REG_AlarmMonth	0x008A
#define WRITE_RTC_REG_AlarmDate		0x008B
#define WRITE_RTC_REG_AlarmHour		0x008C
#define WRITE_RTC_REG_AlarmMin		0x008D
#define WRITE_RTC_REG_AlarmSec		0x008E
#define WRITE_RTC_REG_Flags			0x008F
#define WRITE_RTC_REG_Reserved0		0x0090
#define WRITE_RTC_REG_Reserved1		0x0091
#define WRITE_RTC_REG_Reserved2		0x0092
#define WRITE_RTC_REG_SQW			0x0093
#define WRITE_RTC_REG_USER_RAM_0	0x0094	// Used for RTC Magic Byte
#define WRITE_RTC_REG_USER_RAM_1	0x0095
#define WRITE_RTC_REG_USER_RAM_2	0x0096
#define WRITE_RTC_REG_USER_RAM_3	0x0097	// Used for RTC Magic Byte
#define WRITE_RTC_REG_USER_RAM_4	0x0098
#define WRITE_RTC_REG_USER_RAM_5	0x0099
#define WRITE_RTC_REG_USER_RAM_6	0x00A0	// Used for RTC Magic Byte
#define WRITE_RTC_REG_USER_RAM_7	0x00A1
#define WRITE_RTC_REG_USER_RAM_8	0x00A2
#define WRITE_RTC_REG_USER_RAM_9	0x00A3	// Used for RTC Magic Byte
#define WRITE_RTC_REG_USER_RAM_10	0x00A4
#define WRITE_RTC_REG_USER_RAM_11	0x00A5
#define WRITE_RTC_REG_USER_RAM_12	0x00A6
#define WRITE_RTC_REG_USER_RAM_13	0x00A7
#define WRITE_RTC_REG_USER_RAM_14	0x00A8
#define WRITE_RTC_REG_USER_RAM_15	0x00A9




//// from Address 0x0094 to 0x00BF is write USER RAM - 44 Bytes
#define WRITE_RTC_REG_USER_RAM_43	0x00BF






#endif //#ifndef RTC_M41ST95_H

