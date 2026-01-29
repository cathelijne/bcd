<?php
/**
 * Inkoop Info Block Template
 */

$post_id = $post_id ?: get_the_ID();
require_once get_stylesheet_directory() . '/includes/inkoop_info.inc';
echo render_inkoop_info($post_id);
