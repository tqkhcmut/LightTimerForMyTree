#include "relays.h"

#define RELAY_1					BIT2	//
#define RELAY_2 				BIT3	//

#define RELAY_PORT_SEL			P6SEL
#define RELAY_PORT_DIR			P6DIR
#define RELAY_PORT_OUT			P6OUT
#define RELAY_PORT_IN			P6IN

#define RELAY_1_OFF {RELAY_PORT_OUT |=  (RELAY_1);}
#define RELAY_1_ON 	{RELAY_PORT_OUT &= ~(RELAY_1);}
#define RELAY_2_OFF {RELAY_PORT_OUT |=  (RELAY_2);}
#define RELAY_2_ON 	{RELAY_PORT_OUT &= ~(RELAY_2);}

void Relay_Init(void)
{
	RELAY_PORT_SEL &= ~(RELAY_1 + RELAY_2);  // Set relays pin to zero for I/O function
	RELAY_PORT_DIR |=  (RELAY_1 + RELAY_2);  // Set relays pin direction to output
	
	RELAY_1_OFF;
	RELAY_2_OFF;
}

static unsigned char relay1_state = 0;
static unsigned char relay2_state = 0;

void Relay_On(Relay_Number_t relayNumber)
{
	switch (relayNumber)
	{
	case ROne:
		RELAY_1_ON;
		relay1_state = 1;
		break;
	case RTwo:
		RELAY_2_ON;
		relay2_state = 1;
		break;
	default:
		break;
	}
}

void Relay_Off(Relay_Number_t relayNumber)
{
	switch (relayNumber)
	{
	case ROne:
		RELAY_1_OFF;
		relay1_state = 0;
		break;
	case RTwo:
		RELAY_2_OFF;
		relay2_state = 0;
		break;
	default:
		break;
	}
}

void Relay_Toggle(Relay_Number_t relayNumber)
{
	switch (relayNumber)
	{
	case ROne:
		if (relay1_state)
		{
			RELAY_1_OFF;
			relay1_state = 0;
		}
		else
		{			
			RELAY_1_ON;
			relay1_state = 1;
		}
		break;
	case RTwo:
		if (relay2_state)
		{
			RELAY_2_OFF;
			relay2_state = 0;
		}
		else
		{			
			RELAY_2_ON;
			relay2_state = 1;
		}
		break;
	default:
		break;
	}
}
