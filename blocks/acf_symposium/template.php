<?php
/**
 * Symposium Info Block Template
 */

$post_id = $post_id ?: get_the_ID();
require_once get_stylesheet_directory() . '/includes/symposium_info.inc';
echo render_symposium_info($post_id);
