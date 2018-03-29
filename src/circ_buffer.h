#ifndef CIRC_BUFFER_H
#define CIRC_BUFFER_H

#include <stdint.h>

#define CB_CREATE(name, n) \
    char name ## _buffer[n]; \
    CB_T name = {.size = sizeof(name ## _buffer), .buffer = name ## _buffer}

#define CB_CREATE_STATIC(name, n) \
    static char name ## _buffer[n]; \
    static CB_T name = {.size = sizeof(name ## _buffer), .buffer = name ## _buffer}

typedef uint16_t CB_INDEX_T;

typedef struct
{
    volatile CB_INDEX_T head, tail;     // head and tail indices
    const CB_INDEX_T size;              // buffer size (useful capacity is size - 1)
    volatile char * const buffer;       // const pointer to non-const char buffer
} CB_T;

void       cb_init(CB_T *);
int        cb_is_empty(const CB_T *);
int        cb_is_full(const CB_T *);
void       cb_queue(CB_T *, char);
char       cb_dequeue(CB_T *);
CB_INDEX_T cb_capacity(const CB_T *);

#endif
