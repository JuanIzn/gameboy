#include "include/cpu.h"
#include "include/bus.h"
#include <stdio.h>

static cpu_context ctx;

// Forward declaration: lets cpu_step call execute even though the full
// definition lives at the bottom of the file (it will be the longest).
static void execute(u8 opcode);

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

// --- Flag helpers -----------------------------------------------------------
// Register F holds 4 flags in bits 4-7 (see FLAG_* in cpu.h). These read and
// write a single flag bit without touching the others.

// Returns true if the given flag bit is set in F.
static bool get_flag(u8 flag) {
    return (ctx.regs.f >> flag) & 1;
}

// Sets the given flag bit to 1 (value true) or 0 (value false).
static void set_flag(u8 flag, bool value) {
    if (value) {
        ctx.regs.f |= (1 << flag);    // OR with a 1 in that position -> set it
    } else {
        ctx.regs.f &= ~(1 << flag);   // AND with a 0 in that position -> clear it
    }
}

// Decodes and executes one opcode. For now it only knows NOP; the full
// instruction set (the big switch) will grow here as the next step.
static void execute(u8 opcode) {
    switch (opcode) {
        case 0x00:   // NOP: do nothing
            break;

        case 0xC3: {   // JP nn: jump to the 16-bit address that follows
            u8 lo = bus_read(ctx.regs.pc);   // low byte first (little-endian)
            ctx.regs.pc++;
            u8 hi = bus_read(ctx.regs.pc);   // then high byte
            ctx.regs.pc++;
            ctx.regs.pc = (hi << 8) | lo;    // combine both into the target
            break;
        }

        case 0xFE: {   // CP n: compare A with the next byte (A - n, result discarded)
            u8 n = bus_read(ctx.regs.pc);
            ctx.regs.pc++;
            u8 a = ctx.regs.a;
            set_flag(FLAG_Z, a == n);                    // equal -> result would be 0
            set_flag(FLAG_N, true);                      // CP is a subtraction
            set_flag(FLAG_H, (a & 0x0F) < (n & 0x0F));   // borrow from bit 4
            set_flag(FLAG_C, a < n);                      // borrow (A < n)
            break;
        }

        default:
            printf("Unimplemented opcode: 0x%02X at PC=0x%04X\n",
                   opcode, ctx.regs.pc - 1);
            break;
    }
}


