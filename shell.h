#ifndef CMSC125_LAB1_QUINDAOONGA_SHELL_H
#define CMSC125_LAB1_QUINDAOONGA_SHELL_H

typedef struct {
    char *command;
    char *args[256];

    char *input_file;
    char *output_file;

    int append;
    int background;
} Command;

/* parser */
int parse_command(char *line, Command *cmd);

/* builtins */
int handle_builtin(Command *cmd);

/* executor */
void execute_command(Command *cmd, char *original_cmd);
void cleanup_background_jobs();

#endif //CMSC125_LAB1_QUINDAOONGA_SHELL_H