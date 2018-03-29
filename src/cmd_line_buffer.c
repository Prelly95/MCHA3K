#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stddef.h>
#include "cmd_line_buffer.h"

void clb_init(CLB_T *clb)
{
    clb->count = 0;
    clb->buffer[clb->count] = '\0';
}

bool clb_is_empty(const CLB_T *clb)
{
    return clb->count == 0;
}

bool clb_is_full(const CLB_T *clb)
{
    return clb->count >= clb->size;
}

// Add character to line buffer and report status
CLB_STATUS_T clb_putc(CLB_T *clb, char c)
{
    if (!clb_is_full(clb))
    {
        clb->buffer[clb->count++] = c;
        if (clb_is_cmd_ready(clb))
            return CLB_CMD_READY;
        else
            return CLB_SUCCESS;
    }
    else
    {
        return CLB_BUFFER_FULL;
    }
}

// Remove last character from line buffer
void clb_delc(CLB_T *clb)
{
    if (!clb_is_empty(clb))
    {
        clb->count--;
    }
}

// Append character to line buffer and handle special characters
CLB_STATUS_T clb_consume_char(CLB_T *clb, char c)
{
    if (c == '\b')                      // Backspace character
    {
        clb_delc(clb);
    }
    else if (c != '\r')                 // Ignore carriage return
    {
        if (c != '\n')
        {
            return clb_putc(clb, c);
        }
        else if (!clb_is_empty(clb))    // Ignore empty line
        {
            return clb_putc(clb, '\0'); // Null-terminate string
        }
    }
    return CLB_SUCCESS;
}

CLB_STATUS_T clb_consume_str(CLB_T *clb, const char *s)
{
    CLB_STATUS_T ret = CLB_SUCCESS;
    while(*s && (ret = clb_consume_char(clb, *s++)) >= 0);
    return ret;
}

// Check if line buffer contains a non-trivial null-terminated string
bool clb_is_cmd_ready(const CLB_T *clb)
{
    return !clb_is_empty(clb) && clb->buffer[clb->count-1] == '\0';
}

// Get string starting at specified location
char * clb_gets_at(CLB_T *clb, CLB_INDEX_T i)
{
    if (clb_is_cmd_ready(clb) && i < clb->count)
    {
        return &clb->buffer[i];
    }
    else
    {
        return NULL;
    }
}

char * clb_gets(CLB_T *clb)
{
    return clb_gets_at(clb, 0);
}

#include "cmd_parser.h"

void clb_process(CLB_T *clb)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        switch (clb_consume_char(clb, c))
        {
            case CLB_BUFFER_FULL:
                printf_P(PSTR("*** Max command length exceeded ***\n"));
                clb_init(clb);
                break;
            case CLB_CMD_READY:
                cmd_parse(clb_gets(clb));
                clb_init(clb);
            case CLB_SUCCESS:
            default:
                break;
        }
    }
}