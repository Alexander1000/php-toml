#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include <php.h>

#define BUFFER_SIZE 4096

struct List* get_tokens(char* file_name)
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

    int lexerMode = L_MODE_ROOT;

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

            switch (lexerMode) {
                case L_MODE_ROOT: {
                    if (is_word(buffer[i])) {
                        int startPost = i;
                        do {
                            i++;
                        } while(is_word(buffer[i]) || is_digit(buffer[i]));

                        // initialize lexeme
                        char* lexeme = (char*) malloc(sizeof(char) * (i - startPost + 1));
                        memset(lexeme, 0, sizeof(char) * (i - startPost + 1));
                        memcpy(lexeme, buffer + startPost, sizeof(char) * (i - startPost));

                        // initialize token
                        struct Token* token = (struct Token*) malloc(sizeof(struct Token));
                        memset(token, 0, sizeof(struct Token));
                        token->data = lexeme;
                        token->type = T_TOKEN_PARAMETER_NAME;

                        // setup token in value of list
                        curElement->value = token;

                        // initialize next element
                        struct List* nextElement = (struct List*) malloc(sizeof(struct List));
                        memset(nextElement, 0, sizeof(struct List));

                        // make relation
                        curElement->next = nextElement;
                        nextElement->prev = curElement;

                        curElement = nextElement;
                        i--;
                        continue;
                    }
                    if (buffer[i] == '=') {
                        // initialize token
                        struct Token* token = (struct Token*) malloc(sizeof(struct Token));
                        memset(token, 0, sizeof(struct Token));
                        token->type = T_TOKEN_EQUAL;

                        // setup token in value of list
                        curElement->value = token;

                        // initialize next element
                        struct List* nextElement = (struct List*) malloc(sizeof(struct List));
                        memset(nextElement, 0, sizeof(struct List));

                        // make relation
                        curElement->next = nextElement;
                        nextElement->prev = curElement;

                        curElement = nextElement;

                        lexerMode = L_MODE_SCAN_VALUE;
                        continue;
                    }
                    if (buffer[i] == '[' || buffer[i] == ']') {
                        // initialize token
                        struct Token* token = (struct Token*) malloc(sizeof(struct Token));
                        memset(token, 0, sizeof(struct Token));
                        if (buffer[i] == '[') {
                            token->type = T_TOKEN_BRACE_OPEN;
                        } else {
                            token->type = T_TOKEN_BRACE_CLOSE;
                        }

                        // setup token in value of list
                        curElement->value = token;

                        // initialize next element
                        struct List* nextElement = (struct List*) malloc(sizeof(struct List));
                        memset(nextElement, 0, sizeof(struct List));

                        // make relation
                        curElement->next = nextElement;
                        nextElement->prev = curElement;

                        curElement = nextElement;
                        continue;
                    }
                    break;
                }
                case L_MODE_SCAN_VALUE: {
                    // skip spaces
                    if (buffer[i] == 0x20 || buffer[i] == 0x0A) {
                        do {
                            i++;
                        } while(i < size && (buffer[i] == 0x20 || buffer[i] == 0x0A));
                    }

                    int startPost = i;
                    do {
                        i++;
                    } while(buffer[i] != 0x0A && buffer[i] != 0x0D);

                    // initialize lexeme
                    char* lexeme = (char*) malloc(sizeof(char) * (i - startPost));
                    memset(lexeme, 0, sizeof(char) * (i - startPost + 1));
                    memcpy(lexeme, buffer + startPost, sizeof(char) * (i - startPost));

                    // initialize token
                    struct Token* token = (struct Token*) malloc(sizeof(struct Token));
                    memset(token, 0, sizeof(struct Token));
                    token->data = lexeme;
                    token->type = T_TOKEN_PARAMETER_VALUE;

                    // setup token in value of list
                    curElement->value = token;

                    // initialize next element
                    struct List* nextElement = (struct List*) malloc(sizeof(struct List));
                    memset(nextElement, 0, sizeof(struct List));

                    // make relation
                    curElement->next = nextElement;
                    nextElement->prev = curElement;

                    curElement = nextElement;

                    lexerMode = L_MODE_ROOT;
                    break;
                }
            };
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