PHP_ARG_ENABLE(php_toml, Whether to enable the TomlPHP extension, [ --enable-toml Enable TomlPHP])

if test "$PHP_TOML" != "no"; then
    PHP_NEW_EXTENSION(toml, php_toml.c, $ext_shared)
fi
