#include <stdio.h>
#include "cpu.h"
#include "isa.h"

int main(void) {
    CPU cpu;

    cpu_init(&cpu);

    cpu_dump(&cpu);

    return 0;
}
