#include "cpu.h"

int main(void) {
    CPU cpu;

    cpu_init(&cpu);

    if (!cpu_load_program(&cpu, program3, 18)) {
        printf("Loading program failed.\n");
        return 1;
    }

    cpu_run(&cpu);

    return 0;
}
