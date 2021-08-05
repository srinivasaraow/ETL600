/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Trellis_34_ViterbiDecoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM Metrics calculation & Trellis 3/4 Viterbi decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Trellis_34_ViterbiDecoder(int d2alloc[],int len2d, int *Tmp_ptr);  
 *
 * Tmp_ptr - points temporary vector of length 9*LEN_4D_MAX_RX
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * Initial version
 * 1     13.09.2003  vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Memory usage reduction
 *       17.01.2004 vgp
 *       i6 DAG REGISTER DOESN'T WORK WITH EXTERNAL MEMORY
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 * Correction of SW addressing mode for temporary array
 *       22.01.2004 vgp
 * No i6 usage 
 *       27.02.04 vgp
 * Dynamic temporary memory allocation for TCM decoding according to current len2d
 *       04.03.04 vgp
 * Commands like Fn = Fn - Fn are substituted by Rn = Rn - Rn to avoid possible NANs
 *       08.05.04 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Trellis_34_ViterbiDecoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:56 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 */

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>
#include   "../../h/tcm/TCM_Defs.h"
#define NB 16

.SEGMENT/pm	seg_VitMet;
.var Met2[16];
.var Met1[16];
.ENDSEG;


.SEGMENT/DM	seg_dmda;

.extern _ETF_Ptr;	    // pointer of estimated channel transfer function of length len2d
.extern _Sig_RX_Ptr;    // pointer of the received QAM vector of length len2d

.global _gr_koor_x_ptr;  // decoded X coordinates of quartet's centre of length LEN_2D_MAX_RX
.global _gr_koor_y_ptr;  // decoded Y coordinates of quartet's centre of length LEN_2D_MAX_RX
.global _wb_sig_ptr;     // points array of length 2*LEN_4D_MAX_RX
.global _wb_st_ptr;      // points array of length 2*LEN_4D_MAX_RX
.global _num_abcd_ptr;   // points array of length LEN_4D_MAX_RX

.align 2;
.var DistModif[18] =  4,  4,  -4, -4,  4,  4,  -4,  -4,
                      4,  4,  -4, -4,  4,  4,  -4,  -4,
			          0, 0;  
.var StepMet1Off[18] = Met1,    Met1+8,  Met1+4,  Met1+12,  
                       Met1+1,  Met1+9,  Met1+5,  Met1+13,
                       Met1+10, Met1+2,  Met1+14, Met1+6,  
                       Met1+11, Met1+3,  Met1+15, Met1+7,
                       0, 0;
.var StepMet2Off[18] = Met2,    Met2+8,  Met2+4,  Met2+12,  
                       Met2+1,  Met2+9,  Met2+5,  Met2+13,
                       Met2+10, Met2+2,  Met2+14, Met2+6,  
                       Met2+11, Met2+3,  Met2+15, Met2+7,
                       0, 0;
.var Ptr_CurStepMetOff = StepMet1Off;
.var Ptr_NewStepMetOff = StepMet2Off;
.var SaveReg[7];
.var Sig[16];
.var State[16];
.var PtrWb_Sig;
.var PtrWb_State;
.var _gr_koor_x_ptr;  // decoded X coordinates of quartet's centre of length LEN_2D_MAX_RX
.var _gr_koor_y_ptr;  // decoded Y coordinates of quartet's centre of length LEN_2D_MAX_RX
.var _wb_sig_ptr;     // points array of length 2*LEN_4D_MAX_RX
.var _wb_st_ptr;      // points array of length 2*LEN_4D_MAX_RX
.var _num_abcd_ptr;   // points array of length LEN_4D_MAX_RX

.ENDSEG;

.SEGMENT/PM	seg_pmda;
.var TBL_JUMP1[4]=case_0,case_1,case_2,case_3;      //metrics calc. jump table for even i_2d
.var TBL_JUMP2[4]=case_00,case_10,case_20,case_30;  // metrics calc. jump table for odd i_2d
.var     s_f_z[7] = 0.0, 1.0, 3.0, 7.0, 15.0, 31.0, 63.0; // shift fron zero coordinates table upto 16384QAM
.var WAY_table[16]= 0., 5., 10., 15., 1., 4., 11., 14., 
                    2., 7.,  9., 12., 3., 6.,  8., 13.;	
