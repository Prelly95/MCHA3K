#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "oled.h"
#include "motor.h"
#include "controller.h"

void _cmd_clear		(int argc, const char *argv[]);
void OLED_Colour	(int argc, const char *argv[]);
void Set_Function	(int argc, const char *argv[]);
void ctrl			(int argc, const char *argv[]);
void theta			(int argc, const char *argv[]);
void dtheta			(int argc, const char *argv[]);
void vref			(int argc, const char *argv[]);
void Motor			(int argc, const char *argv[]);
void Reset			(int argc, const char *argv[]);
void test_cmd		(int argc, const char *argv[]);


float _theta = 0;
float _dtheta = 0;
float _vref = 0;

//
const command_s CommandList[] =
{
	{(uint8_t *)"clear",			(void*)&_cmd_clear},
	{(uint8_t *)"clc",				(void*)&_cmd_clear},
	{(uint8_t *)"OLED_Colour",		(void*)&OLED_Colour},
	{(uint8_t *)"Set",				(void*)&Set_Function},
	{(uint8_t *)"ctrl",				(void*)&ctrl},
	{(uint8_t *)"theta",			(void*)&theta},
	{(uint8_t *)"dtheta",			(void*)&dtheta},
	{(uint8_t *)"vref",				(void*)&vref},
	{(uint8_t *)"Motor",			(void*)&Motor},
	{(uint8_t *)"Reset",			(void*)&Reset},
	{(uint8_t *)"testcmd",			(void*)&test_cmd},
	{NULL, NULL}
};

void cmd_parse(int argc, const char *argv[])
{
	uint8_t j = 0;

	if (argv == NULL)
	{
		printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
		return;
	}
	else if (**argv == '\0') // Empty command string
    {
        return;
    }

	for(j=0;  CommandList[j].NameString != NULL; j++)
	{
		if(strcasecmp(argv[0], (const char *)CommandList[j].NameString) == 0)
		{
			(*CommandList[j].Function_p)(argc, argv);
			return;
		}
	}

	printf_P(PSTR("Unknown command: \"%s\"\n"), argv[0]);
	return;
}

void _cmd_clear(int argc, const char *argv[])
{
	(void)argc;
	(void)argv;
	printf_P(PSTR("\014"));
}

void OLED_Colour(int argc, const char *argv[])
{
	(void)argc;

	if(strcasecmp("white", argv[1]) == 0)
	{
		OLED_Fill(White);
	}
	else
	{
		OLED_Fill(Black);
	}

	OLED_UpdateScreen();
	OLED_SetCursor(0, 0);
}


void Set_Function(int argc, const char *argv[])
{
	if(argc != 3)
	{
		printf_P(PSTR("SetFunction expects 3 variables - Syntax: Set [Input name][Value]\n"));
	}
	if(strcmp(argv[1], "theta") == 0)
	{
		_theta = atof(argv[2]);
		// printf_P(PSTR("theta = %g\n"), _theta);
	}
	if(strcmp(argv[1], "dtheta") == 0)
	{
		_dtheta = atof(argv[2]);
		// printf_P(PSTR("dtheta = %g\n"), _dtheta);
	}
	if(strcmp(argv[1], "vref") == 0)
	{
		_vref = atof(argv[2]);
		// printf_P(PSTR("vref = %g\n"), _vref);
	}
}


void ctrl(int argc, const char *argv[])
{

	(void)argv;
	(void)argc;

	float u[3];
	
	u[0] = _theta;
	u[1] = _dtheta;
	u[2] = _vref;
			
	float *OutputValuePointer = ctrl_run(u);
	printf_P(PSTR("%g\n"), OutputValuePointer[0]);
}

void theta(int argc, const char *argv[])
{

	(void)argv;
	(void)argc;

	printf_P(PSTR("%g\n"), _theta);
}

void dtheta(int argc, const char *argv[])
{

	(void)argv;
	(void)argc;

	printf_P(PSTR("%g\n"), _dtheta);
}

void vref(int argc, const char *argv[])
{

	(void)argv;
	(void)argc;

	printf_P(PSTR("%g\n"), _vref);
}

void Motor(int argc, const char *argv[])
{
	if(strcasecmp(argv[1], "Mode") == 0 && argc == 3)
	{
		Motor_Step_Mode(atoi(argv[2]));
		// printf_P(PSTR("Mode is set to %i\n"), atoi(argv[2]));
	}
	else if(strcasecmp(argv[1], "Speed") == 0 && argc == 3)
	{
		Motor_Set_Vref(atof(argv[2]));
		// printf_P(PSTR("Speed is set to %f\n"), atof(argv[2]));
	}
	else if(strcasecmp(argv[1], "Yaw") == 0 && argc == 5)
	{
		Motor_Set_Yaw(atof(argv[2]), atof(argv[3]), atof(argv[4]));
		// printf_P(PSTR("left Yaw =  %f\tRight Yaw =  %f on the spot = %i\n"), atof(argv[2]), atof(argv[3]), atof(argv[4]));
	}
	else if(strcasecmp(argv[1], "direction") == 0 && argc == 6)
	{
		Motor_Set_Vref(atof(argv[2]));
		Motor_Set_Yaw(atof(argv[3]), atof(argv[4]), atof(argv[5]));
	}
	else
	{
		printf_P(PSTR("\"Motor\" expects 3 variables - Syntax: Motor [Input name][Value]\n"));
		return;
	}
		
}

void Reset(int argc, const char *argv[])
{
	(void)argv;
	(void)argc;

	WDTCR &= ~(1 << WDP2);
	WDTCR &= ~(1 << WDP1);
	WDTCR &= ~(1 << WDP0);
	WDTCR |= (1 << WDE);

	while(1);
}

void test_cmd(int argc, const char *argv[])
{
	(void)argv;
	(void)argc;

	printf_P(PSTR("\nThis\nis\na\ntest\n\n"));
}