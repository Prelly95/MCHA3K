F_CPU = 14745600

UNITY_ROOT = ../Unity

TARGET_BASE = lab7
TARGET_ELF = bin/$(TARGET_BASE).elf
TARGET_HEX = bin/$(TARGET_BASE).hex

COMMONDEFS = -DF_CPU=$(F_CPU)UL

CDEFS = $(COMMONDEFS)

CFLAGS  = -std=c99
CFLAGS += $(CDEFS)
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wpointer-to-int-cast
CFLAGS += -Wcomment
CFLAGS += -Wtype-limits

# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# PRINTF_LIB =
# PRINTF_LIB = $(PRINTF_LIB_MIN)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# SCANF_LIB =
# SCANF_LIB = $(SCANF_LIB_MIN)
SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# AVR Linker flags
LDFLAGS_AVR = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

SRC_COMMON = \
	src/circ_buffer.c \
	src/cmd_line_buffer.c \
	src/cmd_parser.c

SRC_AVR = \
	$(SRC_COMMON) \
	src/main.c \
	src/uart_isr.c \
	src/i2c.c \
	src/oled.c \
	src/uart.c \
	src/fonts.c \
	src/mpu6050.c

INC_COMMON = \
	-Isrc

INC_AVR = \
	$(INC_COMMON)

SYMBOLS =

DTREXE = ../../dtr.exe

all: clean default


default:
	avr-gcc -g -Os -mmcu=atmega32 $(CFLAGS) $(INC_AVR) $(SRC_AVR) -o $(TARGET_ELF) $(LDFLAGS_AVR)
	avr-size $(TARGET_ELF)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET_HEX)

clean_avr:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

clean: clean_avr

program: clean default
	- ./$(DTREXE) COM4 HIGH 10
	- ./$(DTREXE) COM4 LOW 10
	avrdude -p atmega32 -c avr109 -P COM4 -b 115200 -u -U flash:w:$(TARGET_HEX)
