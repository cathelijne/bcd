<?php
/**
 * Protocol Info Block Template
 *
 * @param array $block The block settings and attributes.
 * @param string $content The block inner HTML (empty).
 * @param bool $is_preview True during backend preview render.
 * @param int $post_id The post ID the block is rendering content against.
 */

// Load values
$post_id = $post_id ?: get_the_ID();

// Include the rendering function
require_once get_stylesheet_directory() . '/includes/protocol_info.inc';

// Render the protocol information
echo render_protocol_info($post_id);
