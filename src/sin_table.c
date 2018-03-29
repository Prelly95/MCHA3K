#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "task.h"
#include "sin_table.h"

static uint32_t _n_samples;
static float _time, _amplitude, _frequency;

void sin_table_cmd(int argc, const char *argv[])
{
    if (argc != 4)
    {
        printf_P(PSTR("%s: expecting 3 arguments, syntax is: %s <samples> <amplitude> <frequency>\n"), argv[0], argv[0]);
        return;
    }

    _n_samples = atoi(argv[1]);
    _amplitude = atof(argv[2]);
    _frequency = atof(argv[3]);
    _time      = 0.0f;

    printf_P(PSTR("Time [sec],Value\n"));

    task_set(sin_table_task);
    task_enable();
}

void sin_table_task(void)
{
    float y = _amplitude*sin(2*M_PI*_frequency*_time);
    printf_P(PSTR("%.2f,%g\n"), _time, y);
    _time += 0.01f;
    --_n_samples;

    if (_n_samples == 0) task_disable();
}
