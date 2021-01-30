PHP_ARG_ENABLE(toml, Whether to enable the TomlPHP extension, [ --enable-toml Enable TomlPHP])

if test "$PHP_TOML" != "no"; then
    dnl # --enable-toml -> check with-path
    AC_MSG_CHECKING(for libio-buffer in default path)
    for i in /usr/local /usr;
    do
        if test -r $i/lib/libio-buffer.a;
        then
            IO_BUFFER_DIR=$i
            AC_MSG_RESULT(libio-buffer.a found in $i)
        fi
    done

    if test -z "$IO_BUFFER_DIR";
    then
        AC_MSG_RESULT(not found)
        AC_MSG_ERROR(Please reinstall the io-buffer distribution - io-buffer.h should be /include and libio-buffer.a should be in /lib)
    fi

    PHP_ADD_INCLUDE($IO_BUFFER_DIR/include)
    PHP_SUBST(IO_BUFFER_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(io-buffer, $IO_BUFFER_DIR/lib, IO_BUFFER_SHARED_LIBADD)
    AC_DEFINE(HAVE_IO_BUFFER,1,[ ])

    PHP_NEW_EXTENSION(toml, php_toml.c, $ext_shared)
fi