.var Ptr_CurMet = Met1;                             
.var Ptr_NewMet = Met2;
.var States[64] =  0.,  4.,  8., 12.,   8., 12.,  0.,  4.,
                   4.,  0., 12.,  8.,  12.,  8.,  4.,  0.,
                   1.,  5.,  9., 13.,   9., 13.,  1.,  5.,
                   5.,  1., 13.,  9.,  13.,  9.,  5.,  1.,
                  10., 14.,  2.,  6.,   2.,  6., 10., 14.,
                  14., 10.,  6.,  2.,   6.,  2., 14., 10.,
                  11., 15.,  3.,  7.,   3.,  7., 11., 15.,
                  15., 11.,  7.,  3.,   7.,  3., 15., 11.;

.var InDistPtr[18] = InDist,   InDist,   InDist,   InDist,
                     InDist+8, InDist+8, InDist+8, InDist+8,
                     InDist,   InDist,   InDist,   InDist,
                     InDist+8, InDist+8, InDist+8, InDist+8, 0, 0;
.endseg;

.segment/dm	seg_InDistVit;
.var InDist[16];
.endseg;


.SEGMENT/PM		seg_pmco;

.global _TCM_Trellis_34_ViterbiDecoder;

.ENDSEG;

_TCM_Trellis_34_ViterbiDecoder:
      entry;

      r0 = i15; puts = r0;
      puts=  r2;
      puts = r3;
      puts = r5;
      puts = r6;
      puts = r7;
      puts=  r8;
      puts = r9;
      puts = r10;
      puts = r11;
      puts  =r12;
      puts = r13;
      puts = r14;
      puts = r15;
      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i3; puts = r0;
      r0 = i4; puts = r0;
      r0 = i5; puts = r0;
      r0 = i6; puts = r0;
      r0 = i8; puts = r0;
      r0 = i9; puts = r0;
      r0 = i10; puts = r0;
      r0 = i11; puts = r0;
      r0 = i12; puts = r0;
      r0 = i13; puts = r0;
      r0 = i14; puts = r0;
      r0 = m0; puts = r0;
      r0 = m1; puts = r0;
      r0 = m2; puts = r0;
      r0 = m3; puts = r0;
      r0 = m4; puts = r0;
      r0 = m5; puts = r0;
      r0 = m9; puts = r0;
      r0 = m10; puts = r0;
      r0 = m11; puts = r0;

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;

      i0 = r4;                     // i0 = r4 = d2alloc[] 
      r4 = lshift r8 by -1;        // r4 = len4d = len2d/2
      puts = r4;                   // save r4 on stack
/*      
    gr_koor_x_ptr = Tmp_ptr;     
    gr_koor_y_ptr = Tmp_ptr + len2d    
    wb_sig_ptr    = Tmp_ptr + 2*len2d        
    wb_st_ptr     = Tmp_ptr + 3*len2d         
    num_abcd_ptr  = Tmp_ptr + 4*len2d      
*/
      dm(_gr_koor_x_ptr) = r12;         
      r12 = r12 + r8;
      dm(_gr_koor_y_ptr) = r12;         
      r12 = r12 + r8;
      dm(_wb_sig_ptr) = r12;         
      r12 = r12 + r8;
      dm(_wb_st_ptr) = r12;         
      r12 = r12 + r8;
      dm(_num_abcd_ptr) = r12;         
      

//===================================
      bit set mode1 CBUFEN;
      l14 = 16;

// init pointers to wb_st & wb_sig
      r0 = dm(_wb_st_ptr);
      dm(PtrWb_State) = r0;
      r0 = dm(_wb_sig_ptr);
      dm(PtrWb_Sig) = r0;
// initialize array of accumulated Viterby metrics
      i11 = Met1;
      r14 = r14 - r14;
      r14 = r14 + 1, pm(i11,m14) = r14;   //   r14 = 1
      f0 = 1.0e30;
