<?php

/**
 * Praktijk Block template.
 *
 * @param array  $block The block settings and attributes.
 * @param string $content The block inner HTML (empty).
 * @param bool   $is_preview True during backend preview render.
 * @param int    $post_id The post ID the block is rendering content against.
 *                     This is either the post ID currently being displayed inside a query loop,
 *                     or the post ID of the post hosting this block.
 * @param array $context The context provided to the block by the post or its parent block. */

if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
    error_log( "Loading" . __FILE__ );
    $praktijk_fields = get_fields($post_id);
    error_log("Post fields for post ID " . $post_id . ": " . print_r($praktijk_fields, true) );
    error_log("Rendering praktijk block for post ID: " . $post_id);
};

$praktijk_fields = get_fields();

// Render the praktijk information
render_praktijk_info($praktijk_fields);
