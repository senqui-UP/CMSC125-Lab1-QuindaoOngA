#include <stdio.h>
#include "shell.h"

int main() {

    char line[1024];

    while (1) {

        printf("mysh> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
    }

    return 0;
}