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

                    if (token->type != T_TOKEN_BRACE_CLOSE) {
                        zend_error(E_WARNING, "Invalid toml-format");
                    }

                    curToken = curToken->next;

                    zval* array = parse_array(curToken);
                    add_assoc_zval(return_value, paramName, array);
                    curToken = curToken->prev;
                    mode = S_PLAIN_MODE;
                }

                break;
            }
        }

//        php_printf("Token: %d\n", token->type);
//        if (token->data != 0) {
//            php_printf("Value: %s\n", token->data);
//        }

        curToken = curToken->next;
    }

    // php_printf("Hello World! (from our extension): %s\n", filename);
}

PHP_MINIT_FUNCTION(toml)
{
    return SUCCESS;
}
