#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "cpu.h"

// Initializes CPU
void cpu_init(CPU *cpu) {
    if (!cpu) return;

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
    if (!cpu) return;

    printf("------ CPU DUMP ------\n");
    printf("Program counter: %" PRIu8 "\n", cpu->pc);
    printf("Register contents: \n");
    printf(" REG0: %" PRIu8 "\n", cpu->regs[R0]);
    printf(" REG1: %" PRIu8 "\n", cpu->regs[R1]);
    printf(" REG2: %" PRIu8 "\n", cpu->regs[R2]);
    printf(" REG3: %" PRIu8 "\n", cpu->regs[R3]);
    printf("Zero flag: %" PRIu8 "\n", cpu->zf);
    const char *str = cpu->halted ? "true" : "false";
    printf("Halted status: %s\n", str);
    printf("----------------------\n");
}

bool cpu_load_program(CPU *cpu, const uint8_t *program, size_t program_size) {
    if (!cpu) return false;
    if (program_size > NUM_MEMORY_BYTES) return false;

    memcpy(cpu->memory, program, program_size);
    return true;
}

bool cpu_step(CPU *cpu) {
    if (!cpu) return false;
    if (cpu->halted) return false;

    Opcode opcode = cpu->memory[cpu->pc];
    size_t operand1 = cpu->memory[cpu->pc + 1];
    size_t operand2 = cpu->memory[cpu->pc + 2];

    switch (opcode) {
        case NOP:
            cpu->pc += INSTRUCTION_WIDTH;
            break;
        case HALT:
            cpu->halted = true;
            return true;
            break;
        case LOADI:
            cpu->regs[operand1] = operand2;
            cpu->zf = (cpu->regs[operand1] == 0) ? 1 : 0;
            cpu->pc += INSTRUCTION_WIDTH;
            break;
        case ADD:
            cpu->regs[operand1] += cpu->regs[operand2];
            cpu->zf = (cpu->regs[operand1] == 0) ? 1 : 0;
            cpu->pc += INSTRUCTION_WIDTH;
            break;
        default:
            break;
    }

    return true;
}

bool cpu_run(CPU *cpu) {
    if (!cpu) return false;

    int step = 1;
    printf("Initial state:\n");
    while (!cpu->halted) {
        cpu_dump(cpu);
        printf("Step %i:\n", step);
        if (!cpu_step(cpu)) return false;
        step++;
    }
    cpu_dump(cpu);
    return true;
}
