<?php
if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
  error_log( "Loading" . __FILE__ );
};

render_combined_praktijk_map_container();


// Enqueue required scripts and styles
wp_enqueue_script( 'acf-osm-frontend' );
wp_enqueue_style( 'leaflet' );
wp_enqueue_script(
	'acf-osm-combined-praktijk-map',
	get_stylesheet_directory_uri() . '/assets/javascript/acf_osm_combined_praktijk_map.js',
	array('acf-osm-frontend'),
	filemtime( get_stylesheet_directory() . '/assets/javascript/acf_osm_combined_praktijk_map.js' ),
	true
);
wp_enqueue_script(
	'acf-osm-autofit',
	get_stylesheet_directory_uri() . '/assets/javascript/acf_osm_autofit.js',
	array('acf-osm-frontend'),
	filemtime( get_stylesheet_directory() . '/assets/javascript/acf_osm_autofit.js' ),
	true
);