// clear metrics
      lcntr = 15, do cmet until lce;
cmet:      r13=r13-r13, pm(i11,m14) = f0;    //   r13 = 0
// initialize pointers to the arrays of accumulated Viterby metrics	  
      r0 = Met1;
      pm(Ptr_CurMet) = r0;
      r0 = Met2;
      pm(Ptr_NewMet) = r0;
// initialize pointers to the arrays of address offsets in metrics for Viterby procedure  
      r0 = StepMet1Off;
      pm(Ptr_CurStepMetOff) = r0;
      r0 = StepMet2Off;
      pm(Ptr_NewStepMetOff) = r0;

//Calculate metrices for pairs of signal points on the length 2d
      i1  = dm(_Sig_RX_Ptr); //received signal of complex_float
      i5  = dm(_ETF_Ptr);
      i3  = dm(_gr_koor_x_ptr);
      i4  = dm(_gr_koor_y_ptr);
      i12 = TBL_JUMP1;
      i13 = TBL_JUMP2;

      f6 = float r14;     // f6=1.0 
      f6 = (f6+f13)/2;    // f6=0.5, f13=0.0
      i15 = s_f_z+1;      // address of shift from zero table 

//============  start of main_loop of metrics calculation		
      lcntr = r4, do (pc,main_loop) until lce;   // r4 =len4d

        m10 = dm(i0,m6);                 // m10 = d2alloc[i] 
        f15 = pm(m10,i15);               // f15 = shift_from_zero

        f8 = f15+f15,   f9 = dm(i1,m6);  // f9 = x, f8 = 2*Shift_from_zero
        f9 = f9+f15,	r7 = dm(i1,m6);  // f7 = y, f9=x+shift_from_zero
        f7 = f7+f15;                     // f7 = y+shift_from_zero
        f11 = f7+f6;                     // f11 = center_y = y+shift_from_zero+0.5
        if lt r11=r11-r11;               // if (center_y < 0) center_y = 0
        f10 = f9+f6;			     // f10 = center_x = x+shift_from_zero+0.5
        if lt r10=r10-r10;               // if (center_x < 0) center_x = 0
        comp(f10,f8);
        if gt f10 = f8;
        comp(f11,f8);
        if gt f11 = f8;
        r10 = trunc f10;                 // r10 = center_x
        r11 = trunc f11;                 // r11 = center_y

        r5  = r10 and r14;
        r5  = r5+r5;
        r13 = r11 and r14;
        r5  = r5+r13;   			     // r5 = num_tip
        m11 = r5;

        f13 = float r10;              // f13 = (float)center_x              
        f12 = float r11;              // f12 = (float)center_y

        r1 = pm(m11,i12); 		
        f8 = f9-f13,    f0 = dm(i5,m6);  // f0 = k^2
        f7 = f7-f12,    i8 = r1;			
        f9 = f8+f7, 	f7 = f8-f7;      // f9 = reg0, f7 = reg1	

//--------------------------------------------- 
        jump (m13,i8)(db);
        f9 = f9*f0;                        // f9=reg0 * k^2
        f7 = f7*f0;                        // f7=reg1 * k^2	
//=================================================
//	Figure 19   2D metric computation
//				10	         01
//               .     |      .
//             C       |         B
//                    y|   *
//           ----------|---------
//                     |   x
//             A .     |      .  D
//              00     |      11

//	00 x -odd, y-odd, type ABCD
//2D Metrices are stored in registers F0,F1,F2,F3 and determine type of signal point A,B,C or D
// Gray koordinates of 4 nearest signal points are i3 gr_koor_x and  i4 gr_koor_y
// f9 = reg0, f7 = reg1
// f0 = reg0, f1 = -reg0, f2 = reg1, f3=-reg1
// r14 = 1
// f6 = 0.5
case_0:	
        r3 = 0x01010101;
        r11 = lshift r11 by -1;       //r11= center_x/2
        r10 = lshift r10 by -1;	      //r10= center_y/2, gr_koor_x[(j_gr_koor)]=center_x/2
        r10 = r10 * r3(uui), f0 = f9;
		r11 = r11 * r3(uui), f2 = f7;
        jump case_end (DB);
          f1 = -f9, dm(i4,m6) = r11;
		  f3 = -f7, dm(i3,m6) = r10;

