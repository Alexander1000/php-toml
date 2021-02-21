#include <php.h>
#include "lexer.h"
#include <memory.h>

zval* parse_array(struct List* curToken)
{
    zval* return_value = (zval*) malloc(sizeof(zval));
    array_init(return_value);

    int mode = S_PLAIN_MODE;

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
                php_printf("Setup Key: %s and Value: %s\n", paramName, token->data);
            }
        }

        curToken = curToken->next;
    }

    return return_value;
}
