#include "rf_conf.h"

#include "hal_spi_rf_trxeb.h"
#include "cc120x_spi.h"
#include "io_pin_int.h"
#include "bsp.h"
#include "bsp_uart.h"

#define CC1190_CONTROL_PORT_SEL	P6SEL
#define CC1190_CONTROL_PORT_DIR	P6DIR
#define CC1190_CONTROL_PORT_OUT	P6OUT
#define CC1190_CONTROL_PORT_IN	P6IN

#define CC1190_LNA_EN_PIN		BIT1
#define CC1190_PA_EN_PIN		BIT2
#define CC1190_HGM_PIN			BIT0 // require alway high or unconnect to MCU

#define MAX_DATA_LENGTH		255


#define ISR_ACTION_REQUIRED 1
#define ISR_IDLE            0
#define RX_FIFO_ERROR       0x11

#define GPIO_RXTX BIT6

void HgmEnable(void);
void HgmDisable(void);
void PaEnable(void);
void PaDisable(void);
void LnaEnable(void);
void LnaDisable(void);

void registerConfig(void);



static const registerSetting_t preferredSettings[]= 
{
  {CC120X_IOCFG2,            0x06},
  {CC120X_SYNC_CFG1,         0xA8},
  {CC120X_DEVIATION_M,       0x47},
  {CC120X_MODCFG_DEV_E,      0x2F},
  {CC120X_DCFILT_CFG,        0x1E},
  {CC120X_PREAMBLE_CFG0,     0x8A},
  {CC120X_IQIC,              0x00},
  {CC120X_CHAN_BW,           0x01},
  {CC120X_MDMCFG1,           0x42},
  {CC120X_MDMCFG0,           0x05},
  {CC120X_SYMBOL_RATE2,      0xC9},
  {CC120X_SYMBOL_RATE1,      0x99},
  {CC120X_SYMBOL_RATE0,      0x99},
  {CC120X_AGC_REF,           0x2F},
  {CC120X_AGC_CS_THR,        0xF8},
  {CC120X_AGC_CFG2,          0x60},
  {CC120X_AGC_CFG1,          0x12},
  {CC120X_AGC_CFG0,          0x84},
  {CC120X_FIFO_CFG,          0x00},
  {CC120X_FS_CFG,            0x12},
  {CC120X_PKT_CFG2,          0x00},
  {CC120X_PKT_CFG0,          0x20},
  {CC120X_PKT_LEN,           0xFF},
  {CC120X_FREQOFF_CFG,       0x23},
  {CC120X_MDMCFG2,           0x00},
  {CC120X_FREQ2,             0x56},
  {CC120X_FREQ1,             0xCC},
  {CC120X_FREQ0,             0xCC},
  {CC120X_IF_ADC1,           0xEE},
  {CC120X_IF_ADC0,           0x10},
  {CC120X_FS_DIG1,           0x04},
  {CC120X_FS_DIG0,           0xA3},
  {CC120X_FS_CAL1,           0x40},
  {CC120X_FS_CAL0,           0x0E},
  {CC120X_FS_DIVTWO,         0x03},
  {CC120X_FS_DSM0,           0x33},
  {CC120X_FS_DVC1,           0xF7},
  {CC120X_FS_DVC0,           0x0F},
  {CC120X_FS_PFD,            0x00},
  {CC120X_FS_PRE,            0x6E},
  {CC120X_FS_REG_DIV_CML,    0x1C},
  {CC120X_FS_SPARE,          0xAC},
  {CC120X_FS_VCO0,           0xB5},
  {CC120X_IFAMP,             0x0D},
  {CC120X_XOSC5,             0x0E},
  {CC120X_XOSC1,             0x03},
};

static uint8  packetSemaphore;

static void radioTxRxISR(void);

void RF_Send(uint8_t * buff, uint8_t len) ;
uint8_t RF_Recv(uint8_t * buff, uint8_t len) ;

void RF_Config(void)
{	
	// Instantiate transceiver RF SPI interface to SCLK ~ 4 MHz
	// Input parameter is clockDivider
	// SCLK frequency = SMCLK/clockDivider
	trxRfSpiInterfaceInit(16);
	
	registerConfig();
	
	// set P1.7 as input
	P1SEL &= ~(GPIO_RXTX);  // Set pin to zero for I/O function
	P1DIR &= ~(GPIO_RXTX);  // Set pin direction to output
	
	// Connect ISR function to GPIO1.7
	ioPinIntRegister(IO_PIN_PORT_1, GPIO_RXTX, &radioTxRxISR);
	
	// Interrupt on falling edge
	ioPinIntTypeSet(IO_PIN_PORT_1, GPIO_RXTX, IO_PIN_FALLING_EDGE);
	
	// Clear ISR flag
	ioPinIntClear(IO_PIN_PORT_1, GPIO_RXTX);
	
	// Enable interrupt
	ioPinIntEnable(IO_PIN_PORT_1, GPIO_RXTX);
	
	
	//	// Connect ISR function to GPIO7
	//	ioPinIntRegister(IO_PIN_PORT_1, GPIO_TX, &radioRxISR);
	//	
	//	// Interrupt on falling edge
	//	ioPinIntTypeSet(IO_PIN_PORT_1, GPIO_TX, IO_PIN_FALLING_EDGE);
	//	
	//	// Clear ISR flag
	//	ioPinIntClear(IO_PIN_PORT_1, GPIO_TX);
	//	
	//	// Enable interrupt
	//	ioPinIntEnable(IO_PIN_PORT_1, GPIO_TX);
	
	
	// Set radio in RX
	trxSpiCmdStrobe(CC120X_SRX);
}

