<?php
/**
 * BestCare Theme Functions
 *
 * Loads all include files from the includes directory
 */

// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

$files = new \FilesystemIterator( __DIR__.'/includes', \FilesystemIterator::SKIP_DOTS );
foreach ( $files as $file )
{
    /** @noinspection PhpIncludeInspection */
    ! $files->isDir() and include $files->getRealPath();
}

// Source - https://stackoverflow.com/a
// Posted by Jared, modified by community. See post 'Timeline' for change history
// Retrieved 2026-01-23, License - CC BY-SA 3.0

add_action( 'wp_enqueue_scripts', 'my_theme_enqueue_styles', 11 );
function my_theme_enqueue_styles() {
    wp_enqueue_style( 'child-style', get_stylesheet_uri() );
}
