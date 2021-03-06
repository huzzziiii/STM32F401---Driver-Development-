/*
 * stm32f4xx_gpio_driver.c
 *
*/
#include "stm32f4xx_gpio_driver.h"

//INTERRUPTS
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_select edge_sel){
	if (edge_sel == INT_RISING_EDGE){ // @suppress("Symbol is not resolved")
		EXTI->RTSR |= 1 << pin_no;
	}
	else if (edge_sel == INT_FALLING_EDGE){
		EXTI->FTSR |= 1 << pin_no;
	}
	else if (edge_sel == INT_RISING_FALLING_EDGE){
		EXTI->RTSR |= 1 << pin_no;
		EXTI->FTSR |= 1 << pin_no;
	}
}

void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no){
	EXTI->IMR |= 1 << pin_no;	//enabling the interrupt on EXTI controller using interrupt mask reg
	NVIC_EnableIRQ(irq_no);		//enabling the interrupt on NVIC for the specified IRQ_no

}

//clearing the pending bit in PR if interrupt has occurred on the specified pin
void hal_gpio_clear_interrupt(uint16_t pin_no){
	if (EXTI->PR & (1<<pin_no)){
		EXTI->PR |= 1 << pin_no;		//clearing the bit
	}
}

void hal_gpio_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t alt_val){
	if (pin <= 7){
		GPIOx->AFR[0] |= alt_val << (4 * pin);
	}
	else{
		GPIOx->AFR[1] |= alt_val << ( (pin % 8) * 4);
	}
}

uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin){
	uint8_t value;
	value = ((GPIOx->IDR >> pin) & 0x00000001);	///----GET IT !!?!

	return value;
}

void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint8_t value, uint16_t pin){
	if (value){
		GPIOx->ODR |= 1 << pin;
	}
	else{
		GPIOx->ODR &= ~(1 << pin);
	}
}

void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t mode){
	GPIOx->MODER |= mode << (2 * pin);
}

void hal_gpio_configure_pin_output_type(GPIO_TypeDef *GPIOx, uint16_t pin, uint16_t otype){
	GPIOx->OTYPER |= otype << pin;
}

void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t speed){
	GPIOx->OSPEEDR |= speed << (2 * pin);
}

void hal_gpio_configure_pin_pupd(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t pupd){
	GPIOx->PUPDR |= pupd << (2 * pin);
}

// -------helper functions-----------
void hal_gpio_init(GPIO_TypeDef *GPIOx, GPIO_PIN_CONFIG *gpio_pin){
	hal_gpio_configure_pin_mode(GPIOx, gpio_pin->pin, gpio_pin->mode);
	hal_gpio_configure_pin_output_type(GPIOx, gpio_pin->pin, gpio_pin->op_type);
	hal_gpio_configure_pin_speed(GPIOx, gpio_pin->pin, gpio_pin->speed);
	hal_gpio_configure_pin_pupd(GPIOx, gpio_pin->pin, gpio_pin->pull);
}


