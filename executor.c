#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shell.h"

void execute_command(Command *cmd, char *original_cmd) {

    if (cmd->command == NULL)
        return;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {

        execvp(cmd->command, cmd->args);

        perror("execvp failed");
        exit(127);
    }

    else {

        int status;
        waitpid(pid, &status, 0);
    }
}

void cleanup_background_jobs() {
}s