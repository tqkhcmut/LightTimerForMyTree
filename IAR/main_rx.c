#include "msp430.h"
#include "bsp.h"
#include "bsp_uart.h"
#include "rf_conf.h"
#include "relays.h"

#include "LEDs.h"


int main( void )
{
	uint8_t tmpBuff[100];
	uint8_t tmpLen = 0;
	
	// Init clock and IO
	bspInit();
	
	// Init UART 
	bspUartInit();
	
	RF_Config();
	
	Relay_Init();
	
	LED_API.Init();
	
	// Enable global interrupt
	__bis_SR_register(GIE);
	
	bspUartDataPut("Tra Quang Kieu", 14);
	
	for(;;)
	{
//		LED_API.Toggle(LED1);
		//		Relay_Toggle(ROne);
		//		Relay_Toggle(RTwo);
//		bspUartDataPut("Tra Quang Kieu", 14);
		
//		tmpLen = bspUartDataGet(tmpBuff, 100);
//		if (tmpLen > 0)
//		{
//			bspUartDataPut(tmpBuff, tmpLen);
//			RF_Send("transmiter", 10);
//		}  
		tmpLen = RF_Recv(tmpBuff, 100);
		if (tmpLen > 0)
		{
			// append null character
			tmpBuff[tmpLen] = 0;
			//			Relay_Toggle(ROne);
			bspUartDataPut(tmpBuff, tmpLen);
		}
//		RF_Send("transmiter", 10);
		__delay_cycles(16000);
	}
}
