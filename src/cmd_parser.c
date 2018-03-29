#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"
#include "potentiometer.h"
#include "encoder.h"
#include "led.h"
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
	else if (!strncmp_P(cmd, PSTR("pot "), 4))
	{
		printf_P(PSTR("pot: invalid argument \"%s\", syntax is: pot\n"), cmd + 4);
	}
    else if (!strncmp_P(cmd, PSTR("pot"), 3))
    {
        printf_P(PSTR("Potentiometer ADC value is %" PRIu16 "\n"), pot_get_value());
    }
    else if (!strcmp_P(cmd, PSTR("enc")))
    {
        printf_P(PSTR("Encoder count is %" PRId32 "\n"), encoder_get_count());
    }
    else if (!strncmp_P(cmd, PSTR("enc "), 4))
    {
		if(!strncmp_P(cmd, PSTR("enc reset"), 9))
		{
			encoder_set_count(0);
			printf_P(PSTR("Encoder count reset to 0\n"));
		}
		else
		{
			printf_P(PSTR("enc: invalid argument \"%s\", syntax is: enc [reset]\n"), cmd + 4);
		}

    }
    else if (!strncmp_P(cmd, PSTR("led on"), 6))
    {
        led_on();
        printf_P(PSTR("LED is on\n"));
    }
    else if (!strncmp_P(cmd, PSTR("led off"), 7))
    {
        led_off();
        printf_P(PSTR("LED is off\n"));
    }
    else if (!strncmp_P(cmd, PSTR("led "), 4))
    {
		uint8_t led;
		int16_t led_temp;
		char* cmd_end;

		led_temp = strtol((cmd + 4), &cmd_end, 10);

		//fix invalid argument input
		if(*(cmd+4) == '\0')
		{
			printf_P(PSTR("LED brightness is %" PRIu8 "\n"), led_get_brightness());
		}
		else if(*cmd_end == '\0')
		{
			//Prevents led brightness value overflow and underflow
			if(led_temp > 255)
			{
				led = 255;
			}
			else if(led_temp < 0)
			{
				led = 0;
			}
			else
			{
	    		led = led_temp;
			}

			led_set_brightness(led);
			printf_P(PSTR("LED brightness set to %" PRIu8 "\n"), led);
		}
		else
		{
			printf_P(PSTR("led: invalid argument \"%s\", syntax is: led [on|off|<value>]\n"), cmd + 4);
		}
    }
    else if (!strncmp_P(cmd, PSTR("led"), 3))
    {
        printf_P(PSTR("LED brightness is %" PRIu8 "\n"), led_get_brightness());
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
        "    pot                        Get potentiometer ADC value (0-1023)\n"
        "    enc [reset]                Get or reset encoder count\n"
        "    led [on|off|<value>]       Get or set LED brightness (0-255)\n"
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