//	01 x -odd, y-even, type CADB , gr_koor_x are the same  gr_koor_y bilongs to different gray points
// f9 = reg0, f7 = reg1
// f0 = reg1, f1 = -reg1, f2 = reg0, f3=-reg0
// r14 = 1
// f6 = 0.5
case_1:	
        r3 = 0x01010101;
        r10 = lshift r10 by -1;             //r10= center_x/2
        r13 = r11+r14, r11 = r11-r14;         //r13= center_y+1 &  gr_koor_x[(j_gr_koor)]=center_x/2
        r10 = r10 * r3(uui), f0 = f7;
        r13 = lshift r13 by -1;	     
        r11 = lshift r11 by -1;	     
		r13 = r13 or fdep r13 by 24:8;
		r3 = 0x10100;
		r11 = r11 * r3(uui), f2 = f9;
		r11 = r11 or r13;
        jump case_end(DB);                               
		  f1 = -f7, dm(i4,m6) = r11;
		  f3 = -f9, dm(i3,m6) = r10;

//	10 x -even, y-odd, type DBCA , gr_koor_y are the same  gr_koor_x bilongs to different gray points		
// f9 = reg0, f7 = reg1
// f0 = -reg1, f1 = reg1, f2 = -reg0, f3=reg0
// r14 = 1
// f6 = 0.5
case_2:	
        r3 = 0x01010101;
        r11 = lshift r11 by -1;        
        r10 = r10+r14, r13 = r10-r14;
        r11 = r11 * r3(uui), f1 = f7;
        r10 = lshift r10 by -1;	
        r13 = lshift r13 by -1;
        r13 = lshift r13 by 8;
        r10 = r10 or r13, f3 = f9;
        r10 = r10 or fdep r10 by 16:16;
        jump case_end(DB);
          f2 = - f9, dm(i4,m6) = r11;
          f0 = - f7, dm(i3,m6) = r10;
        
//	11 x -even, y-even, type BDAC , gr_koor_y and gr_koor_x bilongs to different gray points			
// f9 = reg0, f7 = reg1
// f0 = -reg0, f1 = reg0, f2 = -reg1, f3=reg1
// r14 = 1
// f6 = 0.5
case_3:		
        r5  = r10+r14, r13 = r10-r14;
        r5  = lshift r5 by -1;
        r13 = lshift r13 by -1;	
        r13 = lshift r13 by 8;
        r5 = r5 or r13;
        r5 = r5 or fdep r5 by 16:16;
        r10 = r11+r14, r11 = r11-r14;
        r10 = lshift r10 by -1;		
		r10 = r10 or fdep r10 by 24:8;
        r11 = lshift r11 by -1;
		r3 = 0x10100;
		r11 = r11 * r3(uui);
		r10 = r10 or r11;
        f2 = -f7, dm(i3,m6) = r5;
        f3 = -f2, dm(i4,m6) = r10;			        
        f0 = -f9;		
        f1 = -f0;
		      				
