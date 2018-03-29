#ifndef LED_H
#define LED_H

#include <stdint.h>

void    led_init(void);
void    led_on(void);
void    led_off(void);
void    led_set_brightness(uint8_t);
uint8_t led_get_brightness(void);
void timer0_init(void);

#endif
