# We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
	TARGET_TEST_EXTENSION=.exe
else
	TARGET_TEST_EXTENSION=.out
endif

F_CPU = 14745600

UNITY_ROOT = ../Unity

TARGET_BASE = lab2
TARGET_ELF = bin/$(TARGET_BASE).elf
TARGET_HEX = bin/$(TARGET_BASE).hex

TARGET_TEST_BASE = test_$(TARGET_BASE)
TARGET_TEST = test/bin/$(TARGET_TEST_BASE)$(TARGET_TEST_EXTENSION)

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

SRC_COMMON = \
	src/dimmer.c \
	src/led.c \
	src/encoder.c \
	src/potentiometer.c

SRC_TEST = \
	$(UNITY_ROOT)/src/unity.c \
	$(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
	$(SRC_COMMON) \
	mock/avr/mock_sfr.c \
	test/src/all_tests.c \
	test/src/test_dimmer.c \
	test/src/test_led.c \
	test/src/test_encoder.c \
	test/src/test_potentiometer.c

SRC_AVR = \
	$(SRC_COMMON) \
	src/main.c \
	src/encoder_isr.c

INC_COMMON = \
	-Isrc

INC_TEST = \
	-I$(UNITY_ROOT)/src \
	-I$(UNITY_ROOT)/extras/fixture/src \
	$(INC_COMMON) \
	-Imock

INC_AVR = \
	$(INC_COMMON)

SYMBOLS =

DTREXE = ../../dtr.exe

all: clean default

test: clean_test test_only

test_only:
	gcc $(CFLAGS) $(INC_TEST) $(SYMBOLS) $(SRC_TEST) -o $(TARGET_TEST)
	- ./$(TARGET_TEST) -v

clean_test:
	rm -f $(TARGET_TEST)

default:
	gcc $(CFLAGS) $(INC_TEST) $(SYMBOLS) $(SRC_TEST) -o $(TARGET_TEST)
	avr-gcc -g -Os -mmcu=atmega32 $(CFLAGS) $(INC_AVR) $(SRC_AVR) -o $(TARGET_ELF)
	avr-size $(TARGET_ELF)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET_HEX)
	- ./$(TARGET_TEST)

clean_avr:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

clean: clean_test clean_avr

program: default
	- ./$(DTREXE) COM4 HIGH 10
	- ./$(DTREXE) COM4 LOW 10
	avrdude -p atmega32 -c avr109 -P COM4 -b 115200 -u -U flash:w:$(TARGET_HEX)
