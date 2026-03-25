#include <stdio.h>
#include <inttypes.h>
#include "cpu.h"

// Initializes CPU
void cpu_init(CPU *cpu) {
    // Zero registers and memory
    for (int i = 0; i < NUM_REGS; i++) cpu->regs[i] = 0;
    for (int i = 0; i < NUM_MEMORY_BYTES; i++) cpu->memory[i] = 0;
    
    // Initialize pc, zf, and halted
    cpu->pc = 0;
    cpu->zf = 0;
    cpu->halted = false;
}

// Dumps current CPU state
void cpu_dump(const CPU *cpu) {
    printf("------ CPU DUMP ------\n");
    printf("Program counter: %" PRIu8 "\n", cpu->pc);
    printf("Register contents: \n");
    printf(" REG0: %" PRIu8 "\n", cpu->regs[0]);
    printf(" REG1: %" PRIu8 "\n", cpu->regs[1]);
    printf(" REG2: %" PRIu8 "\n", cpu->regs[2]);
    printf(" REG3: %" PRIu8 "\n", cpu->regs[3]);
    printf("Zero flag: %" PRIu8 "\n", cpu->zf);
    const char *str = cpu->halted ? "true" : "false";
    printf("Halted status: %s\n", str);
    printf("----------------------\n");
}
