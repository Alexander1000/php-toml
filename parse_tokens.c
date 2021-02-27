#include <php.h>
#include "lexer.h"
#include <memory.h>
#include "parse_tokens.h"

zval* parse_tokens(struct List** pCurToken)
{
    zval* return_value = parse_array(pCurToken);
    struct List* curToken = *pCurToken;

    while (curToken != 0 && curToken->value != 0) {
        struct Token* token = curToken->value;
        if (token == 0) {
            continue;
        }

        if (token->type == T_TOKEN_BRACE_OPEN) {
            curToken = curToken->next;
            if (curToken == 0) {
                zend_error(E_WARNING, "Invalid toml-format");
            }
            token = (struct Token*) curToken->value;
            if (token == 0) {
                zend_error(E_WARNING, "Invalid toml-format");
            }
            if (token->type == T_TOKEN_PARAMETER_NAME) {
                char *paramName = token->data;
                curToken = curToken->next;
                if (curToken == 0) {
                    zend_error(E_WARNING, "Invalid toml-format");
                }

                token = (struct Token *) curToken->value;
                if (token == 0) {
                    zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                }

                if (token->type != T_TOKEN_BRACE_CLOSE) {
                    zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                }

                curToken = curToken->next;

                zval* array = parse_array(&curToken);
                add_assoc_zval(return_value, paramName, array);
                continue;
            }
            if (token->type == T_TOKEN_BRACE_OPEN) {
                curToken = curToken->next;
                if (curToken == 0) {
                    zend_error(E_WARNING, "Invalid toml-format");
                }
                token = (struct Token*) curToken->value;
                if (token == 0) {
                    zend_error(E_WARNING, "Invalid toml-format");
                }
                if (token->type == T_TOKEN_PARAMETER_NAME) {
                    char *paramName = token->data;
                    curToken = curToken->next;
                    if (curToken == 0) {
                        zend_error(E_WARNING, "Invalid toml-format");
                    }

                    token = (struct Token *) curToken->value;
                    if (token == 0) {
                        zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                    }

                    if (token->type != T_TOKEN_BRACE_CLOSE) {
                        zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                    }

                    curToken = curToken->next;

                    if (curToken == 0) {
                        zend_error(E_WARNING, "Invalid toml-format");
                    }

                    token = (struct Token *) curToken->value;
                    if (token == 0) {
                        zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                    }

                    if (token->type != T_TOKEN_BRACE_CLOSE) {
                        zend_error(E_WARNING, "Invalid toml-format (expected close brace)");
                    }

                    zval* array = parse_array(&curToken);

                    HashTable* ftable = HASH_OF(return_value);
                    zend_string* sParamName = zend_string_alloc(strlen(paramName) + 1, 0);
                    memset(sParamName->val, 0, sizeof(char) * (strlen(paramName) + 1));
                    memcpy(sParamName->val, paramName, sizeof(char) * strlen(paramName));
                    sParamName->len = strlen(paramName);

                    zval* nestedArray = zend_hash_find(ftable, sParamName);

                    if (nestedArray == 0) {
                        nestedArray = (zval*) malloc(sizeof(zval));
                        array_init(nestedArray);
                        add_assoc_zval(return_value, paramName, nestedArray);
                    }

                    add_next_index_zval(nestedArray, array);

                    mode = S_PLAIN_MODE;

                    zend_string_release(sParamName);
                    continue;
                }
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
