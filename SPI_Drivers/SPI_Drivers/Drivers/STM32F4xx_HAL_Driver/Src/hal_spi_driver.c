/*
 * hal_spi_driver.c
 *
 */
#include "hal_spi_driver.h"


void hal_spi_init(spi_handle_t *spi_handle){

	/* configure the spi device mode */
	hal_spi_config_device_mode(spi_handle->Instance, spi_handle->Init.Mode);

	/* configure the phase and polarity */
	hal_spi_config_phase_polarity(spi_handle->Instance, spi_handle->Init.CLK_Phase, spi_handle->Init.CLK_Polarity);

	/* configure the spi data size */
	hal_spi_config_data_size_direction(spi_handle->Instance, spi_handle->Init.Data_Size, spi_handle->Init.First_Bit);

	/* configure the slave select line */
	if (spi_handle->Init.Mode == SPI_MASTER_MODE_SEL){
		hal_spi_config_nss_master(spi_handle->Instance, spi_handle->Init.NSS);
	} else {
		hal_spi_config_nss_slave(spi_handle->Instance, spi_handle->Init.NSS);
	}

	/* configure SPI device baud rate */
	hal_spi_configure_baudrate(spi_handle->Instance, spi_handle->Init.Baud_Rate);

	/* configure the SPI device direction */
	hal_spi_configure_direction(spi_handle->Instance, spi_handle->Init.Direction);

	/* ENABLING SPI */
	hal_spi_enable(spi_handle->Instance);
}

/******************************************************************************/
/*                                                                            */
/*                      Helper functions                                 */
/*                                                                            */
/******************************************************************************/

/* Configures SPI baudrate */
void hal_spi_configure_baudrate(SPI_TypeDef *SPIx, uint32_t prescalar){
	SPIx->CR1 &= ~(7 << 3);
	SPIx->CR1 |= prescalar;

//	if (prescalar > 7){
//		SPIx->CR1 |= (0 << 3);		///  &= ??
//	}else{
//		SPIx->CR1 |= prescalar << 3;
//	}
}

/* Configures SPI direction */
void hal_spi_configure_direction(SPI_TypeDef *SPIx, uint32_t direction){
	if(direction){
		SPIx->CR1 |= SPI_REG_CR1_BIDIMODE;
	}else{
		SPIx->CR1 &= ~SPI_REG_CR1_BIDIMODE;
	}

}

/**
 * Configures master or slave mode
 */
void hal_spi_config_device_mode(SPI_TypeDef *SPIx, uint32_t master){
	if (master){
		SPIx->CR1 |= SPI_CR1_MSTR;
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_MSTR;
	}
}

void hal_spi_config_phase_polarity(SPI_TypeDef *SPIx, uint32_t phase, uint32_t polarity){
	if (phase){
		SPIx->CR1 |= SPI_CR1_CPHA;
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_CPHA;
	}

	if (polarity){
		SPIx->CR1 |= SPI_CR1_CPOL;
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_CPOL;
	}
}

// BY DEFAULT, SPI SENTS 8-bit data with MSB first
void hal_spi_config_data_size_direction(SPI_TypeDef *SPIx, uint32_t datasize_16, uint32_t lsb_first){
	if (datasize_16){
		SPIx->CR1 |= SPI_CR1_DFF;	//16 bit
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_DFF;	//8 bit
	}

	if (lsb_first){
		SPIx->CR1 |= SPI_CR1_LSB_FIRST;	//send out LSB first
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_LSB_FIRST;	//send out MSB first
	}
}

//Software/hardware slave management (SSM)
void hal_spi_config_nss_master(SPI_TypeDef *SPIx, uint32_t ssm_enable){
	if (ssm_enable){
		SPIx->CR1 |= SPI_CR1_SSM;		//software management
		SPIx->CR1 |= SPI_CR1_SSI;		//NSS pin takes on SSI's value == 1
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_SSM;		//hardware management

	}
}

void hal_spi_config_nss_slave(SPI_TypeDef *SPIx, uint32_t ssm_enable){
	if (ssm_enable){
		SPIx->CR1 |= SPI_CR1_SSM;
	}
	else{
		SPIx->CR1 &= ~SPI_CR1_SSM;
	}
}

//Enable SPI
void hal_spi_enable(SPI_TypeDef *SPIx){
	if (!(SPIx->CR1 & SPI_EN)){		//Enabling SPI if it isn't enabled
		SPIx->CR1 |= SPI_EN;
	}
}
//Disable SPI
void hal_spi_disable(SPI_TypeDef *SPIx){
	SPIx->CR1 &= ~SPI_EN;
}

/******************************************************************************/
/*                                                                            */
/*                     	Interrupts											*/
/******************************************************************************/

