#include "circ_buffer.h"
#include <stdint.h>

void cb_init(CB_T *cb)
{
	cb->head = 0;
	cb->tail = 0;
}

int cb_is_empty(const CB_T *cb)
{
	return (cb->head == cb->tail);
}

int cb_is_full(const CB_T *cb)
{
	return ((cb->head + 1) % cb->size) == cb->tail;
}

void cb_queue(CB_T *cb, char c)
{
	if (!cb_is_full(cb))
	{
		cb->buffer[cb->head] = c;
		cb->head = ((cb->head + 1) % cb->size);
	}
}

char cb_dequeue(CB_T *cb)
{
	if (cb_is_empty(cb))
	{
		return '\0';	// Return null character since buffer is empty
	}
	else
	{
		char c = cb->buffer[cb->tail];
		cb->tail = ((cb->tail + 1) % cb->size);
		return c;
	}
}

CB_INDEX_T cb_capacity(const CB_T *cb)
{
	return (cb->size - 1);
}
