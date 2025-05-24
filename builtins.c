#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

int fsh_cd(char **args);
int fsh_help(char **args);
int fsh_exit(char **args);
int fsh_pwd();

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

int fsh_pwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("current working directory: %s\n", cwd);
    } else {
        printf("error getting current directory");
    }

    return 1;
}