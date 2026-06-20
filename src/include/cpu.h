#pragma once

#include "common.h"

// CPU registers. Most are 8-bit but pair up into 16-bit registers.
// The union lets each pair be read/written as one 16-bit value OR as
// its two 8-bit halves, sharing the same memory (just like the hardware).
//
// The low byte is declared FIRST because the Game Boy is little-endian:
// the least significant byte lives at the lower address. So in af, f is
// the low byte and a is the high byte.
typedef struct {
    union {
        u16 af;
        struct {
            u8 f;   // flags register (low byte, not addressable by name)
            u8 a;   // accumulator (high byte)
        };
    };
    union {
        u16 bc;
        struct {
            u8 c;
            u8 b;
        };
    };
    union {
        u16 de;
        struct {
            u8 e;
            u8 d;
        };
    };
    union {
        u16 hl;
        struct {
            u8 l;
            u8 h;
        };
    };
    u16 sp;   // stack pointer: top of the stack
    u16 pc;   // program counter: address of the next instruction
} cpu_registers;

// Bit positions of the flags inside register F.
// Bits 0-3 are always 0 and unused.
#define FLAG_Z 7   // Zero:       last result was 0
#define FLAG_N 6   // Subtract:   last op was a subtraction (for BCD)
#define FLAG_H 5   // Half-carry: carry from bit 3 to 4 (for BCD)
#define FLAG_C 4   // Carry:      result overflowed 8 bits

typedef struct {
    cpu_registers regs;
    // More state will go here later: interrupt master enable,
    // halted flag, the opcode currently being decoded, etc.
} cpu_context;

// Sets the registers to their power-up values.
void cpu_init();

// Runs one full instruction: fetch the opcode, advance PC, execute it.
void cpu_step();
