DEVICE = atxmega256d3
AVRDUDE_DEVICE =x256d3

PROG = -c avrispmkII -P usb

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(DEVICE) -Os
CC=avr-gcc
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -Wl,-relax

AVRDUDE=avrdude
TARGET=main
OBJECT_FILES=main.o ff.o sdmm.o
SRC_FILES=$(OBJECT_FILES:%.o=%.c)

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) $(PROG) -U flash:w:$(TARGET).hex
