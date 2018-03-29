#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "task.h"
#include "potentiometer.h"
#include "encoder.h"
#include "log_data.h"

typedef enum {
    LOG_POT,
    LOG_ENC
} LOG_DEVICE_T;

static uint32_t _n_samples;
static LOG_DEVICE_T _device;
static float _time;

void log_cmd(int argc, const char *argv[])
{
    if (argc != 3)
    {
        printf_P(PSTR("%s: expecting 2 arguments, syntax is: %s [pot|enc] <samples>\n"), argv[0], argv[0]);
        return;
    }

    int n = atoi(argv[2]);
    if (n < 0)
    {
        printf_P(PSTR("%s: expecting positive number of samples\n"), argv[0]);
        return;
    }
    _n_samples = n;
    _time = 0.0f;

    if (!strcmp_P(argv[1], PSTR("pot")))
    {
        _device = LOG_POT;
        pot_init();
        printf_P(PSTR("Time [sec],Potentiometer [V]\n"));

    }
    else if (!strcmp_P(argv[1], PSTR("enc")))
    {
        _device = LOG_ENC;
        encoder_init();
        printf_P(PSTR("Time [sec],Encoder [-]\n"));
    }
    else
    {
        printf_P(PSTR("%s: unknown device \"%s\", syntax is: %s [pot|enc] <samples>\n"), argv[0], argv[1], argv[0]);
        return;
    }

    task_set(log_task);
    task_enable();
}

void log_task(void)
{
    switch (_device)
    {
    case LOG_POT:
        printf_P(PSTR("%.2f,%g\n"), _time, (float)pot_get_value()*5.0f/POT_FULL_SCALE);
        break;
    case LOG_ENC:
        printf_P(PSTR("%.2f,%" PRId32 "\n"), _time, encoder_get_count());
        break;
	default:
		break;
    }

    _time += 0.01f;
    --_n_samples;

    if (_n_samples == 0) task_disable();
}
