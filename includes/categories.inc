<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

/**
 * Create default categories if they don't exist
 */
function bestcare_create_default_categories() {
    $categories = [
        'bestcare' => [
            'name' => 'BestCare',
            'slug' => 'bestcare',
            'description' => 'BestCare intern'
        ],
        'public' => [
            'name' => 'Public',
            'slug' => 'public',
            'description' => 'Publieke informatie'
        ],
        'inkoop' => [
            'name' => 'Inkoop',
            'slug' => 'inkoop',
            'description' => 'Inkoop gerelateerd'
        ]
    ];

    foreach ($categories as $key => $cat_data) {
        // Check if category exists
        $term = term_exists($cat_data['slug'], 'category');

        if (!$term) {
            // Create the category
            $term_id = wp_insert_term(
                $cat_data['name'],
                'category',
                [
                    'slug' => $cat_data['slug'],
                    'description' => $cat_data['description']
                ]
            );

            if (!is_wp_error($term_id)) {
                error_log('Created category: ' . $cat_data['name']);
            }
        }
    }

    // Set bestcare as default category
    $bestcare_term = get_term_by('slug', 'bestcare', 'category');
    if ($bestcare_term) {
        update_option('default_category', $bestcare_term->term_id);
    }
}

// Run on theme activation
add_action('after_switch_theme', 'bestcare_create_default_categories');

// Also run on init if categories don't exist
add_action('init', function() {
    $bestcare_exists = term_exists('bestcare', 'category');
    $public_exists = term_exists('public', 'category');
    $inkoop_exists = term_exists('inkoop', 'category');

    if (!$bestcare_exists || !$public_exists || !$inkoop_exists) {
        bestcare_create_default_categories();
    }
}, 999);

/**
 * Ensure bestcare is the default category
 */
add_action('admin_init', function() {
    $default_category = get_option('default_category');
    $bestcare_term = get_term_by('slug', 'bestcare', 'category');

    if ($bestcare_term && $default_category != $bestcare_term->term_id) {
        update_option('default_category', $bestcare_term->term_id);
    }
});

/**
 * Enable categories for pages
 */
function bestcare_add_categories_to_pages() {
    register_taxonomy_for_object_type('category', 'page');
}
add_action('init', 'bestcare_add_categories_to_pages');

/**
 * Add category metabox to pages in classic editor
 */
function bestcare_add_page_category_metabox() {
    add_meta_box(
        'categorydiv',
        __('Categories'),
        'post_categories_meta_box',
        'page',
        'side',
        'default'
    );
}
add_action('add_meta_boxes', 'bestcare_add_page_category_metabox');