/* Enabling interrupt for EMPTY TRANSMIT REG */
void hal_spi_enable_txe_interrupt(SPI_TypeDef *SPIx){
	SPIx->CR2 |= SPI_CR2_TXEIE;	//enabling TX buffer empty interrupt
}

/* Enabling interrupt for NON-EMPTY RECEIVE REG */
void hal_spi_enable_rxne_interrupt(SPI_TypeDef *SPIx){
	SPIx->CR2 |= SPI_CR2_RXNEIE;
}

/* Disabling the TX buffer empty interrupt (TXE) */
void hal_spi_disable_txe_interrupt(SPI_TypeDef *SPIx){
	SPIx->CR2 &= ~SPI_CR2_TXEIE;
}
/* Disabling the RX buffer non empty interrupt (RXNEIE) */
void hal_spi_disable_rxne_interrupt(SPI_TypeDef *SPIx){
	SPIx->CR2 &= ~SPI_CR2_RXNEIE;
}


/* 			-------HANDLERS--------			*/

/* Closing TX transfer */
void hal_spi_close_tx_interrupt(spi_handle_t *hspi){
	//--------------
	hal_spi_disable_txe_interrupt(hspi->Instance);
	//hspi->state = HAL_SPI_STATE_READY;
}
/* Closing RX transfer */
void hal_spi_close_rx_interrupt(spi_handle_t *hspi){
	//---------------
	hal_spi_disable_rxne_interrupt(hspi->Instance);
	hspi->state = HAL_SPI_STATE_READY;
}

/* Handles RXNE interrupt */
void hal_spi_handle_rx_interrupt(spi_handle_t *hspi){
	//receive data in 8-bit mode
	if (hspi->Init.Data_Size == SPI_8_BIT_DFF_ENABLE){
		*(hspi->rxBuffer) = hspi->Instance->DR; 	//reading off of a DR
		hspi->rx_count--;
	}
	//receive data in 16-bit mode
	else{
		*((uint16_t*)hspi->rxBuffer) = hspi->Instance->DR;
		hspi->rx_count-=2;
	}

	/*Done with the RXing of data; closing the RXNE interrupt */
	if (hspi->rx_count == RESET){
		hal_spi_close_rx_interrupt(hspi);
	}

}
/* handling TX interrupt: TX_Buffer = empty(pushed to shift register); peripheral wants to put new data */
void hal_spi_handle_tx_interrupt(spi_handle_t *hspi){

	//transmit data in 8-bit mode
	if (hspi->Init.Data_Size == SPI_8_BIT_DFF_ENABLE){
		hspi->Instance->DR = *hspi->txBuffer++;	//MISO -- TX != empty
		hspi->tx_count--;	//sent 1 byte
	}
	//transmit data in 16-bit mode
	else{
		hspi->Instance->DR = *((uint16_t*)hspi->txBuffer);
		hspi->tx_count-=2;	//sent 2 bytes
	}

	if (hspi->tx_count == RESET){
		/* close TXE interrupt */
		hal_spi_close_tx_interrupt(hspi);
	}

}
//----------------------------------------
uint32_t begin_spi(spi_handle_t *hspi){
	hspi->Instance->DR = *hspi->txBuffer++;		//fill in DR -- TX != empty

	while(!(hspi->Instance->SR & SPI_SR_TXE)){}	//wait till data from DR ----> MOSI (TX == EMPTY)
	while(!(hspi->Instance->SR & SPI_SR_RXNE)){}	//wait till data from MISO ----> DR (RXNE != EMPTY)

	uint32_t val = hspi->Instance->DR;	//read out from DR -- RX buffer = empty (RXNE = 0)
	return val;
}

uint8_t SPI_transfer(spi_handle_t *hspi, uint8_t data){
	while (!(hspi->Instance->SR & SPI_SR_TXE)){}	//Wait till TXE = EMPTY
	hspi->Instance->DR = data;

	while(!(hspi->Instance->SR & SPI_SR_RXNE)){}	//Wait till RXNE = NOT EMPTY
	uint8_t value = hspi->Instance->DR;

	return value;
}

uint32_t SPItransfer(spi_handle_t *hspi, uint8_t *txBuffer, uint8_t size){
	uint32_t temp=0, data;
	hspi->txBuffer = txBuffer;
	hspi->TX_tranfer_size = size;

	//checking if TXE is set in SR -- Ready to write (TX buffer = empty)
	temp = hspi->Instance->SR & (SPI_SR_TXE);

	while(size){
		data = begin_spi(hspi);
		size--;
	}
	return data;
}


