#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>
#include "isa.h"

// Structure to hold CPU data
typedef struct {
    uint8_t regs[NUM_REGS];
    uint8_t memory[NUM_MEMORY_BYTES];
    uint8_t pc; // Program counter
    uint8_t zf; // Zero flag
    bool halted;
} CPU;

void cpu_init(CPU *cpu);
void cpu_dump(const CPU *cpu);

#endif
