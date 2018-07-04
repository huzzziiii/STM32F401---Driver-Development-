/*
 * hal_spi_driver.h
 *
 *  Created on: Jul 3, 2018
 *      Author: huzaifaasif
 */

#ifndef STM32F4XX_HAL_DRIVER_INC_HAL_SPI_DRIVER_H_
#define STM32F4XX_HAL_DRIVER_INC_HAL_SPI_DRIVER_H_

#include "stm32f401xe.h"

/******************************************************************************/
/*                                                                            */
/*                        1. Serial Peripheral Interface                         */
/*                           Register Bit Definitions                          */
/******************************************************************************/

/*******************  Bit definition for SPI_CR1 register  ********************/

//Bidirectional data mode enable
#define SPI_CR1_BIDIMODE 								(uint32_t) 1 << 15
#define SPI_ENABLE_2_LINE_UNI_DIR        				0
#define SPI_ENABLE_1_LINE_BIDI           				1

//Data frame format
#define SPI_CR1_DFF										(uint32_t) 1 << 11
#define SPI_8_BIT_DFF_ENABLE								0
#define SPI_16_BIT_DFF_ENABLE							1

//Software slave management
#define SPI_CR1_SSM										(uint32_t) 1 << 9
#define SPI_CR1_SSM_DISABLE								0
#define SPI_CR1_SSM_ENABLE								1

// Internal slave select
#define SPI_CR1_SSI 									   (uint32_t) 1 << 8

// LSBFIRST -- Frame Format: Data can be shifted out either MSB-first or LSB-first depending on the value of the LSBFIRST bit in the SPI_CR1 Register.
#define SPI_CR1_LSB_FIRST							   (uint32_t) 1 << 7
#define SPI_TX_MSB_FIRST									0
#define SPI_TX_LSB_FIRST									1

//SPI enable
#define SPI_EN										   (uint32_t) 1 << 6

//BAUD RATE
#define SPI_CR1_BR_PCLK_DIV_2							(uint32_t) 0 << 3
#define SPI_CR1_BR_PCLK_DIV_4							(uint32_t) 1 << 3
#define SPI_CR1_BR_PCLK_DIV_8							(uint32_t) 2 << 3
#define SPI_CR1_BR_PCLK_DIV_16							(uint32_t) 3 << 3
#define SPI_CR1_BR_PCLK_DIV_32							(uint32_t) 4 << 3
#define SPI_CR1_BR_PCLK_DIV_64							(uint32_t) 5 << 3
#define SPI_CR1_BR_PCLK_DIV_128							(uint32_t) 6 << 3
#define SPI_CR1_BR_PCLK_DIV_256							(uint32_t) 7 << 3

//Master selection
#define SPI_CR1_MSTR										(uint32_t) 1 << 2
#define SPI_SLAVE_MODE_SEL								0
#define SPI_MASTER_MODE_SEL								1

//CLOCK POLARITY
#define SPI_CR1_CPOL										(uint32_t) 1 << 1
#define SPI_CPOL_LOW										0
#define SPI_CPOL_HIGH									1

//CLOCK PHASE
#define SPI_CR1_CPHA									(uint32_t) 1 << 0
#define SPI_LEADING_CLK									0
#define SPI_TRAILING_CLK									1


/*******************  Bit definition for SPI_CR2 register  ********************/
// TX buffer empty interrupt enable
#define SPI_CR2_TXEIE								(uint32_t) 1 << 7
#define SPI_TXE_INT_MASKED							0
#define SPI_TXE_INT_UNMASKED							1

//RX buffer not empty interrupt enable
#define SPI_CR2_RXNEIE								(uint32_t) 1 << 6
#define SPI_RXNEIE_INT_MASKED						0
#define SPI_RXNEIE_INT_UNMASKED						1

// Error interrupt enable
#define SPI_CR2_ERRIE								(uint32_t) 1 << 5
#define SPI_ERR_INT_MASKED							0
#define SPI_ERR_INT_UNMASKED							1

// FRAME FORMAT
#define SPI_CR2_ERRIE								(uint32_t) 1 << 4
#define SPI_MOTOROLA_MODE							0
#define SPI_TI_MODE									1

//  SS output enable
#define SPI_CR2_ERRIE								(uint32_t) 1 << 2

/*******************  Bit definition for SPI_SR (STATUS_REG) register  ********************/
//  Frame format error
#define SPI_SR_FRE									(uint32_t) 1 << 8

//BUSY FLAG
#define SPI_SR_BSY									(uint32_t) 1 << 7

//OVERRUN FLAG
#define SPI_SR_OVR									(uint32_t) 1 << 6

//TRANSMIT BUFFER EMPTY
#define SPI_SR_TXE									(uint32_t) 1 << 1

//RECEIVE BUFFER NOT EMPTY
#define SPI_SR_RXNE									(uint32_t) 1 << 0

/* Macros to Enable Clock for SPI devices */
#define _HAL_RCC_SPI1_CLK_ENABLE()				    RCC->APB2ENR |= 1 << 12
#define _HAL_RCC_SPI2_CLK_ENABLE()				    RCC->APB1ENR |= 1 << 14
#define _HAL_RCC_SPI3_CLK_ENABLE()				    RCC->APB1ENR |= 1 << 15


/**
  * @brief  SPI Configuration Structure definition
  */

typedef struct{
	uint32_t Mode;
	uint32_t Direction;	//single line Bi-Di mode OR 2-line uni-direction mode
	uint32_t Data_Size;		//8 or 16 bit data transmission
	uint32_t CLK_Polarity;
	uint32_t CLK_Phase;
	uint32_t Baud_Rate;
	uint32_t NSS;		//sets hardware of software slave select management using SSM bit in SPI_CR1 reg
	uint32_t First_Bit; 		//MSB or LSB
} SPI_Init;

typedef enum{
	HAL_SPI_STATE_RESET 		= 0x00,
	HAL_SPI_STATE_READY		= 0x01,
	HAL_SPI_STATE_BUSY		= 0x02
} hal_spi_state;

/**
 * @brief SPI handle structure definition
 */
typedef struct __spi_handle_t {
	SPI_TypeDef		*Instance;	//SPI registers base address
	SPI_Init			Init;		//SPI communication protocols
} spi_handle;


/******************************************************************************/
/*                                                                            */
/*                      3. Driver exposed APIs                                   */
/*                                                                            */
/******************************************************************************/

void hal_spi_init(spi_handle *spi_hndlr);

void hal_spi_irq_handler(spi_handle *hspi);
#endif /* STM32F4XX_HAL_DRIVER_INC_HAL_SPI_DRIVER_H_ */
