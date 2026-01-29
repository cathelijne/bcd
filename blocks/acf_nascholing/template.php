<?php
/**
 * Nascholing Info Block Template
 */

$post_id = $post_id ?: get_the_ID();
require_once get_stylesheet_directory() . '/includes/nascholing_info.inc';
echo render_nascholing_info($post_id);
