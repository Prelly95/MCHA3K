#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "cmd_line_buffer.h"

void cmd_parse(int argc, const char *argv[]);

typedef struct{
	int8_t *NameString;
	int8_t (*Function_p)(uint8_t ArgNum, uint8_t *ArgStrings[]);
	int8_t *HelpString;
} command_s;

#endif
