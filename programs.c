#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

#define ALLOCATION_ERROR "fsh: allocation error\n"
//Buffer size for readline function
#define FSH_RL_BUFSIZE 1024
//Buffer size for tokenizing the input and delmiteres for tokenizing
#define FSH_TOK_BUFSIZE 64
#define FSH_TOK_DELIM " \t\r\n\a"

char *fsh_read_line(void)
{
    int bufsize = FSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } 
        else {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize) {
            bufsize += FSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, ALLOCATION_ERROR);
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **fsh_split_line(char *line)
{
    int bufsize = FSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    token = strtok(line, FSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += FSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
            fprintf(stderr, ALLOCATION_ERROR);
            exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, FSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int fsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
        if (pid == 0) {
        // Child process
            if (execvp(args[0], args) == -1) {
                perror("fsh");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("fsh");
        } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
