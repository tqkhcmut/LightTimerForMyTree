
#ifndef __BSP_UART_H__
#define __BSP_UART_H__


/******************************************************************************
* If building with a C++ compiler, make all of the definitions in this header
* have a C binding.
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


/******************************************************************************
* INCLUDES
*/
#include "bsp.h"


/******************************************************************************
* DEFINES
*/

// Baud rates
#define BSP_UART_BR_9600			9600
#define BSP_UART_BR_38400			38400
#define BSP_UART_BR_57600			57600
#define BSP_UART_BR_115200			115200


// UART return codes
#define BSP_UART_SUCCESS            0x00
#define BSP_UART_UNCONFIGURED       0x01
#define BSP_UART_NOT_SUPPORTED      0x02
#define BSP_UART_BAUDRATE_ERROR     0x04
#define BSP_UART_ERROR              0x08
	
	
#define USCI_A_UART_ONE_STOP_BIT                                           0x00
#define USCI_A_UART_TWO_STOP_BITS                                         UCSPB
	
	
#define USCI_A_UART_MSB_FIRST                                             UCMSB
#define USCI_A_UART_LSB_FIRST                                              0x00
	
	
#define USCI_A_UART_MODE                                               UCMODE_0
#define USCI_A_UART_IDLE_LINE_MULTI_PROCESSOR_MODE                     UCMODE_1
#define USCI_A_UART_ADDRESS_BIT_MULTI_PROCESSOR_MODE                   UCMODE_2
#define USCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE                  UCMODE_3

/******************************************************************************
* TYPEDEFS
*/


/******************************************************************************
* FUNCTION PROTOTYPES
*/
extern void  bspUartInit(void);
extern uint8_t bspUartDataGet(uint8_t *pui8Data, uint8_t ui16Length);
extern uint8_t bspUartDataPut(uint8_t *pui8Data, uint8_t ui16Length);

/******************************************************************************
* Mark the end of the C bindings section for C++ compilers.
******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* #ifndef __BSP_UART_H__ */
