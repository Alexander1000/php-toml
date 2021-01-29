# php-toml

## Build

```shell
phpize
./configure --enable-toml
make
make install
```

## Generate signature
```shell
php build/gen_stub.php
```

## How use it
test.php
```php
<?php

var_dump(parse_toml_file('test.toml'));
```

run script:
```shell
php -d extension=toml.so test.php
```
