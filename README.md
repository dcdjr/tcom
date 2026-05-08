# TCOM

TCOM is a small 8-bit computer emulator written in C. It models the core parts of a simple CPU, including registers, memory, instruction decoding, branching, and flag-based execution.

The project was built to make low-level machine behavior concrete: how instructions are encoded, how CPU state changes over time, and how a small instruction set can be tested systematically.

## Overview

The emulator implements a compact instruction set architecture with:

- 4 general-purpose 8-bit registers
- 256 bytes of memory
- A program counter
- A zero flag
- A halt state
- Fixed-width 3-byte instructions

The project supports arithmetic, control flow, memory access, and register comparison, making it large enough to demonstrate real ISA and execution-model design while still being small enough to reason about completely.

## Instruction Set

TCOM currently supports:

- `NOP`
- `HALT`
- `LOADI`
- `ADD`
- `SUB`
- `JMP`
- `JZ`
- `LOAD`
- `STORE`
- `CMP`

Each instruction is encoded as:

```text
[ opcode ][ operand1 ][ operand2 ]
```

Because the ISA uses fixed-width 3-byte instructions, all valid jump targets must align to instruction boundaries.

## Execution Model

Programs are loaded into memory with `cpu_load_program()` and executed through `cpu_step()` or `cpu_run()`.

The emulator enforces several deliberate invariants:

- Code occupies memory range `[0, program_size)`
- Data occupies memory range `[program_size, 255]`
- `LOAD` and `STORE` cannot access the code region
- Jumps must remain within the loaded program
- Jump targets must align to instruction boundaries
- Non-jump instructions advance the program counter explicitly
- Valid programs are expected to end with `HALT`

## Flags

TCOM currently uses a single status flag:

- `zf` — zero flag

The zero flag is updated by:

- `LOADI`
- `ADD`
- `SUB`
- `CMP`

`LOAD` and `STORE` do not affect flags.

## Example Program

```text
LOADI R0, 3
LOADI R1, 4
LOADI R2, 5
ADD   R0, R1
SUB   R0, R2
HALT
```

This program computes:

```text
R0 = (3 + 4) - 5 = 2
```

## Testing

The project includes a dedicated test suite in `tests/test_cpu.c` covering:

- Arithmetic execution
- Unconditional and conditional jumps
- Memory load/store behavior
- Comparison behavior
- Invalid jump targets
- Invalid writes into the code region

Run the tests with:

```bash
make test
```

## Build

```bash
make
```

## Run

```bash
make run
```

## Why This Project Matters

TCOM demonstrates practical understanding of:

- Instruction encoding
- Register versus memory semantics
- CPU state management
- Branching and control flow
- Flag-driven execution
- Deliberate memory-model constraints
- Targeted correctness testing

Although the machine is intentionally minimal, the design process reflects real systems concerns: defining an ISA, enforcing invariants, validating execution behavior, and proving correctness through tests.
