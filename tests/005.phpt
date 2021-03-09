--TEST--
nested array of tables
--FILE--
<?php

var_dump(parse_toml_file(__DIR__ . '/005.toml'));
?>
--EXPECT--
array(1) {
  ["fruits"]=>
  array(2) {
    [0]=>
    array(3) {
      ["name"]=>
      string(7) ""apple""
      ["physical"]=>
      array(2) {
        ["color"]=>
        string(5) ""red""
        ["shape"]=>
        string(7) ""round""
      }
      ["varieties"]=>
      array(2) {
        [0]=>
        array(1) {
          ["name"]=>
          string(15) ""red delicious""
        }
        [1]=>
        array(1) {
          ["name"]=>
          string(14) ""granny smith""
        }
      }
    }
    [1]=>
    array(2) {
      ["name"]=>
      string(8) ""banana""
      ["varieties"]=>
      array(1) {
        [0]=>
        array(1) {
          ["name"]=>
          string(10) ""plantain""
        }
      }
    }
  }
}
