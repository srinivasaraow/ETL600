/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vsorti_f_DDD.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : sorting of an input float array in descending order
 * OPERATION   : 
 * PROTOTYPE   : 
 *                                   
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsorti_f_DDD.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     26.02.04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r15 = r15 - r15 instead f15 = f15 - f15 which can produce NAN
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>

.global _vsort_f_DD;
.global _vsort_f_ED;
.global _vsort_f_DE;
.global _vsort_f_EE;

.global _vsorti_f_DDD;
.global _vsorti_f_DDE;
.global _vsorti_f_DED;
.global _vsorti_f_DEE;
.global _vsorti_f_EDD;
.global _vsorti_f_EDE;
.global _vsorti_f_EED;
.global _vsorti_f_EEE;

.global _vsortii_f_DDDD;
.global _vsortii_f_DDDE;
.global _vsortii_f_DDED;
.global _vsortii_f_DDEE;
.global _vsortii_f_DEDD;
.global _vsortii_f_DEDE;
.global _vsortii_f_DEED;
.global _vsortii_f_DEEE;
.global _vsortii_f_EDDD;
.global _vsortii_f_EDDE;
.global _vsortii_f_EDED;
.global _vsortii_f_EDEE;
.global _vsortii_f_EEDD;
.global _vsortii_f_EEDE;
.global _vsortii_f_EEED;
.global _vsortii_f_EEEE;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vsortii_f_DDDD(float R[], int SIndX[], float X[], int VIndX[], int L );
*
* Input:
*      X[] - vector of floats to sort; X[0..LEN-1]
*  VIndX[] - virtual indecies or properties of X[]; VIndX[0..LEN-1]
*      L   - length of vector X[]
* Output:
*      R[] - sorted in descending order vector X[]
*  SIndX[] - permuted according to weght of X[] vector VIndX[]
*****************************************************/
_vsortii_f_DDDD:
_vsortii_f_DDDE:
_vsortii_f_DDED:
_vsortii_f_DDEE:
_vsortii_f_DEDD:
_vsortii_f_DEDE:
_vsortii_f_DEED:
_vsortii_f_DEEE:
_vsortii_f_EDDD:
_vsortii_f_EDDE:
_vsortii_f_EDED:
_vsortii_f_EDEE:
_vsortii_f_EEDD:
_vsortii_f_EEDE:
_vsortii_f_EEED:
_vsortii_f_EEEE:

        leaf_entry;
        r0 = i0; puts = r0;
        r0 = i1; puts = r0;
        r0 = i2; puts = r0;
        r0 = i3; puts = r0;
        puts = r1;
        puts = r3;
        puts = r10;
        puts = r11;
        puts = r13;
        puts = r14;
        puts = r15;
        
        i0 = r4;                // address of vector R[]
        i2 = r8;                // address of vector SIdX[]
        i1 = r12;               // address of vector X[]
        i3 = reads(1);          // address of vector IndX[]
        r14 = reads(2);         // LEN
        
        lcntr = r14, do cp_lp1 until lce;
          r0 = dm(i1,m6);
          dm(i0,m6) = r0;
          r0 = dm(i3,m6);
cp_lp1:   dm(i2,m6) = r0;          
        
        r14 = reads(2);         // LEN
                
        jump sorti;
        
/*****************************************************
*
* void vsorti_f_DDD(float R[], int SIndX[], float X[], int L );
*
* Input:
*      X[] - vector of floats to sort X[0..LEN-1]
*      L   - length of vector X[]
* Output:
*      R[] - sorted in descending order vector X[]
*     SInd[] - vector of permutations
************************************************/
_vsorti_f_DDD:
_vsorti_f_DDE:
_vsorti_f_DED:
_vsorti_f_DEE:
_vsorti_f_EDD:
_vsorti_f_EDE:
_vsorti_f_EED:
_vsorti_f_EEE:

        leaf_entry;
        
        r0 = i0; puts = r0;
        r0 = i1; puts = r0;
        r0 = i2; puts = r0;
        r0 = i3; puts = r0;
        puts = r1;
        puts = r3;
        puts = r10;
        puts = r11;
        puts = r13;
        puts = r14;
        puts = r15;
        
        i0 = r4;                  // address of output vector R[] of sorted X[]
        i1 = r8;                  // address of output vector SIndX[] of idecies of sorted X[]
        r1 = r1 - r1, i2 = r12;   // address of input float vector X[], outer loop
        r14 = reads(1);           // LEN
        lcntr = r14, do cp_lp2 until lce;
          r0 = dm(i2,m6);
          dm(i1,m6) = r1;
cp_lp2:   r1 = r1 + 1, dm(i0,m6) = r0;          

