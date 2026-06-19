#include "include/bus.h"
#include "include/cart.h"

static bus_context ctx;

// The checks cascade: each range only needs its UPPER bound, because every
// earlier range has already returned. So "<= 0x9FFF" really means 0x8000-0x9FFF.
//Based on: https://gbdev.io/pandocs/Memory_Map.html
u8 bus_read(u16 address) {
    if (address <= 0x7FFF) {
        return cart_read(address);          // ROM bank 0 + switchable bank
    }
    if (address <= 0x9FFF) {
        return 0;                           // VRAM - TODO: ppu_vram_read
    }
    if (address <= 0xBFFF) {
        return cart_read(address);          // external cartridge RAM
    }
    if (address <= 0xDFFF) {
        return ctx.wram[address - 0xC000];  // work RAM
    }
    if (address <= 0xFDFF) {
        return ctx.wram[address - 0xE000];  // echo RAM, mirrors WRAM
    }
    if (address <= 0xFE9F) {
        return 0;                           // OAM - TODO: ppu_oam_read
    }
    if (address <= 0xFEFF) {
        return 0;                           // not usable
    }
    if (address <= 0xFF7F) {
        return 0;                           // I/O registers - TODO: io_read
    }
    if (address <= 0xFFFE) {
        return ctx.hram[address - 0xFF80];  // high RAM
    }
    return ctx.ie_register;                 // 0xFFFF, interrupt enable
}

void bus_write(u16 address, u8 value) {
    if (address <= 0x7FFF) {
        return;    // ROM is read-only, writes talk to MBC - TODO
        /*
        escribir en 0x0000-0x1FFF  →  activa/desactiva la RAM externa
        escribir en 0x2000-0x3FFF  →  selecciona el banco de ROM   ← aquí va el nº de banco
        escribir en 0x4000-0x5FFF  →  selecciona banco de RAM / bits altos
        escribir en 0x6000-0x7FFF  →  modo de banking
        */
    }
    if (address <= 0x9FFF) {
        return;                             // VRAM - TODO: ppu_vram_write
    }
    if (address <= 0xBFFF) {
        return;                             // external cartridge RAM - TODO: cart_write
    }
    if (address <= 0xDFFF) {
        ctx.wram[address - 0xC000] = value; // work RAM
        return;
    }
    if (address <= 0xFDFF) {
        ctx.wram[address - 0xE000] = value; // echo RAM, mirrors WRAM
        return;
    }
    if (address <= 0xFE9F) {
        return;                             // OAM - TODO: ppu_oam_write
    }
    if (address <= 0xFEFF) {
        return;                             // not usable
    }
    if (address <= 0xFF7F) {
        return;                             // I/O registers - TODO: io_write
    }
    if (address <= 0xFFFE) {
        ctx.hram[address - 0xFF80] = value; // high RAM
        return;
    }
    ctx.ie_register = value;                // 0xFFFF, interrupt enable
}
