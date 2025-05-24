#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include "builtins.c"
#include "programs.c"
#include "shapes_and_colors.c"

void fsh_loop(void);

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

    char *command_line_start;
    char *current_directory;

    do {
        command_line_start = fsh_command_line_start();
        current_directory = fsh_current_directory();
        
        printf("%s %s > " , command_line_start, current_directory);
        line = fsh_read_line();
        args = fsh_split_line(line);
        status = fsh_execute(args);
        
        free(line);
        free(args);
    } while (status);
}