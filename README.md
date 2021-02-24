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

output:
```shell
array(3) {
  'name' =>
  string(11) ""test toml""
  'block' =>
  array(2) {
    'name' =>
    string(15) ""nested object""
    'value' =>
    string(4) "8900"
  }
  'blocks' =>
  array(2) {
    [0] =>
    array(1) {
      'name' =>
      string(7) ""item1""
    }
    [1] =>
    array(1) {
      'name' =>
      string(7) ""item2""
    }
  }
}
```
