// Quick verification of the memory bus routing.
// Compile WITHOUT emu.c (it has its own main):
//   gcc src/bus.c src/cart.c tests/test_bus.c -o test_bus
//   ./test_bus "roms/Pokemon Red.gb"

#include "../src/include/bus.h"
#include "../src/include/cart.h"
#include <stdio.h>

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition) {
    if (condition) {
        printf("  PASS  %s\n", name);
        passed++;
    } else {
        printf("  FAIL  %s\n", name);
        failed++;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: test_bus <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        fprintf(stderr, "Could not load cartridge\n");
        return -1;
    }

    printf("\nRunning bus tests:\n");

    // Cartridge routing: a ROM address must match cart_read directly.
    check("bus_read(0x0100) routes to cartridge",
          bus_read(0x0100) == cart_read(0x0100));
    check("bus_read(0x0134) routes to cartridge",
          bus_read(0x0134) == cart_read(0x0134));

    // WRAM round-trip: write then read back the same value.
    bus_write(0xC000, 0x42);
    check("WRAM round-trip at 0xC000", bus_read(0xC000) == 0x42);
    bus_write(0xDFFF, 0x99);
    check("WRAM round-trip at 0xDFFF (last byte)", bus_read(0xDFFF) == 0x99);

    // Echo RAM mirrors WRAM: 0xE000 should reflect 0xC000.
    bus_write(0xC000, 0x7A);
    check("Echo RAM 0xE000 mirrors WRAM 0xC000", bus_read(0xE000) == 0x7A);

    // HRAM round-trip.
    bus_write(0xFF80, 0x55);
    check("HRAM round-trip at 0xFF80", bus_read(0xFF80) == 0x55);
    bus_write(0xFFFE, 0x11);
    check("HRAM round-trip at 0xFFFE (last byte)", bus_read(0xFFFE) == 0x11);

    // IE register (0xFFFF) is separate from HRAM.
    bus_write(0xFFFF, 0x1F);
    check("IE register round-trip at 0xFFFF", bus_read(0xFFFF) == 0x1F);

    printf("\n%d passed, %d failed\n\n", passed, failed);
    cart_unload();
    return failed == 0 ? 0 : 1;
}
