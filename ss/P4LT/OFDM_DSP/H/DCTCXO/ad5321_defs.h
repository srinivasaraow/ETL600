#ifndef AD5321_DEFS_H
#define AD5321_DEFS_H

#define F_CORE_KHZ (32768*3)   // core clock frequency in kHz
//#define F_CORE_KHZ   100000   // core clock frequency in kHz
#define F_SCL_KHZ   100        // desired AD5321 serial clock frequency


#ifdef __ECC__
#define Read_MODE2(reg_value)        asm("%0=MODE2 ;"   : "=d" (reg_value))
#define Write_MODE2(reg_value)       asm("MODE2=%0;"   : : "d" (reg_value))
#define Write_TCOUNT(reg_value)      asm("TCOUNT=%0;"  : : "d" (reg_value))
#define Write_TPERIOD(reg_value)     asm("TPERIOD=%0;" : : "d" (reg_value))
#endif

#define AD5321_WR_COMMAND_LENGTH 8
#define AD5321_WR_N_COMMANDS         8
#define AD5321_WR_BUF_LENGTH       (AD5321_WR_N_COMMANDS*AD5321_WR_COMMAND_LENGTH)

#define AD5321_SCL_DIR_CONTROL FLG4O
#define AD5321_SCL_VAL_CONTROL  FLG4
#define AD5321_SDA_DIR_CONTROL FLG5O
#define AD5321_SDA_VAL_CONTROL  FLG5

#define AD5321_WR_BYTE  0x80000000
#define AD5321_RD_BYTE  0x40000000
#define AD5321_WR_STOP  0x20000000
#define AD5321_WR_START 0x10000000
#define AD5321_RD_ACK   0x08000000
#define AD5321_WR_ACK   0x04000000

#define AD5321_NORMAL_MODE   0  // normal operation
#define AD5321_PD1_MODE      1  // 1.0 kOhm load to ground
#define AD5321_PD100_MODE    2  // 100.0 kOhm load to ground
#define AD5321_3STATE_MODE   3  // three state output

#define AD5321_DESCRIPTOR (AD5321_WR_BYTE | AD5321_RD_BYTE | AD5321_WR_STOP | AD5321_WR_START | AD5321_RD_ACK | AD5321_WR_ACK)

#define VOSC265FREQPPM  7.0f
#define AD5321_MAX_PPM  VOSC265FREQPPM
#define VOSC265VCODE    1143
#define AD5321_MIN_CODE VOSC265VCODE

#define VOSC065FREQPPM -7.0f
#define AD5321_MIN_PPM  VOSC065FREQPPM
#define VOSC065VCODE    3010
#define AD5321_MAX_CODE VOSC065VCODE

//#define VOSC165VCODE (VOSC265VCODE+(VOSC065VCODE-VOSC265VCODE)/2)
#define VOSC165VCODE 2047 // 0x7FF
#define AD5321_CODE_RANGE (AD5321_MAX_CODE-AD5321_MIN_CODE+1)

#endif
