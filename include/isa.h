#ifndef ISA_H
#define ISA_H

// Define machine constants
#define NUM_REGS 4
#define NUM_MEMORY_BYTES 256
#define INSTRUCTION_WIDTH 3

// Define opcodes
typedef enum {
    NOP,
    HALT,
    LOADI,
    ADD
} Opcode;

// Define register values
typedef enum {
    R0,
    R1,
    R2,
    R3
} Register;

#endif
