<?php
if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
  error_log( "Loading" . __FILE__ );
};

// Query all praktijken and output their marker data
$query = new WP_Query(array(
  'post_type' => 'praktijken',
  'posts_per_page' => -1,
  'orderby' => 'title',
  'order' => 'DESC'
));

if (defined('WP_DEBUG') && WP_DEBUG) {
  error_log('Query found ' . $query->found_posts . ' praktijk posts');
}

if ($query->have_posts()) {
  while ($query->have_posts()) {
    $query->the_post();
    $praktijk_fields = get_fields();

    if (defined('WP_DEBUG') && WP_DEBUG) {
      error_log('Processing post: ' . get_the_title() . ' (ID: ' . get_the_ID() . ')');
      error_log('Has markers: ' . (!empty($praktijk_fields['praktijk_coordinaten']['markers']) ? 'yes' : 'no'));
    }

    output_praktijk_markers_data($praktijk_fields);
  }
  wp_reset_postdata();
} else {
  if (defined('WP_DEBUG') && WP_DEBUG) {
    error_log('No praktijk posts found');
  }
  echo '<!-- No praktijk posts found -->';
}

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
