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


#endif //CMSC125_LAB1_QUINDAOONGA_SHELL_H