#include <stdint.h>
#include "dimmer.h"

static uint8_t _brightness, _step;

void dimmer_init(void)
{
    _brightness = DIMMER_DEFAULT_BRIGHTNESS;
    _step       = DIMMER_DEFAULT_STEP;
}

void dimmer_set_brightness(uint8_t brightness)
{
    _brightness = brightness;
}

uint8_t dimmer_get_brightness(void)
{
    return _brightness;
}

void dimmer_set_step(uint8_t step)
{
    if(step <= DIMMER_MIN_STEP)
    {
        _step = DIMMER_MIN_STEP;
    }
    else if(step >= DIMMER_MAX_STEP)
    {
      _step = DIMMER_MAX_STEP;
    }
    else
    {
      _step = step;
    }
}

uint8_t dimmer_get_step(void)
{
    return _step;
}

void dimmer_increment(void)
{
    if(_brightness + _step >= DIMMER_MAX_BRIGHTNESS)
    {
        _brightness = DIMMER_MAX_BRIGHTNESS;
    }
    else
    {
      _brightness += _step;
    }
}

void dimmer_decrement(void)
{
  if(_brightness - _step <= DIMMER_MIN_BRIGHTNESS)
  {
      _brightness = DIMMER_MIN_BRIGHTNESS;
  }
  else
  {
      _brightness -= _step;
  }
}

void dimmer_change(int32_t count)
{
    while (count)
    {
        if (count > 0)
        {
            dimmer_increment();
            count--;
        }
        if (count < 0)
        {
            dimmer_decrement();
            count++;
        }
    }
}
