PHP_ARG_ENABLE(php_toml, Whether to enable the TomlPHP extension, [ --enable-toml-php Enable TomlPHP])

if test "$PHP_TOML" != "no"; then
    PHP_NEW_EXTENSION(php_toml, php_toml.c, $ext_shared)
fi
