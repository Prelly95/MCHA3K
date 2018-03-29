#include <stddef.h>
#include "controller.h"

static float _ctrl_state[CTRL_N_STATE];
static float _ctrl_output[CTRL_N_OUTPUT];

static const float _ctrl_A[CTRL_N_STATE][CTRL_N_STATE] =
// #include "A.def"
{
    {0.77548},
};

static const float _ctrl_B[CTRL_N_STATE][CTRL_N_INPUT] =
// #include "B.def"
{
    {0.022717, -0.022717, -0.56511},
};

static const float _ctrl_C[CTRL_N_OUTPUT][CTRL_N_STATE] =
// #include "C.def"
{
    {37.19}
};

static const float _ctrl_D[CTRL_N_OUTPUT][CTRL_N_INPUT] =
// #include "D.def"
{
    {-4.7841, 4.7841, 119.01}
};

void ctrl_init(void)
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = 0.0f;
}

void ctrl_set_state(const float state[CTRL_N_STATE])
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = state[i];
}

float * ctrl_get_state(void)
{
    return _ctrl_state;
}

float * ctrl_run(const float input[CTRL_N_INPUT])
{
    // y = C*x + D*u
    for (size_t i = 0; i < CTRL_N_OUTPUT; ++i)
    {
        _ctrl_output[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            _ctrl_output[i] += _ctrl_C[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            _ctrl_output[i] += _ctrl_D[i][j]*input[j];
    }

    // x_next = A*x + B*u
    float state[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
    {
        state[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            state[i] += _ctrl_A[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            state[i] += _ctrl_B[i][j]*input[j];
    }

    // x = x_next
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = state[i];

    return _ctrl_output;
}
