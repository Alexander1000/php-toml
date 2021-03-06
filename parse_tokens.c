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
            if (token->type == T_TOKEN_COMPLEX_PARAMETER_NAME) {
                char *paramName = token->data;

                struct List* parts = get_array_path_parts(paramName);

                zval* relativeArray = return_value;
                int isLastElement = 0;

                while (parts != 0 && parts->value != 0) {
                    if (parts->next == 0) {
                        isLastElement = 1;
                    }

                    if (isLastElement == 0) {
                        char *partName = (char *) parts->value;
                        int length = strlen(partName);
                        HashTable *hashTable = HASH_OF(relativeArray);
                        zend_string *sParamName = zend_string_alloc(length + 1, 0);
                        memset(sParamName->val, 0, sizeof(char) * (length + 1));
                        memcpy(sParamName->val, paramName, sizeof(char) * length);
                        sParamName->len = length;

                        zval *nestedArray = zend_hash_find(hashTable, sParamName);

                        if (nestedArray == 0) {
                            nestedArray = (zval *) malloc(sizeof(zval));
                            array_init(nestedArray);
                            add_assoc_zval(relativeArray, partName, nestedArray);
                        }

                        hashTable = HASH_OF(nestedArray);

                        zval *prop;
                        zend_string *key;
                        zend_long h;

                        int indexed = -1;
                        zval* lastElement = 0;

                        ZEND_HASH_FOREACH_KEY_VAL(hashTable, h, key, prop) {
                            if (key) {
                                // key-value array
                                indexed = 0;
                            } else {
                                // indexed array
                                if (indexed == -1) {
                                    indexed = 1;
                                }

                                lastElement = prop;
                            }
                        } ZEND_HASH_FOREACH_END();

                        if (indexed == 1 && lastElement != 0) {
                            nestedArray = lastElement;
                        }

                        relativeArray = nestedArray;
                    } else {
                        paramName = (char*) parts->value;
                    }

                    parts = parts->next;
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

                curToken = curToken->next;

                zval* array = parse_array(&curToken);
                add_assoc_zval(relativeArray, paramName, array);

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

                    zend_string_release(sParamName);
                    continue;
                }

                if (token->type == T_TOKEN_COMPLEX_PARAMETER_NAME) {
                    char *paramName = token->data;

                    struct List* parts = get_array_path_parts(paramName);

                    zval* relativeArray = return_value;
                    int isLastElement = 0;

                    while (parts != 0 && parts->value != 0) {
                        if (parts->next == 0) {
                            isLastElement = 1;
                        }

                        if (isLastElement == 0) {
                            HashTable *hashTable = HASH_OF(relativeArray);
                            char *partName = (char *) parts->value;
                            int length = strlen(partName);
                            zend_string *sParamName = zend_string_alloc(length + 1, 0);
                            memset(sParamName->val, 0, sizeof(char) * (length + 1));
                            memcpy(sParamName->val, paramName, sizeof(char) * length);
                            sParamName->len = length;

                            zval *nestedArray = zend_hash_find(hashTable, sParamName);

                            if (nestedArray == 0) {
                                nestedArray = (zval *) malloc(sizeof(zval));
                                array_init(nestedArray);
                                add_assoc_zval(relativeArray, partName, nestedArray);
                            }

                            // check nested array
                            hashTable = HASH_OF(nestedArray);

                            zval *prop;
                            zend_string *key;
                            zend_long h;

                            int indexed = -1;
                            zval* lastElement = 0;

                            ZEND_HASH_FOREACH_KEY_VAL(hashTable, h, key, prop) {
                                if (key) {
                                    // key-value array
                                    indexed = 0;
                                } else {
                                    // indexed array
                                    if (indexed == -1) {
                                        indexed = 1;
                                    }

                                    lastElement = prop;
                                }
                            } ZEND_HASH_FOREACH_END();

                            if (indexed == 1 && lastElement != 0) {
                                nestedArray = lastElement;
                            }

                            relativeArray = nestedArray;
                        } else {
                            paramName = (char*) parts->value;
                        }

                        parts = parts->next;
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

                    HashTable* ftable = HASH_OF(relativeArray);
                    zend_string* sParamName = zend_string_alloc(strlen(paramName) + 1, 0);
                    memset(sParamName->val, 0, sizeof(char) * (strlen(paramName) + 1));
                    memcpy(sParamName->val, paramName, sizeof(char) * strlen(paramName));
                    sParamName->len = strlen(paramName);

                    zval* nestedArray = zend_hash_find(ftable, sParamName);

                    if (nestedArray == 0) {
                        nestedArray = (zval*) malloc(sizeof(zval));
                        array_init(nestedArray);
                        add_assoc_zval(relativeArray, paramName, nestedArray);
                    }

                    add_next_index_zval(nestedArray, array);

                    zend_string_release(sParamName);

                    continue;
                }
            }
        }

        curToken = curToken->next;
    }

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
            if (token->type == T_TOKEN_COMPLEX_PARAMETER_NAME) {
                struct List* parts = get_array_path_parts(paramName);
            }
        }

        if (token->type == T_TOKEN_COMPLEX_PARAMETER_NAME) {
            char *paramName = token->data;

            struct List* parts = get_array_path_parts(paramName);

            zval* relativeArray = return_value;
            int isLastElement = 0;

            while (parts != 0 && parts->value != 0) {
                if (parts->next == 0) {
                    isLastElement = 1;
                }

                if (isLastElement == 0) {
                    char *partName = (char *) parts->value;
                    int length = strlen(partName);
                    HashTable *hashTable = HASH_OF(relativeArray);
                    zend_string *sParamName = zend_string_alloc(length + 1, 0);
                    memset(sParamName->val, 0, sizeof(char) * (length + 1));
                    memcpy(sParamName->val, paramName, sizeof(char) * length);
                    sParamName->len = length;

                    zval *nestedArray = zend_hash_find(hashTable, sParamName);

                    if (nestedArray == 0) {
                        nestedArray = (zval *) malloc(sizeof(zval));
                        array_init(nestedArray);
                        add_assoc_zval(relativeArray, partName, nestedArray);
                    }

                    relativeArray = nestedArray;
                } else {
                    paramName = (char*) parts->value;
                }

                parts = parts->next;
            }

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
                add_assoc_string(relativeArray, paramName, token->data);
            }
        }

        curToken = curToken->next;
    }

    *pCurToken = curToken;
    return return_value;
}
