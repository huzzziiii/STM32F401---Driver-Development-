/*
 * led.c

 */

#include "led.h"

void led_init(void){
	GPIO_PIN_CONFIG led_pin_config;			//PA5
	_HAL_RCC_GPIOA_CLK_ENABLE();

	led_pin_config.pin = GPIO_PIN_5;
	led_pin_config.mode = GPIO_PIN_OUTPUT_MODE;
	led_pin_config.op_type = GPIO_PUSH_PULL;
	led_pin_config.speed = GPIO_MEDIUM_SPEED;
	led_pin_config.pull = GPIO_NO_PUPD;

	//led init call
	hal_gpio_init(GPIOA, &led_pin_config);
}

void led_turn_on(GPIO_TypeDef *GPIOx, uint16_t pin){
	hal_gpio_write_to_pin(GPIOx, 1, pin);
}

void led_turn_off(GPIO_TypeDef *GPIOx, uint16_t pin){
	hal_gpio_write_to_pin(GPIOx, 0, pin);
}

void led_toggle(GPIO_TypeDef *GPIOx, uint16_t pin){

	if (hal_gpio_read_from_pin(GPIOx, pin)){
		hal_gpio_write_to_pin(GPIOx, 0, pin);
	}
	else{
		hal_gpio_write_to_pin(GPIOx, 1, pin);
	}
}

