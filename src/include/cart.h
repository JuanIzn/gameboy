#pragma once

#include "common.h"

// Exact layout of the cartridge header in ROM (0x0100 - 0x014F)
// Ref: https://gbdev.io/pandocs/The_Cartridge_Header.html
typedef struct {
    u8 entry[4];           // 0x0100 - 0x0103  entry point
    u8 nintendo_logo[48];  // 0x0104 - 0x0133  validated by boot ROM
    u8 title[16];          // 0x0134 - 0x0143  game title in ASCII
    u8 new_lic_code[2];    // 0x0144 - 0x0145
    u8 sgb_flag;           // 0x0146
    u8 type;               // 0x0147  cartridge MBC type
    u8 rom_size;           // 0x0148  code: 0x00=32KB, 0x01=64KB...
    u8 ram_size;           // 0x0149
    u8 dest_code;          // 0x014A
    u8 lic_code;           // 0x014B
    u8 version;            // 0x014C
    u8 checksum;           // 0x014D  verified on startup
    u8 global_checksum[2]; // 0x014E - 0x014F
} __attribute__((packed)) rom_header;

typedef struct {
    char      *filename;
    u8        *rom_data;  // full contents of the .gb file
    u32        rom_size;  // actual file size in bytes
    rom_header *header;   // points to rom_data + 0x0100, where the header starts
} cart_context;

// Loads the .gb file into memory and validates the header.
// Returns true if the cartridge is valid.
bool cart_load(char *path);

// Reads a byte from the cartridge given a memory bus address.
// The address is u16 because the Game Boy address space spans
// 0x0000 - 0xFFFF (65536 positions), which needs 16 bits. It returns just a single byte.
u8 cart_read(u16 address);
