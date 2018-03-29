#ifndef FONTS
#define FONTS

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint8_t *data; /*!< Pointer to data font data array */
} FontDef;

extern FontDef Font_8x8;

#endif
