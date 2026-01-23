<?php
/**
 * Praktijk Team Block Template.
 * Displays colleagues associated with the current praktijk.
 *
 * @param array  $block The block settings and attributes.
 * @param string $content The block inner HTML (empty).
 * @param bool   $is_preview True during backend preview render.
 * @param int    $post_id The post ID the block is rendering content against.
 *                     This is either the post ID currently being displayed inside a query loop,
 *                     or the post ID of the post hosting this block.
 * @param array $context The context provided to the block by the post or its parent block.
 */

if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
    error_log( "Loading" . __FILE__ );
}

// Get the praktijk post ID
$praktijk_id = $post_id ?? get_the_ID();

// Render the team list for this praktijk
render_praktijk_collega_list($praktijk_id);
