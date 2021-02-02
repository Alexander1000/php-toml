#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define BUFFER_SIZE 1024

struct List* parse_tokens(char* file_name)
{
    // initialize list tokens
    struct List* list = malloc(sizeof(struct List));
    memset(list, 0, sizeof(struct List));

    // initialize file descriptor
    FILE* hFile = fopen(file_name, "r");

    // initialize buffer
    char* buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    size_t size;

    do {
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        size = fread(buffer, sizeof(char), BUFFER_SIZE, hFile);
    } while(size == BUFFER_SIZE);

    // clean up
    fclose(hFile);
    free(buffer);
    return list;
}
