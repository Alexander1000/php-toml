#ifndef TOML_LEXER_INCLUDED
#define TOML_LEXER_INCLUDED

struct Token {
    char* data;
    int type;
};

struct List {
    void* value;
    struct List* next;
    struct List* prev;
};

struct List* parse_tokens(char* file_name);
int is_word(char symbol);
int is_digit(char symbol);

#endif