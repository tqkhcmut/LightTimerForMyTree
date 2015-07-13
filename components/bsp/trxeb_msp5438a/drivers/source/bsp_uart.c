
#include "bsp_uart.h"

#include <string.h>

#define M_MIN(a,b) ((a)>(b))?(b):(a)

#define MAX_RX_COUNT 255
uint16_t RxCount = 0;
uint16_t RxIndex = 0;
uint8_t RxBuffer[MAX_RX_COUNT];

void  bspUartInit(void)
{
	P4SEL |= BIT4 | BIT5;                     // P3.3,4 = USCI_A0 TXD/RXD
	UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA1CTL1 |= UCSSEL_2;                     // SMCLK
	UCA1BR0 = 138;                              // 1MHz 115200 (see User's Guide)
	UCA1BR1 = 0;                              // 1MHz 115200
	UCA1MCTL |= UCBRS_1 | UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
	UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

uint8_t bspUartDataGet(uint8_t *pui8Data, uint8_t ui16Length)
{
	uint8_t resLen = M_MIN(ui16Length, RxCount);
	
	memcpy(pui8Data, RxBuffer, resLen);
	RxCount -= resLen;
	RxIndex -= resLen;
	
	return resLen;
}

uint8_t bspUartDataPut(uint8_t *pui8Data, uint8_t ui16Length)
{
	uint8_t resLen = 0;
	
	while(resLen < ui16Length)
	{
		UCA1TXBUF = *(pui8Data + resLen++);
  		while (!(UCA1IFG & UCTXIFG));
	}
	return resLen;
}

//******************************************************************************
//
//This is the USCI_A1 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV,4))
    {
    //Vector 2 - RXIFG
    case 2:
        RxBuffer[RxIndex++] = UCA1RXBUF;
        RxCount++;
        break;
    default: break;
    }
}