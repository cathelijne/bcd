<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {

	// add_action( 'loop_end', function() {
	// 	error_log( print_r( debug_backtrace(), true ) );
	// });

	// add_action('loop_end', function() {
	// 	$trace = debug_backtrace();
	// 	$file = isset($trace[1]['file']) ? $trace[1]['file'] : null;
	// 	$file_name = basename( $file );
	// 	error_log( $file_name );
	// 	$myPost = get_post();
	// 	if ( $myPost ) {
	// 		error_log( print_r( $myPost, true ) );
	// 	}
	// });

	// add_action('loop_end', function( $post_id ) {
	// 	$fields = get_fields();
	// 	error_log( print_r( $fields, true ) );
	// });
};
