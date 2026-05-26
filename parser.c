#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shell.h"

// Strip trailing '&' from a token in-place
static int strip_background(char *token, Command *cmd) {
    int len = strlen(token);
    if (len > 1 && token[len - 1] == '&') {
        token[len - 1] = '\0';
        cmd->background = true;
        return 1;
    }
    return 0;
}

// Free dynamically allocated command data
void free_command(Command *cmd) {
    for (int i = 0; i < cmd->arg_count; i++) {
        free(cmd->args[i]);
        cmd->args[i] = NULL;
    }

    if (cmd->input_file != NULL) {
        free(cmd->input_file);
        cmd->input_file = NULL;
    }

    if (cmd->output_file != NULL) {
        free(cmd->output_file);
        cmd->output_file = NULL;
    }

    cmd->command = NULL;
    cmd->arg_count = 0;
}

int parse_command(char *line, Command *cmd) {

    cmd->command = NULL;

    cmd->input_file = NULL;
    cmd->output_file = NULL;

    cmd->append = 0;
    cmd->background = 0;
    cmd->arg_count = 0;

    /* work on a copy so the caller's buffer is not mutated by strtok */
    static char buf[1024];
    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int i = 0;

    char *token = strtok(buf, " \t");

    while (token != NULL) {

        strip_background(token, cmd);

        if (strcmp(token, "<") == 0) {

            token = strtok(NULL, " \t");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing input file\n");
                return 0;
            }

            cmd->input_file = strdup(token);
        }

        else if (strcmp(token, ">") == 0) {

            token = strtok(NULL, " \t");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing output file\n");
                return 0;
            }

            cmd->output_file = strdup(token);
            cmd->append = 0;
        }

        else if (strcmp(token, ">>") == 0) {

            token = strtok(NULL, " \t");

            if (token == NULL) {
                fprintf(stderr,
                        "Syntax error: missing output file\n");
                return 0;
            }

           	cmd->output_file = strdup(token);
            cmd->append = 1;
        }

        else if (strcmp(token, "&") == 0) {

            token = strtok(NULL, " \t");

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

            cmd->args[i] = strdup(token);

			if (cmd->args[i] == NULL) {
    			perror("strdup failed");
    			free_command(cmd);
    			return 0;
			}
			i++;
        }

        token = strtok(NULL, " \t");
    }

    cmd->args[i] = NULL;
    cmd->arg_count = i;

    if (i > 0) {
        cmd->command = cmd->args[0];
    }


	if (cmd->command == NULL &&
    (cmd->input_file != NULL ||
     cmd->output_file != NULL)) {

    fprintf(stderr,
            "Syntax error: missing command\n");

    free_command(cmd);

    return 0;
	}
	return 1;
}