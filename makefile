# Directories
TOOLS_DIR = C:\Users\Rafael\AppData\Local\Arduino15\packages\arduino\tools
AVRGCC_ROOT_DIR = $(TOOLS_DIR)\avr-gcc\7.3.0-atmel3.6.1-arduino7
AVRGCC_BIN_DIR = $(AVRGCC_ROOT_DIR)\bin
AVRGCC_INCLUDE_DIR = $(AVRGCC_ROOT_DIR)\avr\include\avr
INCLUDE_DIRS = $(AVRGCC_INCLUDE_DIR)
LIB_DIRS = $(AVRGCC_INCLUDE_DIR)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)\obj
BIN_DIR = $(BUILD_DIR)\bin
AVRDUDE_ROOT_DIR = $(TOOLS_DIR)\avrdude\6.3.0-arduino17

#Toolchain
CC = $(AVRGCC_BIN_DIR)\avr-gcc
OBJ_CPY = $(AVRGCC_BIN_DIR)\avr-objcopy
AVRDUDE = $(AVRDUDE_ROOT_DIR)\bin\avrdude
RM = rmdir

# Files
TARGET = $(BIN_DIR)\blink
HEX_TARGET = $(TARGET).hex
ASM_TARGET = $(TARGET).s

SOURCES = main.c usart.c
CONFIG = $(AVRDUDE_ROOT_DIR)\etc\avrdude.conf

OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)\\%,$(OBJECT_NAMES))

# Flags
MCU = atmega328p
WFLAGS = -Wall # -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(OPT) $(addprefix -I ,$(INCLUDE_DIRS))
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(LIB_DIRS)) 

# Build
## Linking
$(TARGET): $(OBJECTS)
	@echo "Linking $^"
	@if not exist "$(dir $@)" mkdir $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@  

## Compiling
$(OBJ_DIR)\\%.o: %.c
	@echo "Compiling $^"
	@if not exist "$(dir $@)" mkdir $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(HEX_TARGET): $(TARGET)	
	@echo "Copy hex $^"
	@if not exist "$(dir $@)" mkdir $(dir $@)
	$(OBJ_CPY) -O ihex -j.text -j.data $^ $@
	
## Assambling
$(ASM_TARGET): main.c	
	@echo "Assambling $^"
	@if not exist "$(dir $@)" mkdir $(dir $@)
	$(CC) $(CFLAGS) -S -o $@ $^

# Phonies
.PHONY: all clean flash lfuse help asm hfuse

all: $(HEX_TARGET)

asm: $(ASM_TARGET)

clean: 
	$(RM) /s /q $(BUILD_DIR)

flash:
	$(AVRDUDE) -C $(CONFIG) -v -p $(MCU) -c stk500v1 -P $(COM) -b 19200 -U flash:w:$(HEX_TARGET):i

help:
	@echo "options available: all, clean, flash"

lfuse:
	$(AVRDUDE) -C $(CONFIG) -v -p $(MCU) -c stk500v1 -P $(COM) -b 19200 -U lfuse:w:$(VAL):m
	
hfuse:
	$(AVRDUDE) -C $(CONFIG) -v -p $(MCU) -c stk500v1 -P $(COM) -b 19200 -U hfuse:w:$(VAL):m
