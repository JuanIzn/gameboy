#include "include/cart.h"
#include <stdio.h>
#include <string.h>

static cart_context ctx;

static bool validate_checksum() {
    u8 x = 0;
    // checksum covers bytes 0x0134 to 0x014C
    for (u16 i = 0x0134; i <= 0x014C; i++) {
        x = x - ctx.rom_data[i] - 1;
    }
    return (x & 0xFF) == ctx.header->checksum;
}

bool cart_load(char *path) {
    ctx.filename = path;

    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("ERROR: could not open file: %s\n", path);
        return false;
    }

    // get file size
    fseek(f, 0, SEEK_END);
    ctx.rom_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // load entire ROM into memory
    ctx.rom_data = malloc(ctx.rom_size);
    fread(ctx.rom_data, 1, ctx.rom_size, f);
    fclose(f);

    // point header into the buffer at offset 0x0100
    ctx.header = (rom_header *)(ctx.rom_data + 0x0100);

    // validate header checksum
    if (!validate_checksum()) {
        free(ctx.rom_data);
        ctx.rom_data = NULL;
        printf("ERROR: invalid header checksum\n");
        return false;
    }

    printf("Cartridge loaded: %.16s\n", ctx.header->title);
    printf("Type: 0x%02X  ROM size: 0x%02X  RAM size: 0x%02X\n",
           ctx.header->type, ctx.header->rom_size, ctx.header->ram_size);

    return true;
}

u8 cart_read(u16 address) {
    return ctx.rom_data[address];
}

void cart_unload() {
    if (ctx.rom_data) {
        free(ctx.rom_data);
        ctx.rom_data = NULL;
        ctx.header = NULL;
    }
}
