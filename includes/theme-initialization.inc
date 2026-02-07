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
 * Add admin menu item under Tools
 */
function bestcare_add_tools_menu() {
    add_management_page(
        __('Create Default Pages', 'bestcare'),
        __('Create Default Pages', 'bestcare'),
        'manage_options',
        'bestcare-create-pages',
        'bestcare_render_tools_page'
    );
}
add_action('admin_menu', 'bestcare_add_tools_menu');

/**
 * Render the tools page
 */
function bestcare_render_tools_page() {
    // Handle form submission
    if (isset($_POST['create_pages']) && check_admin_referer('bestcare_create_pages')) {
        bestcare_create_default_pages();
        echo '<div class="notice notice-success is-dismissible"><p>' . __('Pages have been created/updated successfully!', 'bestcare') . '</p></div>';
    }

    // Get page status
    $pages = array(
        'protocollen' => 'Protocollen',
        'vergaderingen' => 'Vergaderingen',
        'symposia' => 'Symposia',
        'lezingen' => 'Lezingen',
        'nascholingen' => 'Nascholingen',
        'inkoopovereenkomsten' => 'Inkoopovereenkomsten',
        'nieuwsberichten' => 'Nieuws',
        'praktijken' => 'Praktijken'
    );

    ?>
    <div class="wrap">
        <h1><?php _e('Create Default Pages', 'bestcare'); ?></h1>

        <div class="card">
            <h2><?php _e('Page Status', 'bestcare'); ?></h2>
            <table class="wp-list-table widefat fixed striped">
                <thead>
                    <tr>
                        <th><?php _e('Page Title', 'bestcare'); ?></th>
                        <th><?php _e('Slug', 'bestcare'); ?></th>
                        <th><?php _e('Status', 'bestcare'); ?></th>
                        <th><?php _e('Action', 'bestcare'); ?></th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ($pages as $slug => $title): ?>
                        <?php
                        $page = get_page_by_path($slug);
                        $exists = !empty($page);
                        ?>
                        <tr>
                            <td><strong><?php echo esc_html($title); ?></strong></td>
                            <td><code><?php echo esc_html($slug); ?></code></td>
                            <td>
                                <?php if ($exists): ?>
                                    <span style="color: #46b450;">✓ <?php _e('Exists', 'bestcare'); ?></span>
                                <?php else: ?>
                                    <span style="color: #dc3232;">✗ <?php _e('Missing', 'bestcare'); ?></span>
                                <?php endif; ?>
                            </td>
                            <td>
                                <?php if ($exists): ?>
                                    <a href="<?php echo get_edit_post_link($page->ID); ?>" class="button button-small">
                                        <?php _e('Edit', 'bestcare'); ?>
                                    </a>
                                    <a href="<?php echo get_permalink($page->ID); ?>" class="button button-small" target="_blank">
                                        <?php _e('View', 'bestcare'); ?>
                                    </a>
                                <?php else: ?>
                                    <span style="color: #646970;">—</span>
                                <?php endif; ?>
                            </td>
                        </tr>
                    <?php endforeach; ?>
                </tbody>
            </table>
        </div>

        <div class="card" style="margin-top: 20px;">
            <h2><?php _e('Create or Update Pages', 'bestcare'); ?></h2>
            <p><?php _e('Click the button below to create missing pages. Existing pages will not be modified.', 'bestcare'); ?></p>

            <form method="post">
                <?php wp_nonce_field('bestcare_create_pages'); ?>
                <p>
                    <button type="submit" name="create_pages" class="button button-primary button-hero">
                        <?php _e('Create Missing Pages', 'bestcare'); ?>
                    </button>
                </p>
            </form>

            <p class="description">
                <?php _e('This will create pages for: Protocollen, Vergaderingen, Symposia, Lezingen, Nascholingen, Inkoopovereenkomsten, Nieuws, and Praktijken.', 'bestcare'); ?>
            </p>
        </div>
    </div>
    <?php
}

/**
 * Admin action to manually recreate pages (legacy support)
 */
function bestcare_recreate_pages() {
    if (isset($_GET['bestcare_recreate_pages']) && current_user_can('manage_options')) {
        check_admin_referer('bestcare_recreate_pages');

        bestcare_create_default_pages();

        wp_redirect(add_query_arg('pages_created', '1', admin_url('tools.php?page=bestcare-create-pages')));
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
