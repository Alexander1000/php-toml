#include <php.h>
#include "lexer.h"
#include <memory.h>

zval* parse_array(struct List* curToken)
{
    zval* array = (zval*) malloc(sizeof(zval));
    array_init(array);
    return array;
}
