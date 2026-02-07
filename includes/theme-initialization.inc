<?php
/**
 * Theme Initialization
 *
 * Runs on theme activation to set up default pages and configuration
 */

// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

/**
 * Create default pages on theme activation
 */
function bestcare_create_default_pages() {
    // Pages to create
    $pages = array(
        'protocollen' => array(
            'title' => 'Protocollen',
            'content' => '<!-- wp:paragraph --><p>Hier vindt u alle medische protocollen van BestCare.</p><!-- /wp:paragraph -->',
            'template' => 'archive-protocollen'
        ),
        'vergaderingen' => array(
            'title' => 'Vergaderingen',
            'content' => '<!-- wp:paragraph --><p>Overzicht van alle vergaderingen en notulen.</p><!-- /wp:paragraph -->',
            'template' => 'archive-vergaderingen'
        ),
        'symposia' => array(
            'title' => 'Symposia',
            'content' => '<!-- wp:paragraph --><p>Aankomende symposia en evenementen.</p><!-- /wp:paragraph -->',
            'template' => 'archive-symposia'
        ),
        'lezingen' => array(
            'title' => 'Lezingen',
            'content' => '<!-- wp:paragraph --><p>Overzicht van lezingen en presentaties.</p><!-- /wp:paragraph -->',
            'template' => 'archive-lezingen'
        ),
        'nascholingen' => array(
            'title' => 'Nascholingen',
            'content' => '<!-- wp:paragraph --><p>Nascholingsaanbod voor dierenartsen.</p><!-- /wp:paragraph -->',
            'template' => 'archive-nascholingen'
        ),
        'inkoopovereenkomsten' => array(
            'title' => 'Inkoopovereenkomsten',
            'content' => '<!-- wp:paragraph --><p>Overzicht van alle inkoopovereenkomsten.</p><!-- /wp:paragraph -->',
            'template' => 'archive-inkoopovereenkomsten',
            'category' => 'inkoop'
        ),
        'nieuwsberichten' => array(
            'title' => 'Nieuws',
            'content' => '<!-- wp:paragraph --><p>Laatste nieuws van BestCare.</p><!-- /wp:paragraph -->',
            'template' => 'archive-nieuwsberichten'
        ),
        'praktijken' => array(
            'title' => 'Praktijken',
            'content' => '<!-- wp:paragraph --><p>Overzicht van aangesloten praktijken.</p><!-- /wp:paragraph -->',
            'template' => 'archive-praktijken'
        )
    );

    foreach ($pages as $slug => $page_data) {
        // Check if page already exists
        $existing_page = get_page_by_path($slug);

        if (!$existing_page) {
            // Create the page
            $page_id = wp_insert_post(array(
                'post_title'    => $page_data['title'],
                'post_content'  => $page_data['content'],
                'post_name'     => $slug,
                'post_status'   => 'publish',
                'post_type'     => 'page',
                'post_author'   => 1,
                'comment_status' => 'closed',
                'ping_status'   => 'closed'
            ));

            if ($page_id && !is_wp_error($page_id)) {
                // Set page template
                if (isset($page_data['template'])) {
                    update_post_meta($page_id, '_wp_page_template', $page_data['template'] . '.html');
                }

                // Assign category if specified (for access control)
                if (isset($page_data['category'])) {
                    $category = get_category_by_slug($page_data['category']);
                    if ($category) {
                        wp_set_post_categories($page_id, array($category->term_id));
                    }
                }

                error_log("BestCare Theme: Created page '{$page_data['title']}' (ID: {$page_id})");
            }
        } else {
            error_log("BestCare Theme: Page '{$page_data['title']}' already exists (ID: {$existing_page->ID})");
        }
    }
}

/**
 * Run initialization on theme activation
 */
function bestcare_theme_activation() {
    // Check if we've already run initialization
    $initialized = get_option('bestcare_theme_initialized');

    if (!$initialized) {
        bestcare_create_default_pages();

        // Mark as initialized
        update_option('bestcare_theme_initialized', true);

        error_log('BestCare Theme: Initialization complete');
    }
}

// Run on theme activation
add_action('after_switch_theme', 'bestcare_theme_activation');

/**
 * Admin action to manually recreate pages
 */
function bestcare_recreate_pages() {
    if (isset($_GET['bestcare_recreate_pages']) && current_user_can('manage_options')) {
        check_admin_referer('bestcare_recreate_pages');

        bestcare_create_default_pages();

        wp_redirect(add_query_arg('pages_created', '1', admin_url('themes.php')));
        exit;
    }
}
add_action('admin_init', 'bestcare_recreate_pages');

/**
 * Add admin notice after page creation
 */
function bestcare_pages_created_notice() {
    if (isset($_GET['pages_created']) && $_GET['pages_created'] == '1') {
        ?>
        <div class="notice notice-success is-dismissible">
            <p><?php _e('BestCare theme pages have been created/updated successfully.', 'bestcare'); ?></p>
        </div>
        <?php
    }
}
add_action('admin_notices', 'bestcare_pages_created_notice');

/**
 * Add theme action link to recreate pages
 */
function bestcare_theme_action_links($actions, $theme) {
    if ($theme->get_stylesheet() === 'BestCare' && current_user_can('manage_options')) {
        $url = wp_nonce_url(
            add_query_arg('bestcare_recreate_pages', '1', admin_url('themes.php')),
            'bestcare_recreate_pages'
        );

        $actions[] = '<a href="' . esc_url($url) . '">' . __('Create Default Pages', 'bestcare') . '</a>';
    }

    return $actions;
}
add_filter('theme_action_links', 'bestcare_theme_action_links', 10, 2);
