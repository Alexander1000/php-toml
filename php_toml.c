// include the PHP API itself
#include <php.h>
// then include the header of your extension
#include "php_toml.h"
#include "php_toml_arginfo.h"

#include "lexer.h"

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
        struct Token* token = curToken->value;
        php_printf("Token: %d\n", token->type);
        if (token->data != 0) {
            php_printf("Value: %s\n", token->data);
        }
        curToken = curToken->next;
    }

    php_printf("Hello World! (from our extension): %s\n", filename);
}

PHP_MINIT_FUNCTION(toml)
{
    return SUCCESS;
}
