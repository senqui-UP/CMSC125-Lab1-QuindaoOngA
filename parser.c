#include <stdio.h>
#include <string.h>

#include "shell.h"

int parse_command(char *line, Command *cmd) {

    cmd->command = NULL;

    cmd->input_file = NULL;
    cmd->output_file = NULL;

    cmd->append = 0;
    cmd->background = 0;

    int i = 0;

    char *token = strtok(line, " ");

    while (token != NULL) {

        if (strcmp(token, "<") == 0) {

            token = strtok(NULL, " ");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing input file\n");
                return 0;
            }

            cmd->input_file = token;
        }

        else if (strcmp(token, ">") == 0) {

            token = strtok(NULL, " ");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing output file\n");
                return 0;
            }

            cmd->output_file = token;
            cmd->append = 0;
        }

        else if (strcmp(token, ">>") == 0) {

            token = strtok(NULL, " ");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing output file\n");
                return 0;
            }

            cmd->output_file = token;
            cmd->append = 1;
        }

        else if (strcmp(token, "&") == 0) {

            token = strtok(NULL, " ");

            if (token != NULL) {
                fprintf(stderr,
                        "Syntax error: '&' must be at end\n");
                return 0;
            }

            cmd->background = 1;
            break;
        }

        else {

            if (i >= 255) {
                fprintf(stderr,
                        "mysh: too many arguments (max 255)\n");
                return 0;
            }

            cmd->args[i++] = token;
        }

        token = strtok(NULL, " ");
    }

    cmd->args[i] = NULL;

    if (i > 0) {
        cmd->command = cmd->args[0];
    }

    return 1;
}