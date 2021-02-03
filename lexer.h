#ifndef TOML_LEXER_INCLUDED
#define TOML_LEXER_INCLUDED

#define T_TOKEN_PARAMETER_NAME 1
#define T_TOKEN_EQUAL 2
#define T_TOKEN_PLAIN_VALUE 3
#define T_TOKEN_BRACE_OPEN 4
#define T_TOKEN_BRACE_CLOSE 4

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
