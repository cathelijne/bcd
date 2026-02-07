<?php
/**
 * Environment Indicator
 *
 * Displays environment status (ddev/dev/staging) based on hostname
 * Shows in frontend header and admin bar
 *
 * @package BestCare
 */

/**
 * Detect the current environment from hostname
 *
 * @return string|false Environment name or false for production
 */
function get_current_environment() {
    $hostname = $_SERVER['HTTP_HOST'] ?? $_SERVER['SERVER_NAME'] ?? '';

    if (strpos($hostname, '.ddev.site') !== false) {
        return 'ddev';
    }

    if (strpos($hostname, 'dev.') === 0 || strpos($hostname, '-dev.') !== false) {
        return 'dev';
    }

    if (strpos($hostname, 'staging.') === 0 || strpos($hostname, '-staging.') !== false || strpos($hostname, '.staging.') !== false) {
        return 'staging';
    }

    return false;
}

/**
 * Get environment display properties
 *
 * @param string $env Environment name
 * @return array Display properties (label, color, bg_color)
 */
function get_environment_display($env) {
    $displays = array(
        'ddev' => array(
            'label' => 'DDEV',
            'color' => '#ffffff',
            'bg_color' => '#2c3e50',
        ),
        'dev' => array(
            'label' => 'DEVELOPMENT',
            'color' => '#ffffff',
            'bg_color' => '#e74c3c',
        ),
        'staging' => array(
            'label' => 'STAGING',
            'color' => '#ffffff',
            'bg_color' => '#f39c12',
        ),
    );

    return $displays[$env] ?? array(
        'label' => strtoupper($env),
        'color' => '#ffffff',
        'bg_color' => '#95a5a6',
    );
}

/**
 * Display environment indicator in frontend
 */
function display_frontend_environment_indicator() {
    $env = get_current_environment();

    if (!$env) {
        return;
    }

    $display = get_environment_display($env);

    ?>
    <div class="environment-indicator-banner" style="background-color: <?php echo esc_attr($display['bg_color']); ?>; color: <?php echo esc_attr($display['color']); ?>;">
        <div class="environment-indicator-content">
            <span class="environment-indicator-icon">⚠️</span>
            <span class="environment-indicator-label"><?php echo esc_html($display['label']); ?> ENVIRONMENT</span>
        </div>
    </div>
    <?php
}
add_action('wp_body_open', 'display_frontend_environment_indicator');

/**
 * Add environment indicator to admin bar
 */
function add_admin_bar_environment_indicator($wp_admin_bar) {
    $env = get_current_environment();

    if (!$env) {
        return;
    }

    $display = get_environment_display($env);

    $wp_admin_bar->add_node(array(
        'id' => 'environment-indicator',
        'title' => '<span class="environment-indicator-admin-bar" style="background-color: ' . esc_attr($display['bg_color']) . '; color: ' . esc_attr($display['color']) . ';">⚠️ ' . esc_html($display['label']) . '</span>',
        'href' => false,
        'meta' => array(
            'class' => 'environment-indicator-node',
        ),
    ));
}
add_action('admin_bar_menu', 'add_admin_bar_environment_indicator', 10);

/**
 * Add environment indicator styles
 */
function environment_indicator_styles() {
    $env = get_current_environment();

    if (!$env) {
        return;
    }

    ?>
    <style>
        /* Frontend banner */
        .environment-indicator-banner {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            z-index: 99998;
            padding: 8px 15px;
            text-align: center;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
            font-size: 13px;
            font-weight: 600;
            letter-spacing: 0.5px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
        }

        .environment-indicator-content {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 8px;
        }

        .environment-indicator-icon {
            font-size: 16px;
        }

        .environment-indicator-label {
            font-weight: 700;
        }

        /* Adjust body padding when banner is shown */
        body.wp-admin {
            /* Admin already has top margin for admin bar */
        }

        body:not(.wp-admin) {
            padding-top: 38px !important;
        }

        /* Admin bar indicator */
        #wpadminbar .environment-indicator-admin-bar {
            display: inline-block;
            padding: 4px 12px;
            border-radius: 3px;
            font-weight: 700;
            font-size: 12px;
            letter-spacing: 0.5px;
            line-height: 1.4;
        }

        #wpadminbar .environment-indicator-node {
            background: transparent !important;
        }

        #wpadminbar .environment-indicator-node:hover {
            background: transparent !important;
        }

        #wpadminbar .environment-indicator-node > .ab-item {
            background: transparent !important;
            padding: 0 10px;
        }

        #wpadminbar .environment-indicator-node > .ab-item:hover {
            background: transparent !important;
        }

        /* Adjust for admin bar when both are present */
        body.admin-bar:not(.wp-admin) .environment-indicator-banner {
            top: 32px;
        }

        @media screen and (max-width: 782px) {
            body.admin-bar:not(.wp-admin) .environment-indicator-banner {
                top: 46px;
            }

            .environment-indicator-banner {
                font-size: 12px;
                padding: 6px 10px;
            }

            body:not(.wp-admin) {
                padding-top: 34px !important;
            }
        }

        @media screen and (max-width: 600px) {
            body.admin-bar:not(.wp-admin) {
                padding-top: 80px !important;
            }
        }
    </style>
    <?php
}
add_action('wp_head', 'environment_indicator_styles');
add_action('admin_head', 'environment_indicator_styles');
