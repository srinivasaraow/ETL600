/*  This file contains the global declarations for the
    signal.h header file and its associated routines*/

#include <signal.h>

/* These three are "cast" in signal.h */
#undef SIG_DFL
#undef SIG_ERR
#undef SIG_IGN

#define SIG_DFL 0x01
#define SIG_ERR 0x02
#define SIG_IGN 0x03


#define next_mask 0
#define next_func 1
#define curr_func 2
#define defl_mask 3
#define defl_func 4
#define defl_disp 5
#define siga_size 6	/* Size of signal entry */

/* Accessor macros for various fields of the interrupt table */
#define SIGMASK(I)	___lib_int_table+(I*siga_size)
#define SIGCFUNC(I)	___lib_int_table+(I*siga_size)+curr_func
#define SIGDISP(I)	___lib_int_table+(I*siga_size)+defl_disp

