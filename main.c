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

        /* keep a safe copy of the original for background job display;
           parse_command now works on its own internal copy so line is
           no longer mutated by strtok */
        char original_cmd[1024];
        strncpy(original_cmd, line, sizeof(original_cmd) - 1);
        original_cmd[sizeof(original_cmd) - 1] = '\0';

        Command cmd;

        if (!parse_command(line, &cmd)) {
            continue;
        }

        execute_command(&cmd, original_cmd);
    }

    return 0;
}