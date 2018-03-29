#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/pgmspace.h>

#include "i2c.h"

#include "oled.h"
#include "fonts.h"

static uint8_t OLED_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];
OLED_t OLED;

uint8_t OLED_Init(void)
{
	{
		OLED_Command(0xAE); //display off
		OLED_Command(0x20); //Set Memory Addressing Mode
		OLED_Command(0x01); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		OLED_Command(0xB7); //Set Page Start Address for Page Addressing Mode,0-7
		OLED_Command(0xC8); //Set COM Output Scan Direction
		OLED_Command(0x00); //---set low column address
		OLED_Command(0x10); //---set high column address
		OLED_Command(0x40); //--set start line address
		OLED_Command(0x81); //--set contrast control register
		OLED_Command(0xFF);
		OLED_Command(0xA1); //--set segment re-map 0 to 127
		OLED_Command(0xA6); //--set normal display
		OLED_Command(0xA8); //--set multiplex ratio(1 to 64)
		OLED_Command(0x3F); //
		OLED_Command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		OLED_Command(0xD3); //-set display offset
		OLED_Command(0x00); //-not offset
		OLED_Command(0xD5); //--set display clock divide ratio/oscillator frequency
		OLED_Command(0xF0); //--set divide ratio
		OLED_Command(0xD9); //--set pre-charge period
		OLED_Command(0x22); //
		OLED_Command(0xDA); //--set com pins hardware configuration
		OLED_Command(0x12);
		OLED_Command(0xDB); //--set vcomh
		OLED_Command(0x20); //0x20,0.77xVcc
		OLED_Command(0x8D); //--set DC-DC enable
		OLED_Command(0x14); //
		OLED_Command(0xAF); //--turn on OLED panel

		OLED.Initialised = 1;
		OLED.CurrentX = 3;	//OLED x offset

		OLED_Fill(White);
		OLED_UpdateScreen();
		OLED_Fill(Black);
		OLED_UpdateScreen();
		return 0;
	}
}

uint8_t OLED_Command(uint8_t command)
{
	uint8_t control = 0x00;
	uint8_t data[2] = {control, command};

	if(I2C_Transmit(OLED_WRITE_ADDRESS, 2, data))
	{
		printf_P(PSTR("\nERROR 1\n\n"));
		return 1;
	}

	return 0;
}

uint8_t OLED_Data(uint8_t address, uint8_t length, uint8_t *data)
{
	if (I2C_Start(address))
	{
		printf_P(PSTR("\nERROR: Start Command Unsucessful\n\n"));
		return 1;
	}

	if (I2C_Write(0x40))//data in #D/C = 1
	{
		printf_P(PSTR("\nERROR: I2C Write Unsuccessful\n\n"));
		return 1;
	}

	for (uint8_t i = 0; i < length; i++)
	{
		if (I2C_Write(data[i]))
		{
			printf_P(PSTR("\nERROR: I2C Write Unsuccessful\n\n"));
			return 1;
		}
	}

	I2C_Stop();

	return 0;
}

void OLED_Fill(OLED_COLOUR colour)
{
	uint16_t i;
	for(i = 0; i < sizeof(OLED_Buffer); i++)
	{
		OLED_Buffer[i] = colour;
	}
}

uint8_t OLED_UpdateScreen(void)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		OLED_Command(0xB0 + i);
		OLED_Command(0x00);
		OLED_Command(0x10);
		if(OLED_Data(OLED_WRITE_ADDRESS, OLED_WIDTH, &OLED_Buffer[OLED_WIDTH*i]))
		{
			printf_P(PSTR("\nERROR: OLED Data not written succesfully\n\n"));
			return 1;
		}
	}

	return 0;
}

uint8_t OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOUR colour)
{
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
	{
		return 1;
	}

	if (colour == White)
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
	}
	else
	{
		OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
	}
	return 0;
}

uint8_t OLED_WriteChar(uint8_t ch, FontDef Font, OLED_COLOUR colour)
{
	uint16_t i, b, j;
	uint8_t charOffset;
	static uint8_t currentRow = 0;

	if(OLED.CurrentX > OLED_WIDTH)
	{
		currentRow++;
		OLED.CurrentX = 3;
		OLED.CurrentY = Font.FontHeight*currentRow;
	}

	if(OLED.CurrentY > OLED_HEIGHT)
	{
		OLED.CurrentX = 3;
		OLED.CurrentY = 0;
		currentRow = 0;
	}

	for (i = 0; i < Font.FontHeight; i++)
	{
		if((charOffset = Find_Char(ch)) == 200)
		{
			return 1;
		}

		b = Font.data[i + Font.FontWidth*charOffset];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x80)
			{
				OLED_DrawPixel((OLED.CurrentX + j), (OLED.CurrentY + i), (OLED_COLOUR) colour);
			}
			else
			{
				OLED_DrawPixel((OLED.CurrentX + j), (OLED.CurrentY + i), (OLED_COLOUR)!colour);
			}
		}
	}

	OLED.CurrentX += Font.FontWidth;

	return ch;
}

uint8_t OLED_WriteString(const char *str)
{
	uint8_t i = 0;

	while(str[i] != '\0')
	{
		OLED_WriteChar(str[i], Font_8x8, White);
		if(i > 254)
		{
			return 1;
		}
		i++;
	}
	return OLED_UpdateScreen();
}

uint8_t OLED_SetCursor(uint8_t x, uint8_t y)
{
	if(x > OLED_WIDTH || y > OLED_HEIGHT)
	{
		return 1;//cursor out of bounds
	}

	OLED.CurrentX = x+3;
	OLED.CurrentY = y;
	return 0;
}

uint8_t OLED_PrintNum(const char *frmt, float num, ...)
{
	char *buff[50];
	uint8_t i;
	float *start;

	// printf_P(PSTR("\ntest 1\n\n"));

	start = (&num)+1;

	for(i = 0; i < num; i++)
	{
		// printf_P(PSTR("\ntest 2\n\n"));
		printf("test 2\n");
		// sprintf(buff[i], frmt, (*(start+i)));
		OLED_WriteString(buff[i]);
	}
	return 0;
}

uint8_t Find_Char(uint8_t ch)
{
	if((ch > 45) && (ch < 58))
	{
		return ch - 45;
	}
	else if((ch > 64) && (ch < 91))
	{
		return (ch-52);
	}
	else if(ch == 32)
	{
		return 0;
	}
	else
	{
		return 200;
	}
}
