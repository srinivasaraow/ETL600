/*-------------------------------------------------------------------------*
 * $Workfile: dsp_micro_common.h $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	: This is a common file for the DSPs and the AVR microcontroller. Be carefully with changes!!!
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Microcontroller/H/dsp_micro_common.h $
 * 
 * 2     29.06.05 15:14 Chmibuh
 * 
 * 1     19.01.05 14:48 Chchlee
 * 
 * 4     3.11.04 9:14 Buhl01
 * changed default UART mode to 8E1
 * 
 * 3     22.10.04 16:44 Buhl01
 * UART_DEFAULT added
 * 
 * 2     22.10.04 16:27 Buhl01
 * 
 * 1     22.10.04 16:24 Buhl01
 *-------------------------------------------------------------------------*/

#ifndef DSP_MICRO_COMMON_H
#define DSP_MICRO_COMMON_H

// This task defines are 0x_F and this is not a valid baudrate! See the 4 LSB of baudrate defines. 
#define INIT_SET_UART_BAUDRATE					0x1F
#define CONFIRM_SET_UART_BAUDRATE				0x2F
#define RETURN_RECV_UART_SETTINGS				0x3F

#define	SEND_DATA_INIT_READ_VOLTAGE				0x4F	
#define SEND_DATA_READ_BASE_VOLTAGE				0x5F
#define SEND_DATA_READ_GENERATED_VOLTAGE		0x6F
#define DSP_CONFIRMED_STRAT_UP					0x7F

// All this Baudrates are implemented on microcontroller
#define UART_2400_BD							0x01
#define UART_4800_BD							0x02
#define UART_9600_BD							0x03
#define UART_14400_BD							0x04
#define UART_19200_BD							0x05
#define UART_28800_BD							0x06
#define UART_38400_BD							0x07
#define UART_57600_BD							0x08	// <--  Default
#define UART_76800_BD							0x09
#define UART_115200_BD							0x0A	// do not use this baudrate because of the limited transfer speed of uP <- SPI -> DSP

// All this modes are implemented on microcontroller
#define UART_MODE_8N1							0x10
#define UART_MODE_8E1							0x20	// <--  Default
#define UART_MODE_8O1							0x30
#define UART_MODE_8N2							0x40
#define UART_MODE_8E2							0x50
#define UART_MODE_8O2							0x60

#define CASE_2400_BD							2400
#define CASE_4800_BD							4800
#define CASE_9600_BD							9600
#define CASE_14400_BD							14400
#define CASE_19200_BD							19200
#define CASE_28800_BD							28800
#define CASE_38400_BD							38400
#define CASE_57600_BD							57600	// <--  Default
#define CASE_76800_BD							76800
#define CASE_115200_BD						115200


#define FIRST_BYTE_DSP_BOOT_OK        0x01
#define SECOND_BYTE_DSP_BOOT_OK       0x02
#define THIRD_BYTE_DSP_BOOT_OK        0x03
#define FOURTH_BYTE_DSP_BOOT_OK       0x04

#define FIRST_BYTE_DSP_BOOT_FAILD     0x04
#define SECOND_BYTE_DSP_BOOT_FAILD    0x03
#define THIRD_BYTE_DSP_BOOT_FAILD     0x02
#define FOURTH_BYTE_DSP_BOOT_FAILD    0x01


#endif  // DSP_MICRO_COMMON_H

