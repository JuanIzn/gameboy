#pragma once



typedef struct {
    bool paused;
    bool running;
    u64 ticks;
} emu_context;