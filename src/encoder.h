#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void    encoder_init(void);
void    encoder_edge_A_isr(void);
void    encoder_edge_B_isr(void);

void    encoder_set_count(int32_t);
int32_t encoder_get_count(void);
int32_t encoder_pop_count(void);

#endif