#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#include "shell.h"

#define MAX_BG_JOBS 100

// array for background process IDs
pid_t bg_jobs[MAX_BG_JOBS];

// current number of background jobs
int bg_count = 0;

// job counter for display
static int job_id = 1;


// clean finished background jobs
void cleanup_background_jobs() {

    int status;

    // check all background jobs
    for (int i = 0; i < bg_count; i++) {

        pid_t result =
            waitpid(bg_jobs[i],
                    &status,
                    WNOHANG);

        // if process finished
        if (result > 0) {

            printf("[DONE] Background process %d finished\n",
                   bg_jobs[i]);

            // remove finished process from array
            for (int j = i;
                 j < bg_count - 1;
                 j++) {

                bg_jobs[j] =
                    bg_jobs[j + 1];
            }

            bg_count--;
            i--;
        }
    }
}


// execute command
void execute_command(Command *cmd,
                     char *original_cmd) {

    // ignore empty command
    if (cmd->command == NULL)
        return;

    // handle built-in commands
    if (handle_builtin(cmd))
        return;

    // create child process
    pid_t pid = fork();

    // fork failed
    if (pid < 0) {

        perror("fork failed");
        return;
    }

    // child process
    if (pid == 0) {

        // input redirection
        if (cmd->input_file != NULL) {

            // open input file
            int fd =
                open(cmd->input_file,
                     O_RDONLY);

            if (fd < 0) {
                perror("open input file");
                exit(1);
            }

            // redirect stdin
            if (dup2(fd,
                     STDIN_FILENO) < 0) {

                perror("dup2 input");

                close(fd);
                exit(1);
            }

            // close original file descriptor
            close(fd);
        }

        // output redirection
        if (cmd->output_file != NULL) {

            int flags =
                O_WRONLY | O_CREAT;

            // append or overwrite
            if (cmd->append) {
                flags |= O_APPEND;
            }

            else {
                flags |= O_TRUNC;
            }

            // open output file
            int fd =
                open(cmd->output_file,
                     flags,
                     0644);

            if (fd < 0) {
                perror("open output file");
                exit(1);
            }

            // redirect stdout
            if (dup2(fd,
                     STDOUT_FILENO) < 0) {

                perror("dup2 output");

                close(fd);
                exit(1);
            }

            // close original file descriptor
            close(fd);
        }

        // execute external command
        execvp(cmd->command,
               cmd->args);

        // execvp only returns if command fails
        fprintf(stderr,
                "mysh: command not found: %s\n",
                cmd->command);

        exit(127);
    }

    // parent process
    else {

        // foreground process
        if (!cmd->background) {

            int status;

            // wait for child process
            if (waitpid(pid,
                        &status,
                        0) < 0) {

                perror("waitpid");
            }

            // check exit status
            if (WIFEXITED(status)) {

                int exit_code =
                    WEXITSTATUS(status);

                // print non-zero exit codes
                if (exit_code != 0) {

                    printf("Command exited with code %d\n",
                           exit_code);
                }
            }
        }

        // background process
        else {

            // store background pid
            if (bg_count <
                MAX_BG_JOBS) {

                bg_jobs[bg_count++] =
                    pid;

                // copy original command
                char clean_cmd[1024];

                strncpy(clean_cmd,
                        original_cmd,
                        sizeof(clean_cmd) - 1);

                clean_cmd[
                    sizeof(clean_cmd) - 1
                ] = '\0';

                // remove '&'
                char *amp =
                    strrchr(clean_cmd,
                            '&');

                if (amp != NULL) {
                    *amp = '\0';
                }

                // remove trailing spaces
                int len =
                    strlen(clean_cmd);

                while (len > 0 &&
                       clean_cmd[len - 1]
                       == ' ') {

                    clean_cmd[len - 1]
                        = '\0';

                    len--;
                }

                // print background job info
                printf("[%d] Started background job: %s (PID: %d)\n",
                       job_id++,
                       clean_cmd,
                       pid);
            }

            // too many background jobs
            else {

                fprintf(stderr,
                        "Too many background jobs\n");
            }
        }
    }
}