//----------------------------------------
void hal_spi_irq_handler(spi_handle_t *hspi){

	uint32_t temp1 = 0, temp2 = 0;

	//checking if RX buffer is not empty -- Ready to read in data (RX buffer != empty)
	temp1 = (hspi->Instance->SR & SPI_SR_RXNE);
	//checking if RXNEIE bit is enabled in CR
	temp2 = (hspi->Instance->CR2 & SPI_CR2_RXNEIE);


	if (temp1!= RESET && temp2!= RESET){		//ready to READ in a data
		hal_spi_handle_rx_interrupt(hspi);	//handling RXNE interrupt
		//return;
	}

	//checking if TX -- Ready to write (TX buffer = empty)
	temp1 = (hspi->Instance->SR & SPI_SR_TXE);
	//checking if TXEIE bit is enabled in CR
	temp2 = (hspi->Instance->CR2 & SPI_CR2_TXEIE);

	if (temp1!= RESET && temp2!= RESET){	   //TX = empty -- ready to WRITE
		hal_spi_handle_tx_interrupt(hspi); //handling TXE interrupt
		//return;
	}
}



/******************************************************************************/
/*                                                                            */
/*                      API functions                                 */
/*                                                                            */
/******************************************************************************/

void interrupts_SPI_transfer(spi_handle_t *spi_handle, uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t length){
	spi_handle->txBuffer = tx_buffer;
	spi_handle->rxBuffer = rx_buffer;
	spi_handle->TX_tranfer_size = length;
	spi_handle->tx_count = length;

	spi_handle->rx_count = length; 	//RXing same amount of bytes as TX bytes

	spi_handle->state = HAL_SPI_STATE_BUSY_TX;

	hal_spi_enable_txe_interrupt(spi_handle->Instance);
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
	hal_spi_enable(spi_handle->Instance);
}



//API used for master data transmission
void hal_spi_master_tx(spi_handle_t *spi_handle, uint8_t *tx_buffer, uint32_t length){
	spi_handle->txBuffer = tx_buffer;
	spi_handle->TX_tranfer_size = length;
	spi_handle->tx_count = length;

	spi_handle->state = HAL_SPI_STATE_BUSY_TX; //setting the state for transmitting data

	//current data gets loaded onto the shift register -- TX buffer = empty
	hal_spi_enable_txe_interrupt(spi_handle->Instance); //enabling TX interrupt for empty TX Buffer
	hal_spi_enable(spi_handle->Instance);	//enabling SPI communication

}
//API used for master data reception
void hal_spi_master_rx(spi_handle_t *spi_handle, uint8_t *rx_buffer, uint32_t length){
	int32_t val;

	/* dummy TX */
	spi_handle->txBuffer = rx_buffer;
	spi_handle->TX_tranfer_size = length;
	spi_handle->tx_count = length;

	/* Address of RX Buffer is stored*/
	spi_handle->rxBuffer = rx_buffer;
	spi_handle->RX_tranfer_size = length;
	spi_handle->rx_count = length;

	/*Driver is busy in RX */
	spi_handle->state = HAL_SPI_STATE_BUSY_RX;

	/* Enabling SPI Interrupt */
	hal_spi_enable(spi_handle->Instance);

	/* reading DR to see if it's empty */
	val = spi_handle->Instance->DR;

	/* Enabling both TXE and RXNE Interrupts */
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
	hal_spi_enable_txe_interrupt(spi_handle->Instance);
}

/* --- SLAVE ---- */
// API for SLAVE data transmission
void hal_spi_slave_tx(spi_handle_t *spi_handle, uint8_t *tx_buffer, uint32_t length){

	/* Address of RX Buffer is stored */
	spi_handle->txBuffer = tx_buffer;
	spi_handle->TX_tranfer_size = length;
	spi_handle->tx_count = length;

	/* Dummy RX -- receiving clock from the Master */
	spi_handle->rxBuffer = tx_buffer;
	spi_handle->RX_tranfer_size = length;
	spi_handle->rx_count = length;

	/*Driver is busy in TX */
	spi_handle->state = HAL_SPI_STATE_BUSY_TX;

	/* Enabling SPI Interrupt */
	hal_spi_enable(spi_handle->Instance);

	/* Enabling both TXE and RXNE Interrupts */
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
	hal_spi_enable_txe_interrupt(spi_handle->Instance);
}

// API for SLAVE data reception
void hal_spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rx_buffer, uint32_t length){
	spi_handle->rxBuffer = rx_buffer;
	spi_handle->RX_tranfer_size = length;
	spi_handle->rx_count = length;

	spi_handle->state = HAL_SPI_STATE_BUSY_RX;

	/* Enabling SPI Interrupt */
	hal_spi_enable(spi_handle->Instance);

	/* Enabling RXNE Interrupt -- triggered when data is received from Master */
	hal_spi_enable_rxne_interrupt(spi_handle->Instance);
}








