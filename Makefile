CC      = cc
CFLAGS  = -Wall -Wextra -O3 -g
LDFLAGS =
LDLIBS  = -lm

all: check benchmark example$(EXE)

check: main$(EXE)
	@./main$(EXE)

test: check

main$(EXE): tests/tests.c geohash.c geohash.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/tests.c geohash.c $(LDLIBS)

benchmark: benchmarks$(EXE)
	@./benchmarks$(EXE)

benchmarks$(EXE): tests/benchmark.c geohash.c geohash.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/benchmark.c geohash.c $(LDLIBS)

example$(EXE): tests/example.c geohash.c geohash.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ tests/example.c geohash.c $(LDLIBS)

clean:
	rm -f main$(EXE) benchmarks$(EXE) example$(EXE)
