<?php

/**
 * Praktijk Logotemplate.
 *
 * @param array  $block The block settings and attributes.
 * @param string $content The block inner HTML (empty).
 * @param bool   $is_preview True during backend preview render.
 * @param int    $post_id The post ID the block is rendering content against.
 *                     This is either the post ID currently being displayed inside a query loop,
 *                     or the post ID of the post hosting this block.
 * @param array $context The context provided to the block by the post or its parent block. */

// Get the praktijk_logo field
$praktijk_logo = get_field('praktijk_logo', $post_id);

if ($praktijk_logo) {
    $logo_url = $praktijk_logo['url'] ?? '';
    $logo_alt = $praktijk_logo['alt'] ?? get_the_title($post_id) . ' logo';
    $logo_width = $praktijk_logo['width'] ?? '';
    $logo_height = $praktijk_logo['height'] ?? '';

    if ($logo_url) {
        echo '<div class="praktijk-logo">';
        echo '<img src="' . esc_url($logo_url) . '" ';
        echo 'alt="' . esc_attr($logo_alt) . '" ';
        if ($logo_width) {
            echo 'width="' . esc_attr($logo_width) . '" ';
        }
        if ($logo_height) {
            echo 'height="' . esc_attr($logo_height) . '" ';
        }
        echo 'class="praktijk-logo-image" />';
        echo '</div>';
    }
}
?>
