#include "cpu.h"

int main(void) {
    CPU cpu;

    cpu_init(&cpu);

    uint8_t program[] = {
                            LOADI, R0, 3,
                            LOADI, R1, 4,
                            LOADI, R2, 5,
                            ADD, R0, R1,
                            SUB, R0, R2,
                            HALT, 0, 0
    };

    if (!cpu_load_program(&cpu, program, 18)) {
        printf("Loading program failed.\n");
        return 1;
    }

    cpu_run(&cpu);

    return 0;
}