void RF_Send(uint8_t * buff, uint8_t len) 
{
	PaEnable();
	LnaDisable();
	HgmEnable();
	
	// Write radio registers
	registerConfig();
	
	// Write packet to tx fifo
	cc120xSpiWriteTxFifo(buff, len);
	
	// Strobe TX to send packet
	trxSpiCmdStrobe(CC120X_STX);
	
	// Wait for interrupt that packet has been sent. 
	// (Assumes the GPIO connected to the radioRxTxISR function is set 
	// to GPIOx_CFG = 0x06)
    while(packetSemaphore != ISR_ACTION_REQUIRED);
	
	// Clear semaphore flag
	packetSemaphore = ISR_IDLE;
	
	// Write radio registers
	registerConfig();
	
	// Set radio in RX
	trxSpiCmdStrobe(CC120X_SRX);
	
	
	PaDisable();
	LnaEnable();
	HgmEnable();
}

void radioTxRxISR(void) 
{
	// Set packet semaphore
	packetSemaphore = ISR_ACTION_REQUIRED;
	
	// Clear ISR flag
	ioPinIntClear(IO_PIN_PORT_1, GPIO_RXTX);
}

uint8_t RF_Recv(uint8_t * buff, uint8_t len) 
{
	uint8_t resLen = 0;
	uint8 marcStatus;
		
	if(packetSemaphore == ISR_ACTION_REQUIRED)
	{
		// Read number of bytes in rx fifo
		cc120xSpiReadReg(CC120X_NUM_RXBYTES, &resLen, 1);
		
		// Check that we have bytes in fifo
		if(resLen != 0){
			
			// Read marcstate to check for RX FIFO error
			cc120xSpiReadReg(CC120X_MARCSTATE, &marcStatus, 1);
			
			// Mask out marcstate bits and check if we have a RX FIFO error
			if((marcStatus & 0x1F) == RX_FIFO_ERROR){
				
				// Flush RX Fifo
				trxSpiCmdStrobe(CC120X_SFRX);
			}
			else{
				
				// Read n bytes from rx fifo
				cc120xSpiReadRxFifo(buff, resLen);  
				
				//				// Check CRC ok (CRC_OK: bit7 in second status byte)
				//				// This assumes status bytes are appended in RX_FIFO
				//				// (PKT_CFG1.APPEND_STATUS = 1.)
				//				// If CRC is disabled the CRC_OK field will read 1
				//				if(rxBuffer[rxBytes-1] & 0x80){
				//					
				//					// Update packet counter
				//					packetCounter++;
				//				}
			}
		}
		
		// Reset packet semaphore
		packetSemaphore = ISR_IDLE;
		
		// Set radio back in RX
		trxSpiCmdStrobe(CC120X_SRX);
	}
	
	return resLen;
}

void PaEnable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_PA_EN_PIN);  // Set pin to zero for I/O function
	CC1190_CONTROL_PORT_DIR |=  (CC1190_PA_EN_PIN);  // Set pin direction to output
	CC1190_CONTROL_PORT_OUT |=  (CC1190_PA_EN_PIN);  // Set output pin high
}

void LnaEnable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_LNA_EN_PIN);
	CC1190_CONTROL_PORT_DIR |=  (CC1190_LNA_EN_PIN);
	CC1190_CONTROL_PORT_OUT |=  (CC1190_LNA_EN_PIN);
}

void PaDisable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_PA_EN_PIN);  // Set pin to zero for I/O function
	CC1190_CONTROL_PORT_DIR |=  (CC1190_PA_EN_PIN);  // Set pin direction to output
	CC1190_CONTROL_PORT_OUT &= ~(CC1190_PA_EN_PIN);  // Set output pin low
}

void LnaDisable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_LNA_EN_PIN);
	CC1190_CONTROL_PORT_DIR |=  (CC1190_LNA_EN_PIN);
	CC1190_CONTROL_PORT_OUT &= ~(CC1190_LNA_EN_PIN);    
}

void HgmEnable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_HGM_PIN);
	CC1190_CONTROL_PORT_DIR |=  (CC1190_HGM_PIN);
	CC1190_CONTROL_PORT_OUT |=  (CC1190_HGM_PIN);
}

void HgmDisable(void)
{
	CC1190_CONTROL_PORT_SEL &= ~(CC1190_HGM_PIN);
	CC1190_CONTROL_PORT_DIR |=  (CC1190_HGM_PIN);
	CC1190_CONTROL_PORT_OUT &= ~(CC1190_HGM_PIN);
}


void registerConfig(void)
{
	
	uint8 writeByte;
	
	// Reset radio
	trxSpiCmdStrobe(CC120X_SRES);
	
	// Write registers to radio
	for(uint16 i = 0; i < (sizeof  preferredSettings/sizeof(registerSetting_t)); i++) {
		writeByte =  preferredSettings[i].data;
		cc120xSpiWriteReg( preferredSettings[i].addr, &writeByte, 1);
	}
}

