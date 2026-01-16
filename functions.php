<?php

namespace Theme;

$files = new \FilesystemIterator( __DIR__.'/includes', \FilesystemIterator::SKIP_DOTS );
foreach ( $files as $file )
{
    /** @noinspection PhpIncludeInspection */
    ! $files->isDir() and include $files->getRealPath();
}
