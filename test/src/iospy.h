#ifndef IOSPY_H
#define IOSPY_H

#include <stdio.h>
#include <stddef.h> // for size_t

FILE * iospy_get_fp_in(void);
FILE * iospy_get_fp_out(void);

void   iospy_hook_in(void);
void   iospy_unhook_in(void);

void   iospy_hook_out(void);
void   iospy_unhook_out(void);

void   iospy_hook(void);
void   iospy_unhook(void);

void   iospy_push_in(const void *, size_t, size_t);
void   iospy_push_in_str(const char *);
size_t iospy_pop_out(void *, size_t, size_t);
size_t iospy_pop_out_str(char *, size_t);

#endif