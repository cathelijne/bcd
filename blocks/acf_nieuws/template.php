<?php
/**
 * Nieuws Info Block Template
 */

$post_id = $post_id ?: get_the_ID();
require_once get_stylesheet_directory() . '/includes/nieuws_info.inc';
echo render_nieuws_info($post_id);