//=========================================================================
// r14 = 1
// f6 = 0.5
case_end:

        m10 = dm(i0,m6);                 // m10 = d2alloc[i] 
        f15 = pm(m10,i15);               // f15 = shift_from_zero

        f8 = f15+f15,   f9 = dm(i1,m6);     //f9=x, f8=2*Shift_from_zero
        f9 = f9+f15,	f7 = dm(i1,m6);		//f7=y
        f7 = f7+f15;
        f11 = f7+f6;                 // f11 = center_y = y+shift_from_zero+0.5
        if lt r11=r11-r11;           // if (center_y < 0) center_y = 0
        f10 = f9+f6;				 // f10 = center_x = x+shift_from_zero+0.5
        if lt r10=r10-r10;           // if (center_x < 0) center_x = 0
        comp(f10,f8);
        if gt f10 = f8;
        comp(f11,f8);
        if gt f11 = f8;
        r10 = trunc f10;             // r10 = center_x
        r11 = trunc f11;             // r11 = center_y

        r5  = r10 and r14;
        r5  = r5+r5;
        r13 = r11 and r14;
        r5  = r5+r13;   			     // r5 = num_tip
        m11 = r5;
        f13 = float r10;
        f12 = float r11;

        r4 = pm(m11,i13);
        f8 = f9-f13,    f6 = dm(i5,m6);  // f6 = k^2
        f7 = f7-f12,    i10=r4;			
        f9 = f8+f7, 	f8 = f8-f7;      // f9 = reg0, f8 = reg1	
		
//--------------------------------------------- 
        jump (m13,i10)(db);
        f9 = f9*f6;                      // f9 = reg0 = reg0*k^2
        f8 = f8*f6;                      // f8 = reg1 = reg1*k^2

//=================================================
//2D Metrices are stored in registers F4,F5,F6,F7
// f9 = reg0, f8 = reg1
// f4 = reg0, f5 = -reg0, f6 = reg1, f7=-reg1
// r14 = 1
case_00:	
        r7 = 0x01010101;
        r11 = lshift r11 by -1;
        r10 = lshift r10 by -1;
        r10 = r10 * r7(uui), f4 = f9;
		r11 = r11 * r7(uui), f6 = f8;
        jump case_end0 (DB);
         f5 = -f9, dm(i3,m6) = r10;   
         f7 = -f8, dm(i4,m6) = r11;
           
// f9 = reg0, f8 = reg1
// f4 = reg1, f5 = -reg1, f6 = reg0, f7=-reg0
// r14 = 1
case_10:	
        r7 = 0x01010101;
        r10 = lshift r10 by -1;
        r10 = r10 * r7(uui), f4 = f8;        
        r13 = r11+r14, r11 = r11-r14;
        r12 = lshift r13 by -1;		
        r11 = lshift r11 by -1;	
		r12 = r12 or fdep r12 by 24:8;
		r7 = 0x10100;
		r11 = r11 * r7(uui), f6 = f9;
		r11 = r11 or r12;
        jump case_end0 (DB);
          f5 = -f8, dm(i4,m6)=r11;	
          f7 = -f9, dm(i3,m6)=r10;		 
          
// f9 = reg0, f8 = reg1
// f4 = -reg1, f5 = reg1, f6 = -reg0, f7=reg0
// r14 = 1
case_20:	
        r7 = 0x01010101;
        r11 = lshift r11 by -1;
        r11 = r11 * r7(uui), f5 = f8;        
        r13 = r10+r14, r10 = r10-r14;
        r13 = lshift r13 by -1;	
        r10 = lshift r10 by -1;
        r10 = lshift r10 by 8;
        r10 = r10 or r13, f7 = f9;
        r10 = r10 or fdep r10 by 16:16;
	    jump case_end0(DB);
          f4 = -f8, dm(i4,m6)=r11; 
          f6 = -f9, dm(i3,m6)=r10; 	
        
// f9 = reg0, f8 = reg1
// f4 = -reg0, f5 = reg0, f6 = -reg1, f7=reg1
// r14 = 1
case_30:
        r5 = r10+r14,  r13 = r10-r14; 	
        r5 = lshift r5 by -1;
        r13 = lshift r13 by -1;		
        r13 = lshift r13 by 8;
        r5 = r5 or r13;
        r5 = r5 or fdep r5 by 16:16;
        r10 = r11+r14, r11=r11-r14;
        r10 = lshift r10 by -1;
        r11 = lshift r11 by -1;
		r10 = r10 or fdep r10 by 24:8;
		r7 = 0x10100;
		r11 = r11 * r7(uui);
		r10 = r10 or r11;
        f4 = -f9, dm(i3,m6) = r5;		
        f5 = -f4, dm(i4,m6) = r10;
        f6 = -f8;
        f7 = -f6;
