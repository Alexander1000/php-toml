--TEST--
toml file with complex attribute name
--FILE--
<?php

var_dump(parse_toml_file(__DIR__ . '/002.toml'));
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
    ["some"]=>
    array(1) {
      ["test"]=>
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
    array(2) {
      ["name"]=>
      string(7) ""item2""
      ["props"]=>
      array(2) {
        ["price"]=>
        string(4) "9090"
        ["description"]=>
        string(27) ""some description for item""
      }
    }
  }
}
