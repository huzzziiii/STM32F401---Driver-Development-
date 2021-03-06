/*
 * led.h
 *
 */
#include "stm32f4xx_gpio_driver.h"

#ifndef LED_H_
#define LED_H_

#define GPIO_PIN_5			5		//LED -- PA5
#define GPIO_PIN_13			13		//BUTTON -- PC13
#define GPIO_PIN_10			10		//SLAVE -- Trigger

void led_init(void);
void button_init(void);
void led_turn_on(GPIO_TypeDef *GPIOx, uint16_t pin);
void led_turn_off(GPIO_TypeDef *GPIOx, uint16_t pin);
void led_toggle(GPIO_TypeDef *GPIOx, uint16_t pin);

#endif /* LED_H_ */
