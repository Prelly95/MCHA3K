#ifndef OLED_H
#define OLED_H

// #include "fonts.h"

#define OLED_READ_ADDRESS 0x79
#define OLED_WRITE_ADDRESS 0x78

#define OLED_WIDTH 130
#define OLED_HEIGHT 64

typedef enum {
	Black = 0x00, //pixel is turned off
	White = 0xFF //Pixel is turned on
} OLED_COLOUR;

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialised;
} OLED_t;

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
} FontDef_t;

uint8_t OLED_Init(void);
uint8_t OLED_Command(uint8_t command);
uint8_t OLED_UpdateScreen(void);
uint8_t OLED_Data(uint8_t address, uint8_t length, uint8_t *data);
uint8_t OLED_DrawPixel(uint8_t x, uint8_t y, OLED_COLOUR color);
uint8_t OLED_WriteChar(uint8_t ch, FontDef_t Font, OLED_COLOUR colour);
uint8_t Find_Char(uint8_t ch);
uint8_t OLED_WriteString(const char *string);
uint8_t OLED_SetCursor(uint8_t x, uint8_t y);
uint8_t OLED_PrintNum(const char *frmt, float num, ...);

void OLED_Fill(OLED_COLOUR colour);

#endif
