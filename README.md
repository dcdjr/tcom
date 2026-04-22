# tcom

tcom is a tiny 8-bit computer emulator written in C. I built it as a low-level systems project to better understand how a very small CPU can represent state, execute instructions, handle control flow, and work with memory.

The goal of the project is not to simulate a real commercial processor in detail, but to build a clean, deliberate miniature machine with a simple instruction set and clear execution rules.

## What tcom includes

tcom currently supports:

- 4 general-purpose 8-bit registers
- 256 bytes of memory
- a program counter
- a zero flag
- a halt state
- fixed-width 3-byte instructions
- arithmetic operations
- control flow instructions
- memory load/store instructions
- register comparison through `CMP`

## CPU model

The CPU stores:

- `regs[4]` for the four registers
- `memory[256]` for all addressable memory
- `pc` for the program counter
- `program_size` for the size of the currently loaded program
- `zf` for the zero flag
- `halted` to indicate whether execution has stopped

This is a very small machine, but it still has the same core pieces as a more serious CPU model: state, memory, instruction decoding, and an execution loop.

## Instruction format

Every instruction in tcom is exactly 3 bytes wide:

```c
[ opcode ][ operand1 ][ operand2 ]
