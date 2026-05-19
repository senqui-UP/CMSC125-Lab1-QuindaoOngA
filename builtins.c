#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

int handle_builtin(Command *cmd) {

    if (cmd->command == NULL)
        return 1;

    /* exit */
    if (strcmp(cmd->command, "exit") == 0) {

        printf("Exiting mysh...\n");
        exit(0);
    }

    /* cd */
    if (strcmp(cmd->command, "cd") == 0) {

        if (cmd->args[1] == NULL) {

            fprintf(stderr, "cd: missing argument\n");

        } else {

            if (chdir(cmd->args[1]) != 0) {
                perror("cd");
            }
        }

        return 1;
    }

    /* pwd */
    if (strcmp(cmd->command, "pwd") == 0) {

        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {

            printf("%s\n", cwd);

        } else {

            perror("pwd");
        }

        return 1;
    }

    return 0;
}