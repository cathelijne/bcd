<?php

$myName=$_SERVER['SERVER_NAME'];

// log hostname to console if debugging is on
if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
    error_log( 'Hostname for update checker: ' . $myName );
};

// Require the update checker library first
require_once get_stylesheet_directory() . '/includes/theme-update-checker/plugin-update-checker.php';
use YahnisElsts\PluginUpdateChecker\v5\PucFactory;

// Set up plugin update checker, according to hostname
if ( strpos( $myName, 'staging' ) !== false ) {
    // Staging - use staging branch
    $myUpdateChecker = PucFactory::buildUpdateChecker(
        'https://github.com/cathelijne/bcd/',
        'wp-content/themes/BestCare' . '/style.css',
        'BestCare'
    );
    $myUpdateChecker->setBranch('staging');

} elseif ( $myName == 'bestcaredierenartsen.nl' || $myName == 'www.bestcaredierenartsen.nl' ) {
    $myUpdateChecker = PucFactory::buildUpdateChecker(
        'https://github.com/cathelijne/bcd/',
        get_stylesheet_directory() . '/style.css',
        'BestCare'
    );
    //Set the branch that contains the stable release.
    $myUpdateChecker->setBranch('stable');


} else  {
    // Local development - do not update, we develop the thing here...
    // No update checker needed, exit silently
    return;
}
