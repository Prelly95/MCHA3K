#ifndef CMD_LINE_BUFFER_H
#define CMD_LINE_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

#define CLB_CREATE(name, n) \
    char name ## _buffer[n]; \
    CLB_T name = {.size = sizeof(name ## _buffer), .buffer = name ## _buffer}

#define CLB_CREATE_STATIC(name, n) \
    static char name ## _buffer[n]; \
    static CLB_T name = {.size = sizeof(name ## _buffer), .buffer = name ## _buffer}


typedef enum {
    CLB_SUCCESS = 0, CLB_CMD_READY = 1, // Success status codes >= 0
    CLB_BUFFER_FULL = -1                // Failure status codes < 0
} CLB_STATUS_T;

typedef uint16_t CLB_INDEX_T;

typedef struct
{
    CLB_INDEX_T count;              // number of elements stored
    const CLB_INDEX_T size;         // buffer size
    char * const buffer;            // const pointer to non-const char buffer
} CLB_T;

void         clb_init(CLB_T *);
bool         clb_is_empty(const CLB_T *);
bool         clb_is_full(const CLB_T *);
CLB_STATUS_T clb_consume_char(CLB_T *, char);
CLB_STATUS_T clb_consume_str(CLB_T *, const char *);
bool         clb_is_cmd_ready(const CLB_T *);
char *       clb_gets_at(CLB_T *, CLB_INDEX_T);
char *       clb_gets(CLB_T *);
void         clb_process(CLB_T *);

#endif
