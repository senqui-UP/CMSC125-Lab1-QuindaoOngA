#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#include "shell.h"

#define MAX_BG_JOBS 100

typedef struct {
    pid_t pid;
    int   job_id;
} BgJob;

// array for background jobs
static BgJob bg_jobs[MAX_BG_JOBS];

// current number of background jobs
static int bg_count = 0;

// job counter for display
static int job_id = 1;


// reap finished background jobs and report them
void cleanup_background_jobs() {

    int status;

    for (int i = 0; i < bg_count; i++) {

        pid_t result = waitpid(bg_jobs[i].pid, &status, WNOHANG);

        if (result > 0) {

            int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

            printf("[%d] Background process %d finished (exit status %d)\n",
                   bg_jobs[i].job_id,
                   bg_jobs[i].pid,
                   exit_code);

            // compact the array
            for (int j = i; j < bg_count - 1; j++) {
                bg_jobs[j] = bg_jobs[j + 1];
            }

            bg_count--;
            i--;
        }
    }
}


// execute command
void execute_command(Command *cmd, char *original_cmd) {

    // ignore empty command
    if (cmd->command == NULL)
        return;

    // handle built-in commands
    if (handle_builtin(cmd))
        return;

    // reject background job if the table is full — before forking, so
    // there is no child left untracked that would become a zombie
    if (cmd->background && bg_count >= MAX_BG_JOBS) {
        fprintf(stderr,
                "mysh: too many background jobs (max %d)\n",
                MAX_BG_JOBS);
        return;
    }

    // create child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    // child process
    if (pid == 0) {

        // input redirection
        if (cmd->input_file != NULL) {

            int fd = open(cmd->input_file, O_RDONLY);

            if (fd < 0) {
                perror("open input file");
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2 input");
                close(fd);
                exit(1);
            }

            close(fd);
        }

        // output redirection
        if (cmd->output_file != NULL) {

            int flags = O_WRONLY | O_CREAT;
            flags |= cmd->append ? O_APPEND : O_TRUNC;

            int fd = open(cmd->output_file, flags, 0644);

            if (fd < 0) {
                perror("open output file");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2 output");
                close(fd);
                exit(1);
            }

            close(fd);
        }

        execvp(cmd->command, cmd->args);

        fprintf(stderr, "mysh: command not found: %s\n", cmd->command);
        exit(127);
    }

    // parent process
    else {

        if (!cmd->background) {

            int status;

            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid");
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Command exited with code %d\n", WEXITSTATUS(status));
            }

        } else {

            // register in unified job table
            bg_jobs[bg_count].pid    = pid;
            bg_jobs[bg_count].job_id = job_id;
            bg_count++;

            // strip trailing '&' and spaces for display
            char clean_cmd[1024];
            strncpy(clean_cmd, original_cmd, sizeof(clean_cmd) - 1);
            clean_cmd[sizeof(clean_cmd) - 1] = '\0';

            char *amp = strrchr(clean_cmd, '&');
            if (amp != NULL)
                *amp = '\0';

            int len = strlen(clean_cmd);
            while (len > 0 && clean_cmd[len - 1] == ' ')
                clean_cmd[--len] = '\0';

            printf("[%d] Started background job: %s (PID: %d)\n",
                   job_id++,
                   clean_cmd,
                   pid);
        }
    }
}