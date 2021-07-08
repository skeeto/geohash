CC      = cc
CFLAGS  = -Wall -Wextra -O3 -g
LDFLAGS =
LDLIBS  =

all: check benchmark

check: main
	@./main

test: check

main: tests/tests.c geohash.c geohash.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/tests.c geohash.c $(LDLIBS)

benchmark: benchmarks
	@./benchmarks

benchmarks: tests/benchmark.c geohash.c geohash.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/benchmark.c geohash.c $(LDLIBS)

clean:
	rm -f main benchmarks
