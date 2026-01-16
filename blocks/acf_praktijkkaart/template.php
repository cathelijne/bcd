<?php

/**
 * Praktijkkaart Block template.
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
    error_log("Rendering praktijkkaart block for post ID: " . $post_id);
};

// Enqueue required scripts and styles
wp_enqueue_script( 'acf-osm-frontend' );
wp_enqueue_style( 'leaflet' );

$praktijk_fields = get_fields();

// Render the map
render_praktijk_map($praktijk_fields);

wp_enqueue_script(
	'acf-osm-autofit',
	get_stylesheet_directory_uri() . '/assets/javascript/acf_osm_autofit.js',
	array('acf-osm-frontend'),
	filemtime( get_stylesheet_directory() . '/assets/javascript/acf_osm_autofit.js' ),
	true
);
