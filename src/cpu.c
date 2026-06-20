#include "include/cpu.h"
#include "include/bus.h"
#include <stdio.h>

static cpu_context ctx;

// Power-up register values for the original DMG Game Boy.
// Ref: https://gbdev.io/pandocs/Power_Up_Sequence.html
void cpu_init() {
    ctx.regs.af = 0x01B0;
    ctx.regs.bc = 0x0013;
    ctx.regs.de = 0x00D8;
    ctx.regs.hl = 0x014D;
    ctx.regs.sp = 0xFFFE;
    ctx.regs.pc = 0x0100;   // execution starts right after the cartridge header
}

void cpu_step() {
    u8 opcode = bus_read(ctx.regs.pc);   // FETCH: read the byte PC points to
    ctx.regs.pc++;                       // advance PC to the next byte
    execute(opcode);                     // DECODE + EXECUTE
}

// Decodes and executes one opcode. For now it only knows NOP; the full
// instruction set (the big switch) will grow here as the next step.
static void execute(u8 opcode) {
    switch (opcode) {
        case 0x00:   // NOP: do nothing
            break;
        default:
            printf("Unimplemented opcode: 0x%02X at PC=0x%04X\n",
                   opcode, ctx.regs.pc - 1);
            break;
    }
}


