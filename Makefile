LDLIBS = -lm

USB_DEVICE="/dev/ttyUSB0"

load: bootloader program

fstop:
	gcc -I./lib/fstop fstop.c lib/fstop/fstop.c $(LDLIBS)

program:
	avrdude -c stk500v1 -p atmega328p -P "${USB_DEVICE}" -b 19200 -U flash:w:.pio/build/uno/firmware.hex:i

optiboot_atmega328.hex:
	wget https://raw.githubusercontent.com/arduino/ArduinoCore-avr/master/bootloaders/optiboot/optiboot_atmega328.hex

bootloader: optiboot_atmega328.hex
	avrdude -c stk500v1 -p atmega328p -P "${USB_DEVICE}" -b 19200 \
		-e -u -U lock:w:0x3F:m \
		-U efuse:w:0xFD:m -U hfuse:w:0xDE:m -U lfuse:w:0xFF:m \
		-U flash:w:./optiboot_atmega328.hex:i \
		-U lock:w:0x0F:m
