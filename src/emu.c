#include "include/emu.h"
#include "include/cart.h"
#include "include/cpu.h"
#include "include/bus.h"
#include <stdio.h>

static emu_context ctx;

emu_context *emu_get_context() {
    return &ctx;
}

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        fprintf(stderr, "Failed to load cartridge: %s\n", argv[1]);
        return -1;
    }

    cpu_init();
    bus_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running) {
        if (ctx.paused) {
            continue;
        }
        cpu_step();
        ctx.ticks++;
    }

    cart_unload();
    return 0;
}

int main(int argc, char **argv) {
    return emu_run(argc, argv);
}
