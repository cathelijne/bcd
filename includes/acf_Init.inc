<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

// By defining this, any changes to ACF field configurations
// will be saved as JSON files in the acf-json folder.
// This makes it easier to version control ACF field changes.
define( 'ACF_JSON_DIR', get_stylesheet_directory() . '/acf-json' );

// Function to register ACF blocks
// Blocks are stored in the blocks/ folder of the child theme,
// each block in its own subfolder.

add_action('init', function () {
    /**
     * We register our block's with WordPress's handy
     * register_block_type();
     *
     * @link https://developer.wordpress.org/reference/functions/register_block_type/
     */
    $blocks_dir = get_stylesheet_directory() . '/blocks';

    if ( file_exists( $blocks_dir ) ) {
        $block_folders = array_filter( glob( $blocks_dir . '/*' ), 'is_dir' );

        foreach ( $block_folders as $block_folder ) {
            register_block_type( $block_folder );
        }
    }
});

// Make sure ACF blocks use the real post_id and not the block id.
// ACF has a quirk where it uses the block ID as the post ID
// when loading fields inside a block. This filter checks if
// the post ID is a block ID, and if so, it returns null
// so that ACF falls back to using the global post ID.
add_filter( 'acf/pre_load_post_id', function ( $null, $post_id ) {
  if ( !$post_id AND is_string($null) AND substr( $null, 0, 6 ) == 'block_' ) {
    $null = null;
  }
  return $null;
}, 10, 2);

// Customize OpenStreetMap address format for ACF OSM field
add_filter('acf_osm_address_format', function($format) {
  return [
    'street'  => '{road} {house_number}',
    'city'    => '{city}',
  ];
});
