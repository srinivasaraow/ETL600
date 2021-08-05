#ifndef CHANNEL_DATA_H

#define CHANNEL_DATA_H

#include "../ofdm_defs.h"
#include "../tcm/tcm_defs.h"
#include "c3212.h"
#include "../ofdm_externs.h"

#define CHD_32BW_TX (sizeof(Channel_Data_TX)-sizeof(Channel_Data_TX.Settings)-sizeof(Channel_Data_TX.K_int))
#define CHD_32BW_RX (sizeof(Channel_Data_RX)-sizeof(Channel_Data_RX.Settings)-sizeof(Channel_Data_RX.K_int))

#endif
