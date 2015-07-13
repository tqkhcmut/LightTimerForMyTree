#ifndef _rf_config_h_
#define _rf_config_h_

#include "msp430.h"
#include "inttypes.h"

void 	RF_Config(void);
void 	RF_Send(uint8_t * buff, uint8_t len);
uint8_t RF_Recv(uint8_t * buff, uint8_t len);

#endif // _rf_config_h_

