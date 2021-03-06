/*
 * main.c

 */

#include "led.h"
//#include "stm32f4xx_gpio_driver.h"

void EXTI15_10_IRQHandler(void){
	led_toggle(GPIOA, GPIO_PIN_5);
}

int main(void){
	led_init();

	hal_gpio_configure_interrupt(GPIO_BUTTON_PIN, INT_FALLING_EDGE);
	hal_gpio_enable_interrupt(GPIO_BUTTON_PIN, EXTI15_10_IRQn);


	return 0;

}
