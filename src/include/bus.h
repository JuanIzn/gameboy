#pragma once

#include "common.h"

typedef struct {
    u8 wram[0x2000];   // 0xC000-0xDFFF, 8 KB work RAM
    u8 hram[0x80];     // 0xFF80-0xFFFE, high RAM (127 used bytes)
    u8 ie_register;    // 0xFFFF, interrupt enable (moves to interrupts later)
} bus_context;

void bus_init();

// Reads a byte from anywhere in the 16-bit address space.
// Routes the address to its owner (cartridge, WRAM, PPU...).
u8 bus_read(u16 address);

// Writes a byte to anywhere in the 16-bit address space.
void bus_write(u16 address, u8 value);
