#include <fstop.h>
#include <stdio.h>

int main() {
    for (int i = -3; i <= 3; i++) {
        unsigned long t = generateTime(42000, 0.25, i);
        printf("Time: %d\n", t);
    }
}