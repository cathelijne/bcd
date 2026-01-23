<?php
/**
 * Collega List Block Template.
 *
 * @param array  $block The block settings and attributes.
 * @param string $content The block inner HTML (empty).
 * @param bool   $is_preview True during backend preview render.
 * @param int    $post_id The post ID the block is rendering content against.
 * @param array $context The context provided to the block by the post or its parent block.
 */

if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
    error_log( "Loading" . __FILE__ );
}

// Render complete list of colleagues
render_collega_list();
