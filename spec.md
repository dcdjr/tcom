# Tiny Computer v0.1

## Machine
- 8-bit machine
- 256 bytes of memory
- 4 registers: R0, R1, R2, R3
- 1 program counter: PC
- 1 zero flag: ZF
- instructions are 3 bytes each

## Instructions
- NOP
- HALT
- LOADI reg, imm
- ADD regA, regB
- SUB regA, regB
- CMP regA, regB
- JMP addr
- JZ addr
- JNZ addr

## Rules
- `ADD regA, regB` means `regA = regA + regB`
- `SUB regA, regB` means `regA = regA - regB`
- values wrap as 8-bit unsigned
- normal instructions do `PC += 3`
- jumps set `PC` directly
- `CMP` sets `ZF = 1` if equal, else `0`
- invalid opcode halts the machine

## First demo
Program:
1. load 3 into R0
2. load 4 into R1
3. add R1 into R0
4. halt

Expected result:
- R0 = 7
