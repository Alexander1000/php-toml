#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include <php.h>

#define BUFFER_SIZE 4096

struct List* parse_tokens(char* file_name)
{
    // initialize list tokens
    struct List* list = malloc(sizeof(struct List));
    memset(list, 0, sizeof(struct List));

    struct List* curElement = list;

    // initialize file descriptor
    FILE* hFile = fopen(file_name, "r");

    // initialize buffer
    char* buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    size_t size;

    do {
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        size = fread(buffer, sizeof(char), BUFFER_SIZE, hFile);
        for (int i = 0; i < size; i++) {
            // skip comments
            if (buffer[i] == '#') {
                do {
                    i++;
                } while (buffer[i] != '\n');
            }
            if (is_word(buffer[i])) {
                int startPost = i;
                do {
                    i++;
                } while(is_word(buffer[i]) || is_digit(buffer[i]));

                // initialize lexeme
                char* lexeme = (char*) malloc(sizeof(char) * (i - startPost));
                memset(lexeme, 0, sizeof(char) * (i - startPost));
                memcpy(lexeme, buffer + startPost, sizeof(i - startPost));

                struct Token* token = (struct Token*) malloc(sizeof(struct Token));
                memset(token, 0, sizeof(struct Token));
                token->data = lexeme;
                curElement->value = token;

                // initialize next element
                struct List* nextElement = (struct List*) malloc(sizeof(struct List));
                memset(nextElement, 0, sizeof(struct List));

                // make relation
                curElement->next = nextElement;
                nextElement->prev = curElement;

                curElement = nextElement;
            }
        }
        // php_printf("Content: %s\n", buffer);
    } while(size == BUFFER_SIZE);

    // clean up
    fclose(hFile);
    free(buffer);
    return list;
}

int is_word(char symbol)
{
    return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
}

int is_digit(char symbol)
{
    return symbol >= '0' && symbol <= '9';
}