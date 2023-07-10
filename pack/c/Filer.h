#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

void f_write_in(char *path, struct Information *info)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("[FILER - ERROR] : file not found at %s", path);
        exit(1);
    }
    fprintf(file, "%s", p_info_parse(info));
    fclose(file);
}

struct Information *f_read_from(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("[FILER - ERROR] : file not found at %s", path);
        exit(1);
    }
    char message[MAX_MESSAGE_LENGTH] = "";
    fscanf(file, "%s", message);
    fclose(file);
    return p_info_read(message);
}


void f_write_string(char *path, char *info)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("[FILER - ERROR] : file not found at %s", path);
        exit(1);
    }
    fprintf(file, "%s", info);
    fclose(file);
}
