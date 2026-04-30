//main loop here

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    //CPU
    uint8_t registers[8]; //A, B, C, D, E, F, H, L
    //Memory
    uint8_t memory[0x10000]; //64 KB

    //PPU (Grphics..)

    int running;
    int cycles; 
} GameBoy;

GameBoy* gb_init() {
    Gameboy* gb
}

void main() {

    char[] game = game_menu_selector();
    load_game(game);


}