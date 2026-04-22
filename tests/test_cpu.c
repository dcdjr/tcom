#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"

typedef struct {
    const char *name;
    const uint8_t *program;
    size_t program_size;
    bool expect_load_ok;
    bool expect_run_ok;
    bool check_regs[NUM_REGS];
    uint8_t expected_regs[NUM_REGS];
    bool check_zf;
    bool expected_zf;
    bool check_halted;
    bool expected_halted;
} TestCase;

static bool run_test(const TestCase *test) {
    CPU cpu;
    cpu_init(&cpu);

    bool load_ok = cpu_load_program(&cpu, test->program, test->program_size);
    if (load_ok != test->expect_load_ok) {
        printf("[FAIL] %s: cpu_load_program returned %s, expected %s\n",
               test->name,
               load_ok ? "true" : "false",
               test->expect_load_ok ? "true" : "false");
        return false;
    }

    if (!load_ok) {
        printf("[PASS] %s\n", test->name);
        return true;
    }

    bool run_ok = cpu_run(&cpu);
    if (run_ok != test->expect_run_ok) {
        printf("[FAIL] %s: cpu_run returned %s, expected %s\n",
               test->name,
               run_ok ? "true" : "false",
               test->expect_run_ok ? "true" : "false");
        return false;
    }

    for (int i = 0; i < NUM_REGS; i++) {
        if (test->check_regs[i] && cpu.regs[i] != test->expected_regs[i]) {
            printf("[FAIL] %s: R%d = %u, expected %u\n",
                   test->name, i, cpu.regs[i], test->expected_regs[i]);
            return false;
        }
    }

    if (test->check_zf && cpu.zf != test->expected_zf) {
        printf("[FAIL] %s: zf = %s, expected %s\n",
               test->name,
               cpu.zf ? "true" : "false",
               test->expected_zf ? "true" : "false");
        return false;
    }

    if (test->check_halted && cpu.halted != test->expected_halted) {
        printf("[FAIL] %s: halted = %s, expected %s\n",
               test->name,
               cpu.halted ? "true" : "false",
               test->expected_halted ? "true" : "false");
        return false;
    }

    printf("[PASS] %s\n", test->name);
    return true;
}

/* 1. Arithmetic: R0 = (3 + 4) - 5 = 2 */
static const uint8_t test_arithmetic[] = {
    LOADI, R0, 3,
    LOADI, R1, 4,
    LOADI, R2, 5,
    ADD,   R0, R1,
    SUB,   R0, R2,
    HALT,  0,  0
};

/* 2. JMP skips LOADI R0,99 and lands on HALT */
static const uint8_t test_jmp_skip[] = {
    LOADI, R0, 1,
    JMP,   9,  0,
    LOADI, R0, 99,
    HALT,  0,  0
};

/* 3. JZ taken because LOADI R0,0 sets zf = true */
static const uint8_t test_jz_taken[] = {
    LOADI, R0, 0,
    JZ,    9,  0,
    LOADI, R1, 99,
    HALT,  0,  0
};

/* 4. JZ not taken because LOADI R0,5 sets zf = false */
static const uint8_t test_jz_not_taken[] = {
    LOADI, R0, 5,
    JZ,    9,  0,
    LOADI, R1, 99,
    HALT,  0,  0
};

/* 5. STORE/LOAD roundtrip through first valid data address (12) */
static const uint8_t test_memory_roundtrip[] = {
    LOADI, R0, 42,
    STORE, R0, 12,
    LOAD,  R1, 12,
    HALT,  0,  0
};

/* 6. CMP equal, branch taken, R2 stays 0 */
static const uint8_t test_cmp_equal[] = {
    LOADI, R0, 8,
    LOADI, R1, 8,
    CMP,   R0, R1,
    JZ,    15, 0,
    LOADI, R2, 99,
    HALT,  0,  0
};

/* 7. CMP not equal, branch not taken, halts normally */
static const uint8_t test_cmp_not_equal[] = {
    LOADI, R0, 8,
    LOADI, R1, 5,
    CMP,   R0, R1,
    JZ,    15, 0,
    HALT,  0,  0
};

/* 8. Invalid jump target: not aligned to instruction width */
static const uint8_t test_invalid_jump_alignment[] = {
    JMP,   4,  0,
    HALT,  0,  0
};

/* 9. Invalid store into code region */
static const uint8_t test_invalid_store_code_region[] = {
    LOADI, R0, 55,
    STORE, R0, 0,
    HALT,  0,  0
};

int main(void) {
    const TestCase tests[] = {
        {
            .name = "arithmetic",
            .program = test_arithmetic,
            .program_size = sizeof(test_arithmetic),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, false, false, false},
            .expected_regs = {2, 0, 0, 0},
            .check_zf = true,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "jmp_skip",
            .program = test_jmp_skip,
            .program_size = sizeof(test_jmp_skip),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, false, false, false},
            .expected_regs = {1, 0, 0, 0},
            .check_zf = true,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "jz_taken",
            .program = test_jz_taken,
            .program_size = sizeof(test_jz_taken),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, true, false, false},
            .expected_regs = {0, 0, 0, 0},
            .check_zf = true,
            .expected_zf = true,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "jz_not_taken",
            .program = test_jz_not_taken,
            .program_size = sizeof(test_jz_not_taken),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, true, false, false},
            .expected_regs = {5, 99, 0, 0},
            .check_zf = true,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "memory_roundtrip",
            .program = test_memory_roundtrip,
            .program_size = sizeof(test_memory_roundtrip),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, true, false, false},
            .expected_regs = {42, 42, 0, 0},
            .check_zf = false,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "cmp_equal",
            .program = test_cmp_equal,
            .program_size = sizeof(test_cmp_equal),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, true, true, false},
            .expected_regs = {8, 8, 0, 0},
            .check_zf = true,
            .expected_zf = true,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "cmp_not_equal",
            .program = test_cmp_not_equal,
            .program_size = sizeof(test_cmp_not_equal),
            .expect_load_ok = true,
            .expect_run_ok = true,
            .check_regs = {true, true, false, false},
            .expected_regs = {8, 5, 0, 0},
            .check_zf = true,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = true
        },
        {
            .name = "invalid_jump_alignment",
            .program = test_invalid_jump_alignment,
            .program_size = sizeof(test_invalid_jump_alignment),
            .expect_load_ok = true,
            .expect_run_ok = false,
            .check_regs = {false, false, false, false},
            .expected_regs = {0, 0, 0, 0},
            .check_zf = false,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = false
        },
        {
            .name = "invalid_store_code_region",
            .program = test_invalid_store_code_region,
            .program_size = sizeof(test_invalid_store_code_region),
            .expect_load_ok = true,
            .expect_run_ok = false,
            .check_regs = {true, false, false, false},
            .expected_regs = {55, 0, 0, 0},
            .check_zf = true,
            .expected_zf = false,
            .check_halted = true,
            .expected_halted = false
        }
    };

    const size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;

    for (size_t i = 0; i < num_tests; i++) {
        if (run_test(&tests[i])) {
            passed++;
        }
    }

    printf("\n%zu/%zu tests passed\n", passed, num_tests);
    return (passed == num_tests) ? 0 : 1;
}
