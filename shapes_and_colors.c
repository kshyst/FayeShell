#include <stdlib.h> 

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define FULL_BLOCK "\U00002588"
#define SMALL_BLOCK "\U00002584"
#define VERY_SMALL_BLOCK "\U00002582"
#define LOWER_RIGHT_TRIANGLE "\U00002599"
#define EMBLEM_OF_IRAN "\U0000262B"

char *fsh_command_line_start()
{
    size_t needed_size = strlen(COLOR_RED) + strlen(COLOR_RESET) + strlen(EMBLEM_OF_IRAN) + strlen(COLOR_CYAN) + strlen(FULL_BLOCK) + strlen(FULL_BLOCK) + strlen(LOWER_RIGHT_TRIANGLE) + strlen(SMALL_BLOCK) + strlen(SMALL_BLOCK) + strlen(VERY_SMALL_BLOCK) + strlen(VERY_SMALL_BLOCK) + strlen(COLOR_RESET) + 1;
    char *block = malloc(needed_size);

    if (block == NULL) {
        printf("fucking failed a simple allocation omg\n");
        return NULL;
    }

    strcpy(block, COLOR_CYAN);
    strcat(block, "FSH ");
    strcat(block, FULL_BLOCK);
    strcat(block, FULL_BLOCK);
    strcat(block, LOWER_RIGHT_TRIANGLE);
    strcat(block, SMALL_BLOCK);
    strcat(block, SMALL_BLOCK);
    strcat(block, VERY_SMALL_BLOCK);
    strcat(block, VERY_SMALL_BLOCK);
    strcat(block, COLOR_RESET);
    strcat(block, COLOR_RED);
    strcat(block, "  ");
    strcat(block, EMBLEM_OF_IRAN);
    strcat(block, COLOR_RESET);
    
    return block;
}