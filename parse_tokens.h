#ifndef TOML_PARSE_TOKENS_INCLUDED
#define TOML_PARSE_TOKENS_INCLUDED

#include <php.h>
#include "lexer.h"

zval* parse_array(struct List** pCurToken);
zval* parse_tokens(struct List** pCurToken);

#endif
