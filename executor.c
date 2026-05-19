#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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

        /*
         * INPUT REDIRECTION
         */
        if (cmd->input_file != NULL) {

            int fd = open(cmd->input_file, O_RDONLY);

            if (fd < 0) {
                perror("open input file");
                exit(1);
            }

            dup2(fd, STDIN_FILENO);

            close(fd);
        }

        /*
         * OUTPUT REDIRECTION
         */
        if (cmd->output_file != NULL) {

            int flags = O_WRONLY | O_CREAT;

            if (cmd->append)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;

            int fd = open(cmd->output_file,
                          flags,
                          0644);

            if (fd < 0) {
                perror("open output file");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);

            close(fd);
        }

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
}