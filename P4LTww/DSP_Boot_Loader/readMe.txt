Some remarks about the bootloader for ADSP-21161N from Analog Devices (AD):


By default, the ADSP-2106x SHARC processor is configured to load 256 words of 48-bit width (instruction size) after reset by DMA. 
Since user application code is far larger than 256 words, a boot kernel ensures that the complete user application code and data 
is loaded into the internal memory spaces of the SHARC. Additionally, external memories and data ports are initialized by the kernel.

Following bott kernels/loader are used in the ETL600 project:


FLASH_Boot_SDRAM: Used by the Master DSP, gets application code from parallel FLASH, default loader kernel for prom booting from AD (161_prom.asm) added with SDRAM settings.	


SPI_Boot_SDRAM:   Used by all Slave DSPs with external SDRAM - NSD, NSK, OFDM and AFE DSP, gets application code via SPI from Master DSP. Default loader kernel for spi booting from AD (161_spi.asm) added with SDRAM settings and adapted to avoid the "boot problem".	


SPI_Boot:	  Used by Upconverter DSP, gets application code via SPI from Master DSP. Default loader kernel for spi booting from AD (161_spi.asm). 


EMERGENCY_SDRAM:  Used by Master DSP. It's an enhanced version of FLASH_Boot_SDRAM. It's used to generate a multi-processor FW - Master DSP application + Emergency FW. It polls a input flag to detect which FW has to load into Master DSP (normal application or Emergency FW)


All boot loader are VDSP 3.5 projects!!! VDSP = Visual DSP = IDE = Integrated Development Environment from AD. Do not mistake with Microsoft Visual Studio. *.dpj are the project files. 


07.12.2004/M.BUHL