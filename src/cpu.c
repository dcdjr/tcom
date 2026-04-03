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
    
    // Initialize pc, zf, program_size, and halted
    cpu->pc = 0;
    cpu->zf = false;
    cpu->program_size = 0;
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
    const char *zf_status = cpu->zf ? "true" : "false";
    printf("Zero flag: %s\n", zf_status);
    const char *halted_status = cpu->halted ? "true" : "false";
    printf("Halted status: %s\n", halted_status);
    printf("----------------------\n");
}

bool cpu_load_program(CPU *cpu, const uint8_t *program, size_t program_size) {
    if (!cpu) return false;
    if (!program) return false;
    if (program_size > NUM_MEMORY_BYTES) return false;

    // Reset execution state
    cpu_init(cpu);
    
    cpu->program_size = program_size;
    memcpy(cpu->memory, program, program_size);
    return true;
}

static bool advance_pc(CPU *cpu) {
    size_t next_pc = (size_t)cpu->pc + INSTRUCTION_WIDTH;
    if (next_pc >= cpu->program_size) return false;
    cpu->pc = (uint8_t)next_pc;
    return true;
}

bool cpu_step(CPU *cpu) {
    if (!cpu) return false;
    if (cpu->halted) return false;

    Opcode opcode = cpu->memory[cpu->pc];

    uint8_t operand1;
    uint8_t operand2;

    if (cpu->pc + INSTRUCTION_WIDTH <= cpu->program_size) {
        operand1 = cpu->memory[cpu->pc + 1];
        operand2 = cpu->memory[cpu->pc + 2];
    } else {
        return false;
    }
    
    switch (opcode) {
        case NOP:
            if (!advance_pc(cpu)) return false;
            break;
        case HALT:
            cpu->halted = true;
            return true;
        case LOADI:
            if (operand1 >= NUM_REGS) return false;
            cpu->regs[operand1] = operand2;
            cpu->zf = (cpu->regs[operand1] == 0);
            if (!advance_pc(cpu)) return false;
            break;
        case ADD:
            if (operand1 >= NUM_REGS) return false;
            if (operand2 >= NUM_REGS) return false;
            cpu->regs[operand1] += cpu->regs[operand2];
            cpu->zf = (cpu->regs[operand1] == 0);
            if (!advance_pc(cpu)) return false;
            break;
        case SUB:
            if (operand1 >= NUM_REGS) return false;
            if (operand2 >= NUM_REGS) return false;
            cpu->regs[operand1] -= cpu->regs[operand2];
            cpu->zf = (cpu->regs[operand1] == 0);
            if (!advance_pc(cpu)) return false;
            break;
        default:
            printf("Invalid opcode.\n");
            return false;
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
        if (!cpu_step(cpu)) {
            printf("Failed step state:\n");
            cpu_dump(cpu);
            return false;
        }
        step++;
    }
    cpu_dump(cpu);
    return true;
}
