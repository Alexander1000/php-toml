<?php

$stderr = fopen('php://stderr', 'w');

foreach (glob(__DIR__ . '/tests/*.diff') as $file) {
    $fileInfo = new \SplFileInfo($file);
    $testCaseName = substr($fileInfo->getFilename(), 0, 3);
    fwrite($stderr, sprintf("Test case #%s failed\n", $testCaseName));
    fwrite($stderr, PHP_EOL);
    fwrite($stderr, "Expected output:\n");
    $expected = file_get_contents(__DIR__ . '/tests/' . $testCaseName . '.exp');
    fwrite($stderr, $expected . PHP_EOL);
    fwrite($stderr, PHP_EOL);
    fwrite($stderr, 'Actual output:' . PHP_EOL);
    $actual = file_get_contents(__DIR__ . '/tests/' . $testCaseName . '.out');
    fwrite($stderr, $actual . PHP_EOL);
    fwrite($stderr, PHP_EOL);
}
