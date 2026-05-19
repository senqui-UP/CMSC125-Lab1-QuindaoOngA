#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"

int main() {

    char line[1024];

    while (1) {

        cleanup_background_jobs();

        printf("mysh> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        /* remove newline */
        line[strcspn(line, "\n")] = '\0';

        /* ignore empty input */
        if (strlen(line) == 0)
            continue;

        Command cmd;

        char original_cmd[1024];
        strcpy(original_cmd, line);

        if (!parse_command(line, &cmd)) {
            continue;
        }

        execute_command(&cmd, original_cmd);
    }

    return 0;
}