#ifndef Relay_h_
#define Relay_h_

#include <msp430.h>

typedef enum
{
	ROne = 1,
	RTwo = 2
} Relay_Number_t;


void Relay_Init(void);

void Relay_On(Relay_Number_t relayNumber);
void Relay_Off(Relay_Number_t relayNumber);
void Relay_Toggle(Relay_Number_t relayNumber);

#endif
