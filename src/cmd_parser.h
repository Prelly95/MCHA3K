#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "cmd_line_buffer.h"

void cmd_parse(int argc, const char *argv[]);

typedef struct
{
	uint8_t *NameString;
	void(*Function_p)(char argc, const char *argv[]);
} command_s;

#endif
