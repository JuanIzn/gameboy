#include "include/emu.h"
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

    // TODO: cart_load(argv[1]);
    // TODO: cpu_init();
    // TODO: bus_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running) {
        if (ctx.paused) {
            continue;
        }

        // TODO: cpu_step();

        ctx.ticks++;
    }

    return 0;
}

int main(int argc, char **argv) {
    return emu_run(argc, argv);
}
