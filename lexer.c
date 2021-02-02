#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

struct List* parse_tokens(char* file_name)
{
    struct List* list = malloc(sizeof(struct List));
    memset(list, 0, sizeof(struct List));
    FILE* hFile = fopen(file_name, "r");
    char* buffer = (char*) malloc(sizeof(char) * 1024);
    memset(buffer, 0, sizeof(char) * 1024);
    size_t size = fread(buffer, sizeof(char), 1024, hFile);
    fclose(hFile);
    return list;
}
