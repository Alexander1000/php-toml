--TEST--
declare nested arrays
--FILE--
<?php

var_dump(parse_toml_file(__DIR__ . '/004.toml'));
?>
--EXPECT--
array(3) {
  ["name"]=>
  string(11) ""test toml""
  ["block"]=>
  array(3) {
    ["name"]=>
    string(15) ""nested object""
    ["value"]=>
    string(4) "8900"
    ["item2"]=>
    array(2) {
      ["name"]=>
      string(8) ""test01""
      ["enabled"]=>
      string(4) "true"
    }
  }
  ["blocks"]=>
  array(2) {
    [0]=>
    array(1) {
      ["name"]=>
      string(7) ""item1""
    }
    [1]=>
    array(1) {
      ["name"]=>
      string(7) ""item2""
    }
  }
}
