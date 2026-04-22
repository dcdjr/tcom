#include "cpu.h"

int main(void) {
    CPU cpu;

    cpu_init(&cpu);

    // uint8_t program[] = {
    //                         LOADI, R0, 3,
    //                         LOADI, R1, 4,
    //                         LOADI, R2, 5,
    //                         ADD, R0, R1,
    //                         SUB, R0, R2,
    //                         HALT, 0, 0
    // };

    // uint8_t program2[] = {
    //                         LOADI, R0, 1,
    //                         JMP, 0, 0,
    //                         LOADI, R0, 99,
    //                         HALT, 0, 0
    // };
    
    uint8_t program3[] = {
                            LOADI, R0, 0,
                            JZ, 9, 9,
                            LOADI, R1, 99,
                            HALT, 0, 0
    };

    if (!cpu_load_program(&cpu, program3, 18)) {
        printf("Loading program failed.\n");
        return 1;
    }

    cpu_run(&cpu);

    return 0;
}
