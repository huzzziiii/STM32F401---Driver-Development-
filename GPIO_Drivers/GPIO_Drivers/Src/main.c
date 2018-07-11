/*
 * main.c

 */

#include "led.h"
//#include "stm32f4xx_gpio_driver.h"

void EXTI15_10_IRQHandler(void){
	//
//	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	hal_gpio_clear_interrupt(GPIO_PIN_13);
	led_toggle(GPIOA, GPIO_PIN_5);

}

int main(void){
	led_init();

	button_init();

	while(1){
		/* wait for the interrupt */
	}


	return 0;

}

