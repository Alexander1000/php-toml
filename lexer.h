#ifndef TOML_LEXER_INCLUDED
#define TOML_LEXER_INCLUDED

struct Token {
    char* data;
    int type;
};

struct Token* parse_tokens(char* file_name);

#endif
