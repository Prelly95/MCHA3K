#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"
#include "cmd_parser.h"

static void _cmd_help(void);
//
// const command_s CommandList[] =
// {
// 		{(int8_t *)"help", (void*)&_cmd_help, (int8_t *)"help [command]: Prints help information for a command"},
// // 		// {(int8_t *)"debug", (void*)&_cmd_help, (int8_t *)"debug <on|off>"},
// // 		// {(int8_t *)"clear", (void*)&_cmd_help, (int8_t *)"clears the terminal window"},
// // 		// {(int8_t *)"clc", (void*)&_cmd_help, (int8_t *)"clears the terminal window"},
// // 		{NULL, NULL, NULL}
// };

void cmd_parse(int argc, const char *argv[])
{
    if (argv == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
	else if (!strcmp_P(argv[0], PSTR("help")))
	{
		_cmd_help();
	}
    else
    {
        printf_P(PSTR("Unknown command: \"%s\"\n"), argv[0]);
    }
}

// void OLED_Num(uint8_t num)
// {
// 	OLED_PrintNum("%i", 1, num);
// }

void _cmd_help(void)
{
    printf_P(PSTR(
        "\n"
        "\n"
    ));

    printf_P(PSTR("\n"));

    // Describe argument syntax using POSIX.1-2008 convention
    // see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
    printf_P(PSTR(
        "Available commands:\n"
		"    clear or clc               Clears the current serial terminal (Handle Form Feed characters must be enabled)"
        "\n"
    ));
}
