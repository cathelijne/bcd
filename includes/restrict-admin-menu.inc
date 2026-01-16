<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) {
    exit;
}

function removeFromAdminMenu(){
    // Remove specific admin menu pages for non-administrator users
    if( !current_user_can( 'administrator' ) ){
        remove_menu_page( 'edit.php?post_type=page' );
        remove_menu_page( 'themes.php' );
        remove_menu_page( 'plugins.php' );
    }
    // Always remove the Comments menu page
    remove_menu_page( 'edit-comments.php' );
}

// Disable menus for non-admin users
add_action('admin_menu', 'removeFromAdminMenu');
