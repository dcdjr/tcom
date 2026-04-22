# tcom

tcom is a small 8-bit computer emulator written in C that models the core parts of a simple CPU, including registers, memory, instruction decoding, branching, and flag-based execution. I built it as a systems project to better understand how low-level machine behavior is represented in software.

## Overview

The emulator implements a compact instruction set architecture with:

- 4 general-purpose 8-bit registers
- 256 bytes of memory
- a program counter
- a zero flag
- a halt state
- fixed-width 3-byte instructions

The project supports arithmetic, control flow, memory access, and register comparison, making it large enough to demonstrate real ISA and execution-model design while still being small enough to reason about completely.

## Instruction Set

tcom currently supports the following instructions:

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

The emulator uses a simple fetch-decode-execute cycle and enforces several deliberate invariants:

- code occupies memory range `[0, program_size)`
- data occupies memory range `[program_size, 255]`
- `LOAD` and `STORE` cannot access the code region
- jumps must remain within the loaded program and target valid instruction boundaries
- non-jump instructions advance the program counter explicitly
- valid programs are expected to end with `HALT`

## Flags

tcom currently uses a single status flag:

- `zf` (zero flag)

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

- arithmetic execution
- unconditional and conditional jumps
- memory load/store behavior
- comparison behavior
- invalid jump targets
- invalid writes into the code region

Run the tests with:

```bash
make test
```

## Why This Project Matters

tcom was built to demonstrate practical understanding of:

- instruction encoding
- register vs. memory semantics
- CPU state management
- branching and control flow
- flag-driven execution
- deliberate memory-model constraints

Although the machine is intentionally minimal, the design process reflects real systems concerns: defining an ISA, enforcing invariants, validating execution behavior, and proving correctness through targeted tests.

## Build

```bash
make
```

## Run

```bash
make run
```

