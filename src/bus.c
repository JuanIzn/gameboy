#include "include/bus.h"
#include "include/cart.h"
#include <stdio.h>
#include <string.h>

u8 bus_read(u16 address) {
    u8 byte = cart_read(address);
}