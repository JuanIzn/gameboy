#include "common.h"

#pragma once
/** 
 *  We will define the cart memory structure
 */
typedef struct {
    u8 entry[4];
    u8 nintendo_logo[48]; 
    u8 title[16]; 
    size_t rom_size; 
    u8 version[];

    u8 checksum[];
} rom_header;

typedef struct {
    u8 *rom_data;
    u32 *rom_size
    char *filename;

    rom_header *header;
} cart_context;

void load_cart();
void read_cart();