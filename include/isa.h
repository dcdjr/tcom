#ifndef ISA_H
#define ISA_H

/* This header defines tcom's instruction set architecture (ISA).
   The main invariants are as follows:
    1. Every instruction is 3 bytes.
    2. Code region of memory is [0, program-size), data region is [program_size, 255].
    3. Jumps must target valid instruction boundaries.
    4. LOAD/STORE cannot access memory in code region.
    5. LOAD/STORE do not effect flags as they are not arithmetic or comparison operations. 
    6. CMP sets zf based on register value equality. 
    7. Valid programs should end with HALT, but advance_pc will not allow programs to execute beyond their size.
    8. Every non-jump instruction should require advance_pc. */

// Define machine constants
#define NUM_REGS 4
#define NUM_MEMORY_BYTES 256
#define INSTRUCTION_WIDTH 3

// Define opcodes
typedef enum {
    NOP = 1,
    HALT,
    LOADI,
    ADD,
    SUB,
    JMP,
    JZ,
    LOAD,
    STORE,
    CMP
} Opcode;

// Define register values
typedef enum {
    R0,
    R1,
    R2,
    R3
} Register;

#endif
