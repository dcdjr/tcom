#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "isa.h"

// Structure to hold CPU data
typedef struct {
    uint8_t regs[NUM_REGS];
    uint8_t memory[NUM_MEMORY_BYTES];
    uint8_t pc; // Program counter
    size_t program_size; // Current loaded program size
    bool zf; // Zero flag
    bool halted;
} CPU;

void cpu_init(CPU *cpu);
void cpu_dump(const CPU *cpu);
bool cpu_load_program(CPU *cpu, const uint8_t *program, size_t program_size);
bool cpu_step(CPU *cpu);
bool cpu_run(CPU *cpu);

#endif
