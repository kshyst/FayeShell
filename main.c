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

char *fsh_read_line(void);
char **fsh_split_line(char *line);
int fsh_launch(char **args);
void fsh_loop(void);
int fsh_cd(char **args);
int fsh_help(char **args);
int fsh_pwd();
int fsh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "pwd",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &fsh_cd,
    &fsh_help,
    &fsh_pwd,
    &fsh_exit
};

int fsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

char *fsh_current_directory(){
    static char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    } else {
        return "error getting current directory";
    }
}


int main(int argc, char **argv)
{
    // Load config files, if any.

    // Run command loop.
    fsh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}

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

int fsh_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < fsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return fsh_launch(args);
}

void fsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        // char current_directory[PATH_MAX];
        // strcpy(fsh_current_directory, current_directory);

        printf("fsh > %s > " , fsh_current_directory());
        line = fsh_read_line();
        args = fsh_split_line(line);
        status = fsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int fsh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "fsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int fsh_help(char **args)
{
    int i;
    printf("KSHyst's FayeShell\n");

    for (i = 0; i < fsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
    }

    return 1;
}

int fsh_exit(char **args)
{
    return 0;
}

int fsh_pwd(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("current working directory: %s\n", cwd);
    } else {
        printf("error getting current directory");
    }

    return 1;
}