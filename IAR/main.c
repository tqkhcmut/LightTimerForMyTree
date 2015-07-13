#include "msp430.h"
#include "bsp.h"
#include "bsp_uart.h"
#include "rf_conf.h"
#include "relays.h"

#include "LEDs.h"

#include <stdint.h>
#include <string.h>

#define TWO_HOUR	(2 * 3600)
#define EIGHT_HOUR	(8 * 3600)
#define TEN_HOUR	(10 * 3600)	

int main( void )
{
	uint8_t tmpBuff[100];
	
	uint32_t time = 0;
	
	
	// Init clock and IO
	bspInit();
	
	// Init UART 
	bspUartInit();
	
//	RF_Config();
	
	Relay_Init();
	Relay_Off(ROne);
	
	LED_API.Init();
	
	// Enable global interrupt
	__bis_SR_register(GIE);
	
	bspUartDataPut("Tra Quang Kieu", 14);
	
	time = TWO_HOUR;
	
	for(;;)
	{
		LED_API.Toggle(LED1);
//		Relay_Toggle(ROne);
//		Relay_Toggle(RTwo);
		if (time == 0)
		{
			time = EIGHT_HOUR;
			Relay_Toggle(ROne);
		}
		else
		{
			time--;
		}
		if (bspUartDataGet(tmpBuff, 10) > 0)
		{
			if (memcmp(tmpBuff, "on\n", 3) == 0)
			{
				Relay_On(ROne);
			}
			else if (memcmp(tmpBuff, "off\n", 4) == 0)
			{
				Relay_Off(ROne);
			}
		}
//		bspUartDataPut("Tra Quang Kieu", 14);
		
		__delay_cycles(16000000);
	}
}
