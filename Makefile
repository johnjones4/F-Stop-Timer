LDLIBS = -lm

fstop:
	gcc -I./lib/fstop fstop.c lib/fstop/fstop.c $(LDLIBS)

load:
	avrdude -c stk500v1 -p atmega328p -P /dev/ttyUSB0 -b 19200 -U flash:w:.pio/build/uno/firmware.hex:i
