<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

/**
 * Apply default logo if none is set
 *
 * @param string $html The custom logo HTML
 * @return string Modified HTML with default logo if needed
 */
function apply_default_logo( $html ) {
    if ( empty( $html ) ) {
        $html = '<img src="' . get_template_directory_uri() . '/assets/images/BCD_Logo.png" alt="' . get_bloginfo( 'name' ) . '">';
    }
    return $html;
}

add_filter( 'get_custom_logo', 'apply_default_logo' );