sorti:
        f3 = 1.0;
        r15 = r15 - r15, i0 = r4;   // address of output vector R[] of sorted X[]
        f0 = -f3, i1 = r8;          // address of output vector SIndX[] of idecies of sorted X[]
           
        r12 = r4 + 1, r13 = r8;
        r14 = r14 - 1, r10 = r4;    // r14 = len-1
        
        lcntr = r14, do outer_lp until lce;
           f0 = f0 + f3, i3 = r12;        // i_xmax = i
           f11 = f0 + f3, f8 = dm(i0,m5); 
           f1 = f0 + f15, f4 = dm(i3,m6);
           lcntr = r14, do inner_lp until lce;
              comp(f8,f4);
              if lt  f8 = f3*f4, f1 = f11+f15;
inner_lp:     f11 = f11 + f3, f4 = dm(i3,m6);
           comp(f0,f1);
           if eq jump outer_lp1;  
             r1 = fix f1;
             r11 = r10 + r1, f4 = dm(i0,m5);
             i3 = r11;
             dm(i0,m5) = f8;
             r4 = r13 + r1, dm(i3,m5) = f4;
             i3 = r4;
             r4 = dm(i1,m5);
             r8 = dm(i3,m5);
             dm(i1,m5) = r8;
             dm(i3,m5) = r4; 
outer_lp1: r12 = r12 + 1, modify(i1,m6);
outer_lp:  r14 = r14 - 1, modify(i0,m6);

        i0  = gets( 11 );
        i1  = gets( 10 );
        i2  = gets( 9 );
        i3  = gets( 8 );
        r1  = gets( 7 );
        r3  = gets( 6 );
        r10 = gets( 5 );
        r11 = gets( 4 );
        r13 = gets( 3 );
        r14 = gets( 2 );
        r15 = gets( 1 );

        leaf_exit;
        
_vsorti_f_DDD.end:
_vsorti_f_DDE.end:
_vsorti_f_DED.end:
_vsorti_f_DEE.end:
_vsorti_f_EDD.end:
_vsorti_f_EDE.end:
_vsorti_f_EED.end:
_vsorti_f_EEE.end:

_vsortii_f_DDDD.end:
_vsortii_f_DDDE.end:
_vsortii_f_DDED.end:
_vsortii_f_DDEE.end:
_vsortii_f_DEDD.end:
_vsortii_f_DEDE.end:
_vsortii_f_DEED.end:
_vsortii_f_DEEE.end:
_vsortii_f_EDDD.end:
_vsortii_f_EDDE.end:
_vsortii_f_EDED.end:
_vsortii_f_EDEE.end:
_vsortii_f_EEDD.end:
_vsortii_f_EEDE.end:
_vsortii_f_EEED.end:
_vsortii_f_EEEE.end:

/*****************************************************
*
* void vsort_f_DDD(float R[], float X[], int L );
*
* Input:
*      X[] - vector of floats to sort X[0..LEN-1]
*      L   - length of vector X[]
* Output:
*      R[] - sorted in descending order vector X[]
************************************************/
_vsort_f_DD:
_vsort_f_ED:
_vsort_f_DE:
_vsort_f_EE:

        leaf_entry;

        r0 = i0; puts = r0;
        r0 = i1; puts = r0;
        puts = r1;
        puts = r3;
        puts = r10;
        puts = r11;
        puts = r14;
        puts = r15;
        
        i0 = r4;
        i1 = r8;
        lcntr = r12, do cp_lp3 until lce;
          r0 = dm(i1,m6);
cp_lp3:   dm(i0,m6) = r0;          

        f3 = 1.0;                   // f3 = 1.0
        r15 = r15 - r15, i0 = r4;   // i0 = &r[0], f15 = 0
        f0 = -f3, r10 = r4;         // f0 = -1.0, r10 = &r[0]
           
        r14 = r4 + 1;               // r14 = &r[1]
        r12 = r12 - 1;              // r14 = len-1
        
        lcntr = r12, do outer_lp2 until lce;
           f0 = f0 + f3, i1 = r14;        
           f11 = f0 + f3, f8 = dm(i0,m5); 
           f1 = f0 + f15, f4 = dm(i1,m6);
           lcntr = r12, do inner_lp1 until lce;
              comp(f8,f4);
              if lt  f8 = f3*f4, f1 = f11+f15;
inner_lp1:    f11 = f11 + f3, f4 = dm(i1,m6);
           comp(f0,f1);
           if eq jump lb2;  
             r1 = fix f1, f4 = dm(i0,m5);
             r11 = r10 + r1, dm(i0,m5) = f8;
             i3 = r11;
             dm(i3,m5) = f4;
lb2:       r14 = r14 + 1, modify(i1,m6);
outer_lp2: r12 = r12 - 1, modify(i0,m6);


        i0  = gets(8);
        i1  = gets(7);
        r1  = gets(6);
        r3  = gets(5);
        r10 = gets(4);
        r11 = gets(3);
        r14 = gets(2);
        r15 = gets(1);

        leaf_exit;
                        
_vsort_f_DD.end:
_vsort_f_ED.end:
_vsort_f_DE.end:
_vsort_f_EE.end:


.endseg;
