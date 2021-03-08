--TEST--
parse simple toml file
--FILE--
<?php

var_dump(parse_toml_file(__DIR__ . '/003.toml'));
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
    ["items"]=>
    array(3) {
      [0]=>
      array(1) {
        ["name"]=>
        string(8) ""test01""
      }
      [1]=>
      array(1) {
        ["name"]=>
        string(8) ""test02""
      }
      [2]=>
      array(2) {
        ["name"]=>
        string(8) ""test03""
        ["enabled"]=>
        string(4) "true"
      }
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
