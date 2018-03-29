#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "encoder.h"
#include "potentiometer.h"
#include "dimmer.h"

int main(void)
{
    led_init();
    encoder_init();
    pot_init();
    dimmer_init();

    sei();      // Enable global interrupts

    for (;;)
    {
        // Read potentiometer value and set dimmer step
        uint8_t step = ((DIMMER_MAX_STEP - DIMMER_MIN_STEP)*pot_get_value())/POT_FULL_SCALE
        + DIMMER_MIN_STEP;
        dimmer_set_step(step);
        // Process pending encoder increments
        int32_t count = encoder_pop_count();
        dimmer_change(count);
        // Update LED brightness
        uint8_t brightness = dimmer_get_brightness();
        led_set_brightness(brightness);
    }

    return 0;
}
