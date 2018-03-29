#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"
#include "cmd_parser.h"

static void _cmd_help(void);
static void _print_chip_pinout(void);

void cmd_parse(const char * cmd)
{
    if (cmd == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
	else if (!strncmp_P(cmd, PSTR("clear"), 5) || !strncmp_P(cmd, PSTR("clc"), 3))
	{
		printf_P(PSTR("\014"));
	}
    else if (*cmd == '\0') // Empty command string
    {
        return;
    }
    else if (!strncmp_P(cmd, PSTR("help"), 4))
    {
        _cmd_help();
    }
    else
    {
        printf_P(PSTR("Unknown command: \"%s\"\n"), cmd);
    }
}

void _cmd_help(void)
{
    printf_P(PSTR(
        "\n"
        "\n"
    ));

    _print_chip_pinout();

    printf_P(PSTR("\n"));

    // Describe argument syntax using POSIX.1-2008 convention
    // see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
    printf_P(PSTR(
        "Available commands:\n"
        "    help                       Print this help message\n"
		"    clear or clc               Clears the current serial terminal (Handle Form Feed characters must be enabled)"
        "\n"
    ));
}

void _print_chip_pinout(void)
{
    printf_P(PSTR(
        "Pin configuration:\n"
        "\n"
        "                               .----. ,----.\n"
        "           :    (XCK/T0)PB0 <->|1    \"   40|<-- PA0(ADC0)  : Potentiometer\n"
        "           :        (T1)PB1 <->|2        39|<-> PA1(ADC1)  :\n"
        "           : (INT2/AIN0)PB2 <->|3        38|<-> PA2(ADC2)  :\n"
        "      !LED :  (OC0/AIN1)PB3 <--|4    A   37|<-> PA3(ADC3)  :\n"
        "           :       (!SS)PB4 <->|5    T   36|<-> PA4(ADC4)  :\n"
        "           :      (MOSI)PB5 <->|6    M   35|<-> PA5(ADC5)  :\n"
        "           :      (MISO)PB6 <->|7    E   34|<-> PA6(ADC6)  :\n"
        "           :       (SCK)PB7 <->|8    L   33|<-> PA7(ADC7)  :\n"
        "                     !RESET -->|9        32|<-- AREF\n"
        "                        VCC ---|10   A   31|--- GND\n"
        "                        GND ---|11   T   30|--- AVCC\n"
        "                      XTAL2 <--|12   m   29|<-> PC7(TOSC2) :\n"
        "                      XTAL1 -->|13   e   28|<-> PC6(TOSC1) :\n"
        "  RS232 RX :       (RXD)PD0 -->|14   g   27|<-> PC5(TDI)   :\n"
        "  RS232 TX :       (TXD)PD1 <--|15   a   26|<-> PC4(TDO)   :\n"
        " Encoder A :      (INT0)PD2 -->|16   3   25|<-> PC3(TMS)   :\n"
        " Encoder B :      (INT1)PD3 -->|17   2   24|<-- PC2(TCK)   : BTLDR button\n"
        "           :      (OC1B)PD4 <->|18       23|<-> PC1(SDA)   :\n"
        "           :      (OC1A)PD5 <->|19       22|<-> PC0(SCL)   :\n"
        "           :      (ICP1)PD6 <->|20       21|<-> PD7(OC2)   :\n"
        "                               `-----------'\n"
    ));
}
