<?php

namespace Theme;

$dir = get_stylesheet_directory() . '/includes/required-plugins/';
// exit if this dir doesn't exist
if (!is_dir($dir)) {
    return;
};
$subdirs = new \RecursiveIteratorIterator(
    new \RecursiveDirectoryIterator($dir),
    \RecursiveIteratorIterator::LEAVES_ONLY
);

foreach ($subdirs as $subdir) {
    if ($subdir->isDir()) {
        $index_file = $subdir->getPathname() . '/index.php';
        if (file_exists($index_file)) {
            require_once $index_file;
        }
    }
};
