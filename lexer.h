#ifndef TOML_LEXER_INCLUDED
#define TOML_LEXER_INCLUDED

#define T_TOKEN_PARAMETER_NAME 1
#define T_TOKEN_EQUAL 2
#define T_TOKEN_PLAIN_VALUE 3
#define T_TOKEN_BRACE_OPEN 4
#define T_TOKEN_BRACE_CLOSE 5
#define T_TOKEN_PARAMETER_VALUE 6
#define T_TOKEN_COMPLEX_PARAMETER_NAME 7

// lexer mode
#define L_MODE_ROOT 0
#define L_MODE_SCAN_VALUE 1

struct Token {
    char* data;
    int type;
};

struct List {
    void* value;
    struct List* next;
    struct List* prev;
};

struct List* get_tokens(char* file_name);
int is_word(char symbol);
int is_digit(char symbol);

#endif
