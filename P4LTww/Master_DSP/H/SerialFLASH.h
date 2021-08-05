/*-------------------------------------------------------------------------*
 * $Workfile: SerialFLASH.h $
 * Part of	: NSD570
 * Language	: C
 * Created by	:  M. Buhl
 * Remarks	:  
 * Purpose	: Command Defines for a AT45DB04B serial FLASH from Atmel.
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/SerialFLASH.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 3     13.05.04 11:52 Buhl01
 * 
 * 2     11.05.04 15:24 Buhl01
 *-------------------------------------------------------------------------*/

#ifndef SERIAL_FLASH_
#define SERIAL_FLASH_


// SerialFlash Bisy Pin at (PB 4)
#define FLASH_BISY_STATUS 0x10
// SerialFlash Write Protect Boot Sector
#define FLASH_WRITE_PROTECT_BOOT_SECT  0x20

#define P4LT_SPI_MODE_0_3

#ifdef P4LT_SPI_MODE_0_3
	// buffer 1 read
	#define BUFFER_1_READ 0xD4
	// buffer 2 read
	#define BUFFER_2_READ 0xD6
	// DataFlash status register for reading density, compare status,
	// and ready/busy status
	#define STATUS_REGISTER 0xD7
	// main memory page read
	#define MAIN_MEMORY_PAGE_READ 0xD2
	// Continous Array Read
	#define CONTINOUS_ARRY_READ 0xE8

#else						// use this for EZ-KIT with AT45DB11
	// buffer 1 read
	#define BUFFER_1_READ 0x54
	// buffer 2 read
	#define BUFFER_2_READ 0x56
	// DataFlash status register for reading density, compare status,
	// and ready/busy status
	#define STATUS_REGISTER 0x57
	// main memory page read
	#define MAIN_MEMORY_PAGE_READ 0x52
	// Continous Array Read
	#define CONTINOUS_ARRY_READ 0x68
#endif

// defines for all opcodes
// buffer 1 write
#define BUFFER_1_WRITE 0x84
// buffer 2 write
#define BUFFER_2_WRITE 0x87

// buffer 1 to main memory page program with built-in erase
#define B1_TO_MM_PAGE_PROG_WITH_ERASE 0x83
// buffer 2 to main memory page program with built-in erase
#define B2_TO_MM_PAGE_PROG_WITH_ERASE 0x86

// buffer 1 to main memory page program without built-in erase
#define B1_TO_MM_PAGE_PROG_WITHOUT_ERASE 0x88
// buffer 2 to main memory page program without built-in erase
#define B2_TO_MM_PAGE_PROG_WITHOUT_ERASE 0x89

// main memory page program through buffer 1
#define MM_PAGE_PROG_THROUGH_B1 0x82
// main memory page program through buffer 2
#define MM_PAGE_PROG_THROUGH_B2 0x85

// auto page rewrite through buffer 1
#define AUTO_PAGE_REWRITE_THROUGH_B1 0x58
// auto page rewrite through buffer 2
#define AUTO_PAGE_REWRITE_THROUGH_B2 0x59

// main memory page compare to buffer 1
#define MM_PAGE_TO_B1_COMP 0x60
// main memory page compare to buffer 2
#define MM_PAGE_TO_B2_COMP 0x61

// main memory page to buffer 1 transfer
#define MM_PAGE_TO_B1_XFER 0x53
// main memory page to buffer 2 transfer
#define MM_PAGE_TO_B2_XFER 0x55

// erase a 264 byte page
#define PAGE_ERASE 0x81

// erase 512 pages !!!!!! Checken; Block = 8 Pages
#define BLOCK_ERASE 0x50

#define NUMBER_OF_PAGES	2048

#define NUMBER_OF_BLOCKS 256

#define SIZE_OF_TESTPAGE 250


#endif
