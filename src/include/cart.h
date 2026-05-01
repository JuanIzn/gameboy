#include "common.h"

typedef struct {
    u8 entry[4];
    u8 nintendo_logo[48]; 
    u8 title[16]; 
    size_t rom_size; 
} rom_header;