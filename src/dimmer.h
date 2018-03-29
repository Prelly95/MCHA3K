#ifndef DIMMER_H
#define DIMMER_H

#include <stdint.h>

enum {
    DIMMER_MIN_STEP             = 1,
    DIMMER_MAX_STEP             = 64,
    DIMMER_MIN_BRIGHTNESS       = 0,
    DIMMER_MAX_BRIGHTNESS       = 255,
    DIMMER_DEFAULT_BRIGHTNESS   = 128,
    DIMMER_DEFAULT_STEP         = 25,
};

void    dimmer_init(void);
void    dimmer_set_brightness(uint8_t);
uint8_t dimmer_get_brightness(void);
void    dimmer_set_step(uint8_t);
uint8_t dimmer_get_step(void);
void    dimmer_increment(void);
void    dimmer_decrement(void);
void    dimmer_change(int32_t);

#endif