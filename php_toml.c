// include the PHP API itself
#include <php.h>
// then include the header of your extension
#include "php_toml.h"
#include "php_toml_arginfo.h"

#include "lexer.h"
#include "parse_tokens.h"

// some pieces of information about our module
zend_module_entry toml_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_TOML_EXTNAME,
    ext_functions,
    PHP_MINIT(toml),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_TOML_VERSION,
    STANDARD_MODULE_PROPERTIES
};

// use a macro to output additional C code, to make ext dynamically loadable
ZEND_GET_MODULE(toml)

PHP_FUNCTION(parse_toml_file)
{
    size_t filename_size;
    char *filename;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &filename, &filename_size) == FAILURE) {
        RETURN_THROWS();
    }

    array_init(return_value);

    struct List* tokens = parse_tokens(filename);
    struct List* curToken = tokens;

    while (curToken != 0 && curToken->value != 0) {
        struct Token *token = curToken->value;
        if (token == 0) {
            continue;
        }
        php_printf("Token: %d\n", token->type);
        if (token->data != 0) {
            php_printf("Value: %s\n", token->data);
        }
        php_printf("=======\n");
        curToken = curToken->next;
    }

    curToken = tokens;
    int mode = S_PLAIN_MODE;

    while (curToken != 0 && curToken->value != 0) {
        struct Token* token = curToken->value;
        if (token == 0) {
            continue;
        }

        switch (mode) {
            case S_PLAIN_MODE: {
                if (token->type == T_TOKEN_BRACE_OPEN) {
                    struct List* forwardElement = curToken->next;
                    if (forwardElement != 0) {
                        struct Token* forwardToken = (struct Token*) forwardElement->value;
                        if (forwardToken != 0) {
                            if (forwardToken->type == T_TOKEN_BRACE_OPEN) {
                                mode = S_ARRAY_MODE;
                                break;
                            }
                        }
                    }
                    mode = S_OBJECT_MODE;
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

                break;
            }

            case S_OBJECT_MODE: {
                if (token->type == T_TOKEN_BRACE_CLOSE) {
                    mode = S_PLAIN_MODE;
                    break;
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
                    mode = S_PLAIN_MODE;
                }

                break;
            }

            case S_ARRAY_MODE: {
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
                }
                break;
            }
        }

        curToken = curToken->next;
    }
}

PHP_MINIT_FUNCTION(toml)
{
    return SUCCESS;
}