//				 			
//=========================================================================
//-------------------------------------------------------------------
//			Calculate structure met_4d
//		Every pair   defines the QPSK point a,b,c, or d as follows

//			z0	z1	point
//			0	0	a	
//			0	1	b
//			1	0	c	
//			1	1	d
//   Table 7 SMU   Z-code to QPSK A,B,C,D points mapping

//	4D types is formed by pairs of point 
//		(a,a)  (b,b)
//		(c,c)  (d,d)
//		(a,b)  (b,a)
//		(c,d)  (d,c)
//		(a,c)  (b,d)
//		(c,b)  (d,a)
//		(a,b)  (b,c)
//		(c,a)  (d,b)
	
//		and we shold chose 4D types by summing 2D-metrics of point and chosing the best (minimal)
//	 	The appopriate bit values corresponds to the following 
//		4d orthogonal trellis code we have to decode

/*
Met for points	z0(n)	z1(n)	z0(n+1)	z1(n+1)      The correspondent way
(a,a)            0        0        0       0          0
(b,b)            0        1        0       1          5
(c,c)		     1        0        1       0         10
(d,d)            1        1	       1       1         15
(a,b)            0        0        0       1          1
(b,a)            0        1        0       0          4
(c,d)	         1    	  0        1       1         11
(d,c)	         1        1        1       0         14
(a,c)            0    	  0        1       0          2
(b,d)	         0        1        1       1          7
(c,b)            1        0        0       1          9
(d,a)            1        1        0       0         12
(a,b)            0        0        1       1          3
(b,c)            0        1        1       0          6
(c,a)            1        0        0       0          8
(d,b)            1        1        0       1         13
*/
//
case_end0:
        dm(SaveReg+6) = i0;
        dm(SaveReg  ) = i1;
        i1 = SaveReg+1;
        i0 = InDist;
        i9 = WAY_table;

        r14 = r14-r14, f15<->s15;
        f8 = f0+f4,	  r13 = pm(i9,m14);  //r13 = 0
        f9 = f1+f5,   r10 = pm(i9,m14);  //s10 = 5
        comp(f8,f9), r15 = i2;
        if ge f8 = f14+f9, r13 = r10; 
        f11 = f2+f6, dm(i0,m6)=f8,  r10=pm(i9,m14);
        f12 = f3+f7, dm(i0,m6)=r13, r13=pm(i9,m14); 
        comp(f11,f12), dm(i1,m6)=r15;
        if ge f11 = f12+f14, r10 = r13;
        f8 = f0+f5, dm(i0,m6)=f11, r13=pm(i9,m14);
        f9 = f1+f4,	dm(i0,m6)=r10, r10=pm(i9,m14);
        comp(f8,f9), r15 = i3;
        if ge f8=f9+f14, r13 = r10;
        f11=f2+f7, 	dm(i0,m6)=f8,  r10=pm(i9,m14);
        f12=f3+f6,	dm(i0,m6)=r13, r13=pm(i9,m14);
        comp(f11,f12), dm(i1,m6)=r15;
        if ge f11=f12+f14, r10 = r13;
        f8=f0+f6,  dm(i0,m6)=f11, r13=pm(i9,m14);
        f9=f1+f7,  dm(i0,m6)=r10, r10=pm(i9,m14);
        comp(f8,f9), r15 = i4;
        if ge f8=f9+f14, r13 = r10;
        f11=f2+f5, 	dm(i0,m6)=f8,   r10=pm(i9,m14);
        f12=f3+f4,	dm(i0,m6)=r13,	r13=pm(i9,m14);		
        comp(f11,f12), dm(i1,m6)=r15;
        if ge f11=f12+f14, r10 = r13;
        f8=f0+f7, 	dm(i0,m6)=f11,  r13=pm(i9,m14);
        f9=f1+f6,	dm(i0,m6)=r10,	r10=pm(i9,m14);
        comp(f8,f9), r15 = i5;
        if ge f8=f9+f14, r13 = r10;
        f11=f2+f4, 	dm(i0,m6)=f8,   r10=pm(i9,m14);
        f12=f3+f5,	dm(i0,m6)=r13,	r13=pm(i9,m14);
        comp(f11,f12), dm(i1,m6)=r15;
        if ge f11=f12+f14, r10 = r13;
        r7 = r7-r7, dm(i0,m6)=f11;
        r7 = r7+1, dm(i0,m6)=r10;          // r7 = 1


//==================================  One step Viterbi start

        i3  = State;
        i5  = Sig;
        
        i11 = pm(Ptr_NewMet);
        b14 = pm(Ptr_CurMet); 	   	   
        i1  = DistModif;
        i10 = InDistPtr;
        i0  = dm(Ptr_CurStepMetOff);

        i8  = States;
        f7 = float r7, f15<->s15;               // f7 = 1.0, restore f15
        r10 = r10 - r10, i2 = pm(i10,m14);
        r14 = r10+1, i14 = dm(i0,m6);           // r14=1 for the next step of main loop
        f6 = (f7+f10)/2, m11 = dm(i1,m6);       // f6=0.5 for the next step of main loop
        lcntr = 16, do (pc,VitStep) until lce;
          f12 = dm(i2,m6), f8 = pm(i14,m11);   // f12=met_4d[0].met, f8=CurMet[0];
          f9  = f8+f12,    r4 = dm(i2,m6), r5 = pm(i8,m14);     // f9=CurMet[0]+met_4d[0].met; r4 =met_4d[0].way
          f12 = dm(i2,m6), f8 = pm(i14,m11);   // f8=CurMet[4]; f12=met_4d[1].met
          f3 = f8+f12,     f13 = dm(i2,m6), r1 = pm(i8,m14);     // f3=CurMet[4]+met_4d[1].met; f2 =met_4d[1].way 
          comp(f9,f3), f12 = dm(i2,m6), f8 = pm(i14,m11);    // f12=met_4d[2].met, f8=CurMet[8];
          if gt f5= f1*f7, f4 = f10+f13, f9=f3;     
          f3 = f8+f12,  f13 = dm(i2,m6), r1 = pm(i8,m14);        // f3=CurMet[8]+met_4d[2].met; r2=met_4d[2].way;
          comp(f9,f3),  f12 = dm(i2,m6), f8 = pm(i14,m13);   // f12=met_4d[3].met , f8=CurMet[12];
          if gt f5= f1*f7, f4 = f10+f13, f9=f3;     
          f3 = f8+f12,  f13 = dm(i2,m6), r1 = pm(i8,m14);        // f3=CurMet[12]+met_4d[2].met; r2=met_4d[3].way;
          comp(f9,f3),  i2 = pm(i10,m14); 
          if gt f5= f1*f7, f4 = f10+f13, f9=f3;     
          r5 = trunc f5, i14 = dm(i0,m6);
          r4 = trunc f4, dm(i3,m6) = r5; 
          m11 = dm(i1,m6);
VitStep:  dm(i5,m6) = r4, pm(i11,m14)=f9;       //store wb_sig[]=r4, store NewMet[]=summet 
 
        r5 = pm(Ptr_NewMet);
        r4 = pm(Ptr_CurMet);
        pm(Ptr_NewMet) = r4;
        pm(Ptr_CurMet) = r5;

        r5 = dm(Ptr_NewStepMetOff);
        r4 = dm(Ptr_CurStepMetOff);
        dm(Ptr_NewStepMetOff) = r4;
        dm(Ptr_CurStepMetOff) = r5;

        i3 = dm(PtrWb_State);
        i5 = dm(PtrWb_Sig);
        i1 = State+15;
        i2 = Sig+15;
        r4 = 0x11c; // len6 field for "fdep" instr=4, bit6 = 28
        r0 = dm(i1,m7);  // State[0]
        r0 = lshift r0 by 28, r2 = dm(i2,m7);  // Sig[0]
        r2 = lshift r2 by 28;
        r3 = 4;
        lcntr = 7, do (pc,Pk_St_Sg0) until lce;
           r4 = r4-r3, r1 = dm(i1,m7);
           r0 = r0 or fdep r1 by r4, r1 = dm(i2,m7);
Pk_St_Sg0: r2 = r2 or fdep r1 by r4;
        dm(i3,m6) = r0;  // save State[i][7:0]
        dm(i5,m6) = r2;  // save Sig[i][7:0]

        r4 = 0x11c; // len6 field for "fdep" instr
        r0 = dm(i1,m7);  // State[0]
        r0 = lshift r0 by 28, r2 = dm(i2,m7);  // Sig[0]
        r2 = lshift r2 by 28;
        lcntr = 7, do (pc,Pk_St_Sg1) until lce;
           r4 = r4-r3, r1 = dm(i1,m7);
           r0 = r0 or fdep r1 by r4, r1 = dm(i2,m7);
Pk_St_Sg1: r2 = r2 or fdep r1 by r4;
        dm(i3,m6) = r0;  // save State[i][15:7]
        dm(i5,m6) = r2;  // save Sig[i][15:7]
        
        dm(PtrWb_State) = i3;
        dm(PtrWb_Sig) = i5;
        
        i0  = dm(SaveReg+6);
        i1  = dm(SaveReg  );
        i2  = dm(SaveReg+1);
        i3  = dm(SaveReg+2);
        i4  = dm(SaveReg+3);
main_loop: i5  = dm(SaveReg+4); 

//===end of main loop 
//======================= end of Viterbi 

    bit clr mode1 CBUFEN;
    l14 = 0;

//========================== Viterbi back step
    i2 = dm(_num_abcd_ptr);
    i5 = dm(_wb_sig_ptr);
    i3 = dm(_wb_st_ptr);
    
    r4 = r4-r4;            // last State
    r8 = gets( 1 );        // r8 = len4d
    r2 = r8  -  1;         // r2 = len4d-1
    r5 = r2 + r2, m2 = r2;
    modify(i2,m2);    
    m2 = r5;
    modify(i3,m2);
    modify(i5,m2); 
    m2 = -2;
    r6 = 8;
    r7 = r7 - r7;
    r9 = 0x100;
    lcntr = r8, do (pc,Vit_Back) until lce;
      r0 = dm(i3,m2)(LW);
      r5 = r4 - r6, r2 = dm(i5,m2)(LW);  
      if lt r2 = r7+r3, r0 = r1;
      r5 = r4 - r6;
      if ge r4 = r4 - r6;
      r5 = lshift r4 by 2;
      r5 = r5 or r9;
      r2 = fext r2 by r5;
Vit_Back: r4 = fext r0 by r5, dm(i2,m7) = r2;  // m7 = -1

    mode1 = ustat1;             // 22.01.04 vgp

    i15 = gets( 38 );
    r2  = gets( 37 );
    r3  = gets( 36 );
    r5  = gets( 35 );
    r6  = gets( 34 );
    r7  = gets( 33 );
    r8  = gets( 32 );
    r9  = gets( 31 );
    r10 = gets( 30 );
    r11 = gets( 29 );
    r12 = gets( 28 );
    r13 = gets( 27 );
    r14 = gets( 26 );
    r15 = gets( 25 );
    i0  = gets( 24 );
    i1  = gets( 23 );
    i2  = gets( 22 );
    i3  = gets( 21 );
    i4  = gets( 20 );
    i5  = gets( 19 );
    i6  = gets( 18 );
    i8  = gets( 17 );
    i9  = gets( 16 );
    i10 = gets( 15 );

    i11 = gets( 14 );
    i12 = gets( 13 );
    i13 = gets( 12 );
    i14 = gets( 11 );
    m0  = gets( 10 );
    m1  = gets( 9 );
    m2  = gets( 8 );
    m3  = gets( 7 );
    m4  = gets( 6 );
    m5  = gets( 5 );
    m9  = gets( 4 );
    m10 = gets( 3 );
    m11 = gets( 2 );


      exit; 

_TCM_Trellis_34_ViterbiDecoder.end:

.endseg;	




