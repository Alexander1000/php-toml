dnl $Id$
dnl config.m4 for extension toml

PHP_ARG_ENABLE(toml, Whether to enable the TomlPHP extension, [ --enable-toml Enable TomlPHP])

if test "$PHP_TOML" != "no"; then
    dnl # --enable-toml -> check with-path

    CXX_FLAGS="-std=c++17"
    PHP_SUBST(STD_SHARED_LIBADD)
    PHP_REQUIRE_CXX()
    PHP_ADD_LIBRARY(stdc++, "", STD_SHARED_LIBADD)
    # AC_DEFINE(HAVE_IO_BUFFER,1,[ ])

    PHP_NEW_EXTENSION(toml, php_toml.c lexer.c parse_tokens.c, $ext_shared)
fi
