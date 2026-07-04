CC      = gcc
CFLAGS  = -Wall -Wextra -g
SRC     = $(wildcard src/*.c)
ROM     = roms/Pokemon Red.gb

gameboy: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gameboy

run: gameboy
	./gameboy "$(ROM)"

# Test has its own main, so it never links emu.c.
test_bus: src/bus.c src/cart.c tests/test_bus.c
	$(CC) $(CFLAGS) src/bus.c src/cart.c tests/test_bus.c -o test_bus

test: test_bus
	./test_bus "$(ROM)"

clean:
	rm -f gameboy test_bus

.PHONY: run test clean
