#ifndef CMD_PARSER_H
#define CMD_PARSER_H

void cmd_parse(const char *);

typedef struct
{
	int8_t *NameString;
	int8_t (*Function_p)(uint8_t ArgNum, uint8_t *ArgStrings[]);
	int8_t *HelpString;
}cmd_struct;

#endif
