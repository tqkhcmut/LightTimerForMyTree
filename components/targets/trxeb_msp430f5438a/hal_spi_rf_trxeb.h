/******************************************************************************
*  Filename: hal_spi_rf_trxeb.h
*
*  Description: Common header file for spi access to the different tranceiver
*               radios. Supports CC1101/CC112X radios
*				 
*  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/

#ifndef HAL_SPI_RF_TRXEB_H
#define HAL_SPI_RF_TRXEB_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 */
#include <msp430.h>
#include "hal_types.h"
#include "hal_defs.h"

/******************************************************************************
 * CONSTANTS
 */

#define TRXEM_PORT_SEL       P4SEL
#define TRXEM_PORT_OUT       P4OUT
#define TRXEM_PORT_DIR       P4DIR
#define TRXEM_PORT_IN        P4IN

#define TRXEM_SPI_MOSI_PIN   BIT1
#define TRXEM_SPI_MISO_PIN   BIT2
#define TRXEM_SPI_SCLK_PIN   BIT3
#define TRXEM_SPI_SC_N_PIN   BIT0

#define RF_RESET_N_PORT_OUT  P4OUT
#define RF_RESET_N_PORT_SEL  P4SEL
#define RF_RESET_N_PORT_DIR  P4DIR
#define RF_RESET_N_PIN       BIT6


#define RADIO_BURST_ACCESS   0x40
#define RADIO_SINGLE_ACCESS  0x00
#define RADIO_READ_ACCESS    0x80
#define RADIO_WRITE_ACCESS   0x00


/* Bit fields in the chip status byte */
#define STATUS_CHIP_RDYn_BM             0x80
#define STATUS_STATE_BM                 0x70
#define STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F


/******************************************************************************
 * MACROS
 */

/* Macros for Tranceivers(TRX) */
#define TRXEM_SPI_BEGIN()              st( TRXEM_PORT_OUT &= ~TRXEM_SPI_SC_N_PIN; NOP(); )
#define TRXEM_SPI_TX(x)                st( UCB1IFG &= ~UCRXIFG; UCB1TXBUF= (x); )
#define TRXEM_SPI_WAIT_DONE()          st( while(!(UCB1IFG & UCRXIFG)); )
#define TRXEM_SPI_RX()                 UCB1RXBUF
#define TRXEM_SPI_WAIT_MISO_LOW(x)     st( uint8 count = 200; \
                                           while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN) \
                                           { \
                                              __delay_cycles(5000); \
                                              count--; \
                                              if (count == 0) break; \
                                           } \
                                           if(count>0) (x) = 1; \
                                           else (x) = 0; )

#define TRXEM_SPI_END()                st( NOP(); TRXEM_PORT_OUT |= TRXEM_SPI_SC_N_PIN; )

/******************************************************************************
 * TYPEDEFS
 */

typedef struct
{
  uint16  addr;
  uint8   data;
}registerSetting_t;

typedef uint8 rfStatus_t;

/******************************************************************************
 * PROTOTYPES
 */

void trxRfSpiInterfaceInit(uint8 clockDivider);
rfStatus_t trx8BitRegAccess(uint8 accessType, uint8 addrByte, uint8 *pData, uint16 len);
rfStatus_t trxSpiCmdStrobe(uint8 cmd);

/* CC112X specific prototype function */
rfStatus_t trx16BitRegAccess(uint8 accessType, uint8 extAddr, uint8 regAddr, uint8 *pData, uint8 len);

#ifdef  __cplusplus
}
#endif
#endif //HAL_SPI_RF_TRXEB_H