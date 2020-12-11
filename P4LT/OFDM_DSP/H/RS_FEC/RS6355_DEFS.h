#ifndef RS6355_DEFS_H

#define RS6355_DEFS_H

#define GF64        64
#define LOG_GF64     6
#define GF64_2      (2*GF64)
#define RS6355_N    (GF64-1)
#define RS6355_K    55
#define RS6355_R    (RS6355_N-RS6355_K)
#define RS6355_DEC_RES  728  // 728 - temporary array length for decoding in DM, 
                             //       i.e. (3*(RS6355_R+2) + 2*GF64 + 570)
#define RS6355_ENC_RES  GF64 //  64 - temporary array length for decoding in DM

#endif

