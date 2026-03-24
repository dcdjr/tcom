#include "cpu.h"
#include "isa.h"

void cpu_init(CPU *cpu) {
    // Zero registers and memory
    for (int i = 0; i < NUM_REGS; i++) cpu->regs[i] = 0;
    for (int i = 0; i < NUM_MEMORY_BYTES; i++) cpu->memory[i] = 0;
    
    // Initialize pc, zf, and halted
    cpu->pc = 0;
    cpu->zf = 0;
    cpu->halted = false;
}
