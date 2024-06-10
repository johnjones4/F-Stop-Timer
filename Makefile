.PHONY: test

test:
	rm a.out || true
	g++ -I lib/timescale test/test_timescale.cpp lib/timescale/timescale.cpp
	./a.out
	rm a.out
