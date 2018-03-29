#ifndef STDIO_REDIRECT_H
#define STDIO_REDIRECT_H

#include <stdio.h>

FILE * fstdin(void);
void push_stdin(void);
void pop_stdin(void);

FILE * fstdout(void);
void push_stdout(void);
void pop_stdout(void);

void push_stdio(void);
void pop_stdio(void);

#endif