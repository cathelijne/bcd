<?php
/**
 * All ACF Fields Block Template.
 * Displays all ACF fields for the current post.
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

// Get all ACF fields for the current post
$fields = get_fields($post_id);

if ( !empty($fields) ) {
    echo '<div class="acf-all-fields">';
    echo '<h3>ACF Fields</h3>';
    echo '<dl class="acf-fields-list">';

    foreach ( $fields as $field_name => $field_value ) {
        echo '<dt class="acf-field-name">' . esc_html(ucwords(str_replace('_', ' ', $field_name))) . '</dt>';
        echo '<dd class="acf-field-value">';

        // Handle different field types
        if ( is_array($field_value) ) {
            echo '<pre>' . esc_html(print_r($field_value, true)) . '</pre>';
        } elseif ( is_bool($field_value) ) {
            echo $field_value ? 'Yes' : 'No';
        } elseif ( empty($field_value) ) {
            echo '<em>Empty</em>';
        } else {
            echo esc_html($field_value);
        }

        echo '</dd>';
    }

    echo '</dl>';
    echo '</div>';
} else {
    if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
        echo '<p class="acf-no-fields"><em>No ACF fields defined for this post.</em></p>';
    }
}
