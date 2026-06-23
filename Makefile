# Compiler and flags. -Wall -Wextra turn on warnings; -g adds debug info.
CC      = gcc
CFLAGS  = -Wall -Wextra -g

# All emulator sources (every .c under src/). The main binary links them.
SRC     = $(wildcard src/*.c)
ROM     = roms/Pokemon Red.gb

# Default target: build the emulator binary.
gameboy: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gameboy

# Build (if needed) and run with the test ROM.
run: gameboy
	./gameboy "$(ROM)"

# The bus test has its own main, so it links ONLY the files it needs
# (bus + cart), never emu.c.
test_bus: src/bus.c src/cart.c tests/test_bus.c
	$(CC) $(CFLAGS) src/bus.c src/cart.c tests/test_bus.c -o test_bus

# Build (if needed) and run the bus tests.
test: test_bus
	./test_bus "$(ROM)"

# Remove built binaries.
clean:
	rm -f gameboy test_bus

# These targets are commands, not files, so Make always runs them.
.PHONY: run test clean
