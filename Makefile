LDLIBS = -lm

fstop:
	gcc -I./lib/fstop fstop.c lib/fstop/fstop.c $(LDLIBS)