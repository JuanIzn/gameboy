#pragma once

#include "common.h"

struct bus_context {
    u8 wram[8000];
    u8 hram[127];
};

u8 bus_read(u16 address);