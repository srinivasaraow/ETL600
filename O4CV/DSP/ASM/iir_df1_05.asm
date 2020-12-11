/*****************************************************************************
*             Copyright (c) 1995 - 20xx by GAO Research Inc.                 *
*                          All rights reserved.                              *
*                                                                            *
*  This program is a confidential and an unpublished work of GAO Research    *
*  Inc. that is protected under international and Canadian copyright laws.   *
*  This program is a trade secret and constitutes valuable property of GAO   *
*  Research Inc. All use, reproduction and/or disclosure in whole or in      *
*  part of this program is strictly prohibited unless expressly authorized   *
*  in writing by GAO Research Inc.                                           *
*****************************************************************************/

/*****************************************************************************
*  This assembly function implements a Direct Form I IIR filter.
*  
*  The coefficients for each biquad should be ordered as 
*      b0, b1, b2, a1, a2  // stage 1
*      b0, b1, b2, a1, a2  // stage 2
*      b0, b1, b2, a1, a2  // stage 3
*      ...
*  
*  Note a0 is normalized to 0.5, so all coefficients should be divided by 2
*  compared with the case when a0 is 1.
*  
*  User should provide coefficient buffer to call this function. Coefficient buffer 
*  needs to be 16-bit aligned.
*  
*  Two delay lines are needed, one for input, one for output. For each biquad,
*  2 previous input samples are srored in the input delay line, and 2 previous 
*  output samples are stored in the output delay line.
*  
*  User should provide 2 buffers for delay lines. Both buffers should be 32-bit 
*  aligned.
*  
*  Unlike ADI's iir_fr16 function, this function doesn't impose any limit as to 
*  which memory bank should reside the coefficients, input/output buffers, and 
*  delay lines.
*
*  Refer to GAO_IIR_API.h for function prototype.
******************************************************************************/

    .section program;
    .global _iir_df1_05;
    .align 2;
_iir_df1_05:
    [--SP] = R4;
    [--SP] = P3;
    P2 = [SP+0x14];                 // pointer to iir_df1_state structure
    CC = R2 < 1;                    // Number of samples < 0 ?
    if CC jump _iir_df1_05_last;
    P0 = R0;                        // Input buffer
    P1 = R1;                        // Output buffer
    R4 = [P2++];                    // I0: Coefficients
    I0 = R4;
    R4 = [P2++];                    // I1: X delay line
    I1 = R4;
    R4 = [P2++];                    // I2: Y delay line
    I2 = R4;
    R3 = [P2];                      // R3: Number of stages
    CC = R3 < 1;                    // Number of stages < 0 ?
    if CC jump _iir_df1_05_last;
    P2 = R3;
    P3 = R2;
    R0 = 10(Z);                     // Each stage has 5 coefficients, i.e. 10 bytes
    R0.L = R0.L * R3.L(IS);
    L0 = R0;                        // Coefficients set to circular buffer
    R0 = R3 << 2;
    L1 = R0;                        // X buffer circular
    L2 = R0;                        // Y buffer circular
    B0 = I0;
    B1 = I1;
    B2 = I2;
    M0 = 1;
    M1 = 1;
    M2 = 1;
    
    R0.L = W[I0++];                 // R0.L: b0, no 32-bit alignment required
    R0.H = W[I0++];                 // R0.H: b1
    R1.L = W[I0++];                 // R1.L: b2
    CC = R2 == 1;                   // don't go into loop if number of samples is 1
    if CC jump _iir_df1_05_last_samp;
    lsetup (_iir_samp_loop_start, _iir_samp_loop_end) LC0 = P3 >> 1;
_iir_samp_loop_start:
        R3 = [P0++];       // R3.L: x[n], R3.H: x[n+1]
        lsetup (_iir_stage_loop_start, _iir_stage_loop_end) LC1 = P2;
_iir_stage_loop_start:
            // intm[n+0] = b0 * x[n+0] + b1 * x[n-1] + b2 * x[n-2]
            // intm[n+1] = b0 * x[n+1] + b1 * x[n+0] + b2 * x[n-1]
            A0  = R0.L*R3.L, A1  = R0.L*R3.H || R4 = [I1];         // R4.L: x[n-2], R4.H: x[n-1]
            A0 += R0.H*R4.H || R1.H = W[I0++];                     // R1.H: a1
            A1 += R0.H*R3.L || [I1++] = R3 || R0.L = W[I0++];      // x[n-2] <-- x[n+0], x[n-1] <-- x[n+1], R0.L: a2
            A0 += R1.L*R4.L, A1 += R1.L*R4.H || R4 = [I2];         // R4.L: y[n-2], R4.H: y[n-1]
            
            // y[n+0] = intm[n+0] - (a1 * y[n-1] + a2 * y[n-2])
            // y[n+1] = intm[n+1] - (a1 * y[n+0] + a2 * y[n-1])
            A0 -= R0.L*R4.L, A1 -= R0.L*R4.H || R0.L = W[I0++];    // R0.L: b0 for next stage
            R3.L = (A0 -= R1.H*R4.H)(s2rnd) || R0.H = W[I0++];     // R3.L: y[n+0]*2; R0.H: b1 for next stage
            R3.H = (A1 -= R1.H*R3.L)(s2rnd) || R1.L = W[I0++];     // R3.H: y[n+1]*2; R1.L: b2 for next stage
_iir_stage_loop_end:
            [I2++] = R3;   // y[n-2] <-- y[n+0], y[n-1] <-- y[n+1], R3: input for next stage
_iir_samp_loop_end:
        [P1++] = R3;       // update output buffer

    R2 = P3;
    CC = bittst(R2, 0);
    if !CC jump _iir_df1_05_last;   // if even number of samples, we are done
_iir_df1_05_last_samp:
    // R0.L: b0, R0.H: b1 R1.L: b2, all loaded from last loop
    R3.L = W[P0];                   // R3.L: x[n]
    lsetup (_iir_sing_samp_start, _iir_sing_samp_end) LC1 = P2;
_iir_sing_samp_start:
        // intm[n+0] = b0 * x[n+0] + b1 * x[n-1] + b2 * x[n-2]
        A0  = R0.L*R3.L || R4 = [I1];         // R4.L: x[n-2], R4.H: x[n-1]
        A0 += R0.H*R4.H || R1.H = W[I0++];    // R1.H: a1
        A0 += R1.L*R4.L || R0.L = W[I0++];    // R0.L: a2
        R3  = pack(R3.L, R4.H) || R4 = [I2];  // R4.L: y[n-2], R4.H: y[n-1]  
        
        // y[n+0] = intm[n+0] - (a1 * y[n-1] + a2 * y[n-2])
        // x[n-1] <-- x[n+0], x[n-2] <-- x[n-2]
        A0 -= R1.H*R4.H || [I1++] = R3;
        R3.L = (A0 -= R0.L*R4.L)(s2rnd) || R0.L = W[I0++];          // R0.L: b0 for next stage; y[n+0] * 2, 
        R4 = pack(R3.L, R4.H) || W[P1] = R3.L || R0.H = W[I0++];    // R0.H: b1 for next stage; update output buffer
_iir_sing_samp_end:
        [I2++] = R4 || R1.L = W[I0++];       // R1.L: b2 for next stage; y[n-1] <-- y[n+0], y[n-2] <-- y[n-1]
    
_iir_df1_05_last:
    L0 = 0;
    L1 = 0;
    L2 = 0;
    P3 = [SP++];
    R4 = [SP++];
    rts;

._iir_df1_05.end:
