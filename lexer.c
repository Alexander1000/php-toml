#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

struct Token* parse_tokens(char* file_name)
{
    FILE* hFile = fopen(file_name, "r");
    char* buffer = (char*) malloc(sizeof(char) * 1024);
    memset(buffer, 0, sizeof(char) * 1024);
    size_t size = fread(buffer, sizeof(char), 1024, hFile);
    fclose(hFile);
    return (struct Token*) 0;
}
