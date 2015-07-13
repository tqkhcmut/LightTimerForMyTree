//*****************************************************************************
//! @file       bsp.c
//! @brief      Board support package for MSP430F5438a on SmartRF06EB.
//!
//! Revised     $Date: 2013-09-18 13:18:48 +0200 (on, 18 sep 2013) $
//! Revision    $Revision: 10587 $
//
//  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************/


/**************************************************************************//**
* @addtogroup bsp_api
* @{
******************************************************************************/


/******************************************************************************
* INCLUDES
*/
#include "bsp.h"


/******************************************************************************
* DEFINES
*/
#define DCO_MULT_1MHZ           30
#define DCO_MULT_4MHZ           122
#define DCO_MULT_8MHZ           244
#define DCO_MULT_12MHZ          366
#define DCO_MULT_16MHZ          488
#define DCO_MULT_20MHZ          610
#define DCO_MULT_25MHZ          763

#define DCORSEL_1MHZ            DCORSEL_2
#define DCORSEL_4MHZ            DCORSEL_4
#define DCORSEL_8MHZ            DCORSEL_4
#define DCORSEL_12MHZ           DCORSEL_5
#define DCORSEL_16MHZ           DCORSEL_5
#define DCORSEL_20MHZ           DCORSEL_6
#define DCORSEL_25MHZ           DCORSEL_7

#define VCORE_1MHZ              PMMCOREV_0
#define VCORE_4MHZ              PMMCOREV_0
#define VCORE_8MHZ              PMMCOREV_0
#define VCORE_12MHZ             PMMCOREV_1
#define VCORE_16MHZ             PMMCOREV_1
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

#define VCORE_1_35V             PMMCOREV_0
#define VCORE_1_55V             PMMCOREV_1
#define VCORE_1_75V             PMMCOREV_2
#define VCORE_1_85V             PMMCOREV_3

// Register defines
#define IO_SPI0_BUS_DIR         P9DIR
#define IO_SPI0_BUS_SEL         P9SEL
#define IO_SPI0_BUS_OUT         P9OUT
#define IO_SPI0_BUS_REN         P9REN

#define IO_SPI1_BUS_DIR         IO_SPI0_BUS_DIR
#define IO_SPI1_BUS_SEL         IO_SPI0_BUS_SEL
#define IO_SPI1_BUS_OUT         IO_SPI0_BUS_OUT
#define IO_SPI1_BUS_REN         IO_SPI0_BUS_REN


/******************************************************************************
* FUNCTION PROTOTYPES
*/

/******************************************************************************
* LOCAL VARIABLES
*/
static uint32_t ui32BspMclkSpeed;


/******************************************************************************
* FUNCTIONS
*/

/**************************************************************************//**
* @brief    Function initializes the MSP430F5438a clocks and I/O for use on
*           SmartRF06EB.
*
*           The function assumes an external crystal oscillator to be available
*           to the MSP430F5438a. The MSP430F5438a main system clock (MCLK) and
*           Sub Main System Clock (SMCLK) are set to the frequency given by
*           input argument \e ui32SysClockSpeed. ACLK is set to 32768 Hz.
*
* @param    ui32SysClockSpeed   is the system clock speed in MHz; it must be
*                               one of the following:
*           \li \b BSP_SYS_CLK_1MHZ
*           \li \b BSP_SYS_CLK_4MHZ
*           \li \b BSP_SYS_CLK_8MHZ
*           \li \b BSP_SYS_CLK_12MHZ
*           \li \b BSP_SYS_CLK_16MHZ
*           \li \b BSP_SYS_CLK_20MHZ
*           \li \b BSP_SYS_CLK_25MHZ
*
* @return   None
******************************************************************************/
void
bspInit(void)
{
    uint16_t ui16IntState;

    //
    // Stop watchdog timer (prevent timeout reset)
    //
    WDTCTL = WDTPW + WDTHOLD;

    //
    // Disable global interrupts
    //
    ui16IntState = __get_interrupt_state();
    __disable_interrupt();

    //
    //  Set capacitor values for XT1, 32768 Hz */
    //
//    bspMcuStartXT1();

    bspSysClockSpeedSet();
	
	P5SEL |= BIT4 + BIT5 + BIT3 +BIT2;


    //
    // Return to previous interrupt state
    //
    __set_interrupt_state(ui16IntState);
}


/**************************************************************************//**
* @brief    Function returns the system clock speed (in MHz) set by bspInit().
*
* @return   Returns the system clock speed (in MHz) set by bspInit().
******************************************************************************/
uint32_t
bspSysClockSpeedGet(void)
{
    return (ui32BspMclkSpeed);
}


/**************************************************************************//**
* @brief    This is an assert function. It runs an infinite loop that blinks
*           all LEDs quickly. The function assumes LEDs to be initialized by,
*           for example,  bspInit().
*
* @return   None
******************************************************************************/
void
bspAssert(void)
{
    // just loop forever
	for(;;);
}


void SetVcoreUp (unsigned int level)
{
	// Open PMM registers for write
	PMMCTL0_H = PMMPW_H;              
	// Set SVS/SVM high side new level
	SVSMHCTL = SVSHE | SVSHRVL0 * level | SVMHE | SVSMHRRL0 * level;
	// Set SVM low side to new level
	SVSMLCTL = SVSLE | SVMLE | SVSMLRRL0 * level;
	// Wait till SVM is settled
	while ((PMMIFG & SVSMLDLYIFG) == 0);
	// Clear already set flags
	PMMIFG &= ~(SVMLVLRIFG | SVMLIFG);
	// Set VCore to new level
	PMMCTL0_L = PMMCOREV0 * level;
	// Wait till new level reached
	if ((PMMIFG & SVMLIFG))
		while ((PMMIFG & SVMLVLRIFG) == 0);
	// Set SVS/SVM low side to new level
	SVSMLCTL = SVSLE | SVSLRVL0 * level | SVMLE | SVSMLRRL0 * level;
	// Lock PMM registers for write access
	PMMCTL0_H = 0x00;
}

/**************************************************************************//**
* @brief    This function sets the MCLK frequency. It also selects XT1 as ACLK
*           source with no divison in Low Power mode.
*
* @param    ui32SystemClockSpeed    is the intended frequency of operation.
*
* @return   None
******************************************************************************/
void
bspSysClockSpeedSet(void)
{
	// Increase Vcore setting to level3 to support fsystem=25MHz
	// NOTE: Change core voltage one level at a time..
	SetVcoreUp (0x01);
	SetVcoreUp (0x02);  
	SetVcoreUp (0x03);  
	
	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
	
	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
	UCSCTL2 = FLLD_0 | 487;                   // Set DCO Multiplier for 25MHz
	// (N + 1) * FLLRef = Fdco
	// (762 + 1) * 32768 = 25MHz
	// Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop
	
	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
	__delay_cycles(500000);
	
	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	do
	{
		UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
		// Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
	
	ui32BspMclkSpeed = BSP_SYS_CLK_16MHZ;
}

/**************************************************************************//**
* Close the Doxygen group.
* @}
******************************************************************************/
