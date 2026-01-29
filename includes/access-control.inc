<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

/**
 * BestCare Access Control
 * Controls what content is visible to logged-in vs non-logged-in users
 */

/**
 * Get allowed category IDs based on user login status
 */
function bestcare_get_allowed_categories() {
    if (is_user_logged_in()) {
        // Logged in users can see bestcare, public, and inkoop
        $categories = ['bestcare', 'public', 'inkoop'];
    } else {
        // Non-logged-in users can only see public
        $categories = ['public'];
    }

    $category_ids = [];
    foreach ($categories as $slug) {
        $term = get_term_by('slug', $slug, 'category');
        if ($term) {
            $category_ids[] = $term->term_id;
        }
    }

    return $category_ids;
}

/**
 * Filter main query to only show allowed categories
 */
function bestcare_filter_query_by_category($query) {
    // Don't filter admin queries
    if (is_admin()) {
        return;
    }

    // Only filter main query
    if (!$query->is_main_query()) {
        return;
    }

    // Get allowed categories
    $allowed_categories = bestcare_get_allowed_categories();

    if (empty($allowed_categories)) {
        return;
    }

    // Get existing tax_query
    $tax_query = $query->get('tax_query');
    if (!is_array($tax_query)) {
        $tax_query = [];
    }

    // Add category filter
    $tax_query[] = [
        'taxonomy' => 'category',
        'field' => 'term_id',
        'terms' => $allowed_categories,
        'operator' => 'IN'
    ];

    $query->set('tax_query', $tax_query);
}
add_action('pre_get_posts', 'bestcare_filter_query_by_category');

/**
 * Filter search results
 */
function bestcare_filter_search_results($query) {
    if (!is_admin() && $query->is_search() && $query->is_main_query()) {
        $allowed_categories = bestcare_get_allowed_categories();

        if (!empty($allowed_categories)) {
            $tax_query = $query->get('tax_query');
            if (!is_array($tax_query)) {
                $tax_query = [];
            }

            $tax_query[] = [
                'taxonomy' => 'category',
                'field' => 'term_id',
                'terms' => $allowed_categories,
                'operator' => 'IN'
            ];

            $query->set('tax_query', $tax_query);
        }
    }
}
add_action('pre_get_posts', 'bestcare_filter_search_results');

/**
 * Filter navigation menus to hide restricted items
 */
function bestcare_filter_nav_menu_items($items, $args) {
    if (is_admin()) {
        return $items;
    }

    $allowed_categories = bestcare_get_allowed_categories();

    if (empty($allowed_categories)) {
        return $items;
    }

    foreach ($items as $key => $item) {
        // Only check menu items that link to posts
        if ($item->object == 'post' || $item->object == 'page' || in_array($item->object, get_post_types(['_builtin' => false]))) {
            $post_id = $item->object_id;

            // Get categories for this post
            $post_categories = wp_get_post_categories($post_id);

            // Check if post has any allowed categories
            $has_allowed_category = false;
            foreach ($post_categories as $cat_id) {
                if (in_array($cat_id, $allowed_categories)) {
                    $has_allowed_category = true;
                    break;
                }
            }

            // Remove item if it doesn't have an allowed category
            if (!$has_allowed_category && !empty($post_categories)) {
                unset($items[$key]);
            }
        }
    }

    return $items;
}
add_filter('wp_nav_menu_objects', 'bestcare_filter_nav_menu_items', 10, 2);

/**
 * Redirect single posts that user shouldn't have access to
 */
function bestcare_restrict_single_post_access() {
    if (!is_singular()) {
        return;
    }

    // Allow logged-in users to see everything
    if (is_user_logged_in()) {
        return;
    }

    global $post;

    // Get post categories
    $post_categories = wp_get_post_categories($post->ID);

    if (empty($post_categories)) {
        return;
    }

    // Get allowed categories
    $allowed_categories = bestcare_get_allowed_categories();

    // Check if post has any allowed categories
    $has_allowed_category = false;
    foreach ($post_categories as $cat_id) {
        if (in_array($cat_id, $allowed_categories)) {
            $has_allowed_category = true;
            break;
        }
    }

    // Redirect to home if no access
    if (!$has_allowed_category) {
        wp_redirect(home_url());
        exit;
    }
}
add_action('template_redirect', 'bestcare_restrict_single_post_access');

/**
 * Check if current user can see a specific block
 * Use this in block templates to conditionally render blocks
 *
 * @param string $block_visibility 'public', 'logged_in', or 'all'
 * @return bool
 */
function bestcare_can_see_block($block_visibility = 'all') {
    switch ($block_visibility) {
        case 'public':
            return !is_user_logged_in();
        case 'logged_in':
            return is_user_logged_in();
        case 'all':
        default:
            return true;
    }
}

/**
 * Filter blocks based on CSS classes for visibility
 */
function bestcare_filter_blocks_by_class($block_content, $block) {
    if (is_admin()) {
        return $block_content;
    }

    // Check for visibility classes
    if (isset($block['attrs']['className'])) {
        $classes = $block['attrs']['className'];
        $is_logged_in = is_user_logged_in();

        // Remove content for non-logged-in users
        if (!$is_logged_in && (
            strpos($classes, 'hide-for-public') !== false ||
            strpos($classes, 'show-for-logged-in') !== false
        )) {
            return '';
        }

        // Remove content for logged-in users
        if ($is_logged_in && (
            strpos($classes, 'hide-for-logged-in') !== false ||
            strpos($classes, 'show-for-public') !== false
        )) {
            return '';
        }
    }

    return $block_content;
}
add_filter('render_block', 'bestcare_filter_blocks_by_class', 10, 2);

/**
 * Add body class to identify login status
 */
function bestcare_body_class($classes) {
    if (is_user_logged_in()) {
        $classes[] = 'user-logged-in';
    } else {
        $classes[] = 'user-not-logged-in';
    }
    return $classes;
}
add_filter('body_class', 'bestcare_body_class');

/**
 * Add custom ACF location rules for user login status
 */
function bestcare_acf_location_rules_types($choices) {
    $choices['BestCare']['bestcare_user_status'] = 'User Login Status';
    $choices['BestCare']['bestcare_user_can_see'] = 'User Can See Content';
    return $choices;
}
add_filter('acf/location/rule_types', 'bestcare_acf_location_rules_types');

/**
 * Add values for custom ACF location rules
 */
function bestcare_acf_location_rules_values($choices, $rule) {
    if ($rule['param'] == 'bestcare_user_status') {
        $choices = [
            'logged_in' => 'Logged In',
            'logged_out' => 'Logged Out'
        ];
    }

    if ($rule['param'] == 'bestcare_user_can_see') {
        $choices = [
            'bestcare' => 'BestCare Content (logged in)',
            'public' => 'Public Content Only',
            'inkoop' => 'Inkoop Content (logged in)'
        ];
    }

    return $choices;
}
add_filter('acf/location/rule_values/bestcare_user_status', 'bestcare_acf_location_rules_values', 10, 2);
add_filter('acf/location/rule_values/bestcare_user_can_see', 'bestcare_acf_location_rules_values', 10, 2);

/**
 * Match custom ACF location rules
 */
function bestcare_acf_location_rules_match($match, $rule, $options) {
    if ($rule['param'] == 'bestcare_user_status') {
        $is_logged_in = is_user_logged_in();

        if ($rule['operator'] == '==') {
            $match = ($rule['value'] == 'logged_in' && $is_logged_in) ||
                     ($rule['value'] == 'logged_out' && !$is_logged_in);
        } elseif ($rule['operator'] == '!=') {
            $match = ($rule['value'] == 'logged_in' && !$is_logged_in) ||
                     ($rule['value'] == 'logged_out' && $is_logged_in);
        }
    }

    if ($rule['param'] == 'bestcare_user_can_see') {
        $user_can_see = true;

        // Get allowed categories for current user
        $allowed_categories = bestcare_get_allowed_categories();
        $term = get_term_by('slug', $rule['value'], 'category');

        if ($term) {
            if ($rule['operator'] == '==') {
                $user_can_see = in_array($term->term_id, $allowed_categories);
            } elseif ($rule['operator'] == '!=') {
                $user_can_see = !in_array($term->term_id, $allowed_categories);
            }
        }

        $match = $user_can_see;
    }

    return $match;
}
add_filter('acf/location/rule_match/bestcare_user_status', 'bestcare_acf_location_rules_match', 10, 3);
add_filter('acf/location/rule_match/bestcare_user_can_see', 'bestcare_acf_location_rules_match', 10, 3);

/**
 * Filter ACF blocks and field groups based on user permissions
 */
function bestcare_filter_acf_blocks($block_content, $block, $context) {
    if (is_admin()) {
        return $block_content;
    }

    // Check if block has custom visibility settings in attributes
    if (isset($block['attrs']['data']) && is_array($block['attrs']['data'])) {
        $data = $block['attrs']['data'];

        // Check for bestcare_visibility attribute
        if (isset($data['bestcare_visibility'])) {
            $visibility = $data['bestcare_visibility'];

            if ($visibility === 'logged_in' && !is_user_logged_in()) {
                return '';
            }
            if ($visibility === 'logged_out' && is_user_logged_in()) {
                return '';
            }
        }

        // Check for bestcare_category attribute
        if (isset($data['bestcare_category'])) {
            $required_category = $data['bestcare_category'];
            $allowed_categories = bestcare_get_allowed_categories();
            $term = get_term_by('slug', $required_category, 'category');

            if ($term && !in_array($term->term_id, $allowed_categories)) {
                return '';
            }
        }
    }

    return $block_content;
}
add_filter('render_block', 'bestcare_filter_acf_blocks', 10, 3);

/**
 * Filter REST API queries to respect category restrictions
 */
function bestcare_filter_rest_query($args, $request) {
    $allowed_categories = bestcare_get_allowed_categories();

    if (!empty($allowed_categories)) {
        $args['tax_query'] = [
            [
                'taxonomy' => 'category',
                'field' => 'term_id',
                'terms' => $allowed_categories,
                'operator' => 'IN'
            ]
        ];
    }

    return $args;
}
add_filter('rest_post_query', 'bestcare_filter_rest_query', 10, 2);
add_filter('rest_page_query', 'bestcare_filter_rest_query', 10, 2);

// Apply to all custom post types
foreach (get_post_types(['_builtin' => false]) as $post_type) {
    add_filter("rest_{$post_type}_query", 'bestcare_filter_rest_query', 10, 2);
}

// Apply to all custom post types
add_action('init', function() {
    foreach (get_post_types(['_builtin' => false]) as $post_type) {
        add_filter("rest_{$post_type}_query", 'bestcare_filter_rest_query', 10, 2);
    }
}, 999);
