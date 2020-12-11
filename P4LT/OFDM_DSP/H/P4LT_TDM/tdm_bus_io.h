#ifndef TDM_BUS_IO_H

#define TDM_BUS_IO_H

extern int Flag_TDM02_Data_Received;
extern int Flag_TDM02_Data_Transmitted;
extern int Flag_TDM13_Data_Received;
extern int Flag_TDM13_Data_Transmitted;


int Init_TDM_Bus_IO(int nch, int slen, int *rxmask, int *txmask, 
                    int *inbuf, int *outbuf,
                    int ismaster, int issport13);

//
// Description
//           Initialization of SPORT02 or SPORT13 in TDM bus mode
//
// Input:
//  NCH  - number of channels in TDM frame in the range 1..128
//  SLEN - number of bits in time slot in the range 3..32
//  RXMASK[0..3] - vector of 4 word length, RX bit mask of receive time slot within frame for each time slot out of 128
//  TXMASK[0.3] - vector of 4 word length, TX bit mask of receive time slot within frame within the frame for each time slot out of 128
// *InBuf - points receive TDM buffer of length 2*HD( RXMASK[0..3]) 32-bit words
//                 where, HD(x) - hamming distance of bit vector "x", i.e. number of non zero bits in "x"
// *OutBuf - points transmit TDM buffer of length 2*HD(TXMASK[0..3]) 32-bit words
//  IsMaster -  if eq. to 1, then accepts serial clock and generates frame syncs, out of external serial clock
//              if eq 0, then accepts both seral clock and frame syncs
//  IsSport13 - if 1, then TDM bus is SPORT13 of DSP21161, sport buffer 1A  receives data, sport buffer 3A transmits data
//              if 0, then TDM bus is SPORT02 of DSP21161, sport buffer 0A  receives data, sport buffer 2A transmits data
// Output:
//  0 - if initialization of sport is succeseful, othervise returns 1


void  Start_TDM_Bus_IO(int IsSport13);
//
// Description
//           Start of TDM mode operating of SPORT02 or SPORT13
//
// Input:
//  IsSport13 - if ne 0, then TDM bus is SPORT13 of DSP21161, sport buffer 1A  receives data, sport buffer 3A transmits data
//              if 0, then TDM bus is SPORT02 of DSP21161, sport buffer 0A  receives data, sport buffer 2A transmits data
// Output:
//    none

void  Stop_TDM_Bus_IO(int IsSport13);
//
// Description
//           Stop of TDM mode operating of SPORT02 or SPORT13
//
// Input:
//  IsSport13 - if ne 0, then TDM bus is SPORT13 of DSP21161, sport buffer 1A  receives data, sport buffer 3A transmits data
//              if 0, then TDM bus is SPORT02 of DSP21161, sport buffer 0A  receives data, sport buffer 2A transmits data
// Output:
//    none

int  *Get_TDM_Bus_Data_RX(int IsSport13);
//
// Description
//           Returns pointer to buffer with received data of SPORT02 or SPORT13
//
// Input:
//  IsSport13 - if ne 0, then TDM bus is SPORT13 of DSP21161, sport buffer 1A  receives data, sport buffer 3A transmits data
//              if 0, then TDM bus is SPORT02 of DSP21161, sport buffer 0A  receives data, sport buffer 2A transmits data
// Output:
//              pointer to received data buffer if operation is complete,
//           or NULL (see stdlib.h) if operation is in progress
//

int  *Get_TDM_Bus_Data_TX(int IsSport13);
//
// Description
//           Returns pointer to empty transmit of SPORT02 or SPORT13
//
// Input:
//  IsSport13 - if ne 0, then TDM bus is SPORT13 of DSP21161, sport buffer 1A  receives data, sport buffer 3A transmits data
//              if 0, then TDM bus is SPORT02 of DSP21161, sport buffer 0A  receives data, sport buffer 2A transmits data
// Output:
//              pointer to empty transmit data buffer (previous transmit operation from buffer is completed)
//           or NULL (see stdlib.h) if operation is in progress
//

#endif

