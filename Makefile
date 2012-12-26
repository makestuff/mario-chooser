#
# Copyright (C) 2009 Chris McClelland
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

TARGET = firmware
MCU = at90usb162
F_CPU = 16000000

SRCS = $(shell ls *.c)
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
LIBS =
CC = avr-gcc
OBJDIR = .build
DEPDIR = .deps
CFLAGS = \
	-O3 -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -ffunction-sections \
	-fpack-struct -fshort-enums -finline-limit=20 -Wall -Wstrict-prototypes \
	-Wundef -std=gnu99 -Wall -pedantic
LDFLAGS = -mmcu=$(MCU) -Wl,--relax -Wl,--gc-sections -lm

all: $(TARGET).hex

$(TARGET).hex: $(OBJS)
	$(CC) $(LDFLAGS) -Wl,-Map=$(OBJDIR)/$(TARGET).map,--cref -o $(OBJDIR)/$(TARGET).elf $(OBJS) $(LIBS)
	avr-objcopy -O ihex -R .eeprom $(OBJDIR)/$(TARGET).elf $(TARGET).hex
	avr-size --mcu=$(MCU) --format=avr $(OBJDIR)/$(TARGET).elf

$(OBJDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) -MMD -MP -MF $(DEPDIR)/$(@F).d -Wa,-adhlns=$(OBJDIR)/$<.lst $< -o $@ -DF_CPU=$(F_CPU)

clean: FORCE
	rm -rf $(OBJDIR) $(TARGET).hex $(DEPDIR)

flash: $(TARGET).hex
	-dfu-programmer $(MCU) erase
	-dfu-programmer $(MCU) flash $<
	-dfu-programmer $(MCU) reset
	-dfu-programmer $(MCU) reset

-include $(shell mkdir -p $(OBJDIR) $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)

FORCE:
