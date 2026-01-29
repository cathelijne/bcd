<?php
/**
 * Lezing Info Block Template
 */

$post_id = $post_id ?: get_the_ID();
require_once get_stylesheet_directory() . '/includes/lezing_info.inc';
echo render_lezing_info($post_id);
