// include the PHP API itself
#include <php.h>
// then include the header of your extension
#include "php_toml.h"

// register our function to the PHP API
// so that PHP knows, which functions are in this module
zend_function_entry toml_php_functions[] = {
    PHP_FE(parse_toml, NULL)
    PHP_FE(parse_toml_file, NULL)
    {NULL, NULL, NULL}
};

// some pieces of information about our module
zend_module_entry toml_php_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_TOML_EXTNAME,
    toml_php_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_TOML_VERSION,
    STANDARD_MODULE_PROPERTIES
};

// use a macro to output additional C code, to make ext dynamically loadable
ZEND_GET_MODULE(toml_php)

// Finally, we implement our "Hello World" function
// this function will be made available to PHP
// and prints to PHP stdout using printf
PHP_FUNCTION(parse_toml) {
    php_printf("Hello World! (from our extension)\n");
}

PHP_FUNCTION(parse_toml_file) {
    php_printf("Hello World! (from our extension)\n");
}
