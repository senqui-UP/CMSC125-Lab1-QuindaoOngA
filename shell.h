#ifndef CMSC125_LAB1_QUINDAOONGA_SHELL_H
#define CMSC125_LAB1_QUINDAOONGA_SHELL_H

#include <stdbool.h>

typedef struct {
    char *command;
    char *args[256];
    int   arg_count;      // add this
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

/* parser */
int parse_command(char *line, Command *cmd);
void free_command(Command *cmd);

/* builtins */
int handle_builtin(Command *cmd);

/* executor */
void execute_command(Command *cmd, char *original_cmd);
void cleanup_background_jobs();

#endif //CMSC125_LAB1_QUINDAOONGA_SHELL_H