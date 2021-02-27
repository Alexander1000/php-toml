#include <php.h>
#include "lexer.h"
#include <memory.h>
#include "parse_tokens.h"

zval* parse_tokens(struct List** pCurToken)
{
    zval* return_value = (zval*) malloc(sizeof(zval));
    array_init(return_value);

    struct List* curToken = *pCurToken;

    while (curToken != 0 && curToken->value != 0) {
        struct Token* token = curToken->value;
        if (token == 0) {
            continue;
        }

        if (token->type == T_TOKEN_BRACE_OPEN) {
            break;
        }

        if (token->type == T_TOKEN_PARAMETER_NAME) {
            char *paramName = token->data;
            curToken = curToken->next;
            if (curToken == 0) {
                continue;
            }
            token = (struct Token *) curToken->value;
            if (token == 0) {
                continue;
            }
            if (token->type != T_TOKEN_EQUAL) {
                zend_error(E_WARNING, "Invalid toml-format");
            }
            curToken = curToken->next;
            if (curToken == 0) {
                continue;
            }
            token = (struct Token *) curToken->value;
            if (token == 0) {
                continue;
            }
            if (token->type == T_TOKEN_PARAMETER_VALUE) {
                add_assoc_string(return_value, paramName, token->data);
            }
        }

        curToken = curToken->next;
    }

    *pCurToken = curToken->prev;
    return return_value;
}

zval* parse_array(struct List** pCurToken)
{
    zval* return_value = (zval*) malloc(sizeof(zval));
    array_init(return_value);

    struct List* curToken = *pCurToken;

    while (curToken != 0 && curToken->value != 0) {
        struct Token* token = curToken->value;
        if (token == 0) {
            continue;
        }

        if (token->type == T_TOKEN_BRACE_OPEN) {
            break;
        }

        if (token->type == T_TOKEN_PARAMETER_NAME) {
            char *paramName = token->data;
            curToken = curToken->next;
            if (curToken == 0) {
                continue;
            }
            token = (struct Token *) curToken->value;
            if (token == 0) {
                continue;
            }
            if (token->type != T_TOKEN_EQUAL) {
                zend_error(E_WARNING, "Invalid toml-format");
            }
            curToken = curToken->next;
            if (curToken == 0) {
                continue;
            }
            token = (struct Token *) curToken->value;
            if (token == 0) {
                continue;
            }
            if (token->type == T_TOKEN_PARAMETER_VALUE) {
                add_assoc_string(return_value, paramName, token->data);
            }
        }

        curToken = curToken->next;
    }

    *pCurToken = curToken->prev;
    return return_value;
}
