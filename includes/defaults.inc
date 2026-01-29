<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

// Define the theme images version - increment this when images are updated
define( 'BESTCARE_IMAGES_VERSION', '1.1' );

/**
 * Get theme images configuration
 */
function bestcare_get_theme_images() {
    return [
        'logo' => [
            'filename' => '_logo.png',
            'title' => 'BestCare Logo',
            'option' => 'bestcare_default_logo_id'
        ],
        'icon' => [
            'filename' => '_icon.png',
            'title' => 'BestCare Icon',
            'option' => 'bestcare_default_icon_id'
        ],
        'label' => [
            'filename' => '_label.png',
            'title' => 'BestCare Label',
            'option' => 'bestcare_default_label_id'
        ]
    ];
}

/**
 * Programmatically add theme images to media library
 *
 * @param bool $force Force re-upload even if images exist
 * @return array Results of the operation
 */
function bestcare_add_default_images_to_media_library( $force = false ) {
    $images = bestcare_get_theme_images();
    $results = [
        'success' => [],
        'skipped' => [],
        'errors' => []
    ];

    foreach ( $images as $key => $image ) {
        // Check if this image has already been added
        $existing_id = get_option( $image['option'] );

        if ( !$force && $existing_id && get_post( $existing_id ) ) {
            $results['skipped'][] = $image['title'] . ' (already exists)';
            continue;
        }

        // Get image path - use get_stylesheet_directory for child theme
        $image_path = get_stylesheet_directory() . '/assets/images/' . $image['filename'];

        if ( !file_exists( $image_path ) ) {
            $results['errors'][] = $image['title'] . ' (file not found: ' . $image_path . ')';
            continue;
        }

        // Delete old attachment if forcing update
        if ( $force && $existing_id ) {
            wp_delete_attachment( $existing_id, true );
        }

        // Prepare upload with unique filename to avoid conflicts
        $file_content = file_get_contents( $image_path );
        $unique_filename = 'bestcare-' . $image['filename'];
        $upload_file = wp_upload_bits( $unique_filename, null, $file_content );

        if ( !$upload_file['error'] ) {
            $wp_filetype = wp_check_filetype( $unique_filename, null );

            $attachment = [
                'post_mime_type' => $wp_filetype['type'],
                'post_title' => $image['title'],
                'post_content' => '',
                'post_status' => 'inherit'
            ];

            $attachment_id = wp_insert_attachment( $attachment, $upload_file['file'] );

            if ( !is_wp_error( $attachment_id ) ) {
                require_once( ABSPATH . 'wp-admin/includes/image.php' );
                $attachment_data = wp_generate_attachment_metadata( $attachment_id, $upload_file['file'] );
                wp_update_attachment_metadata( $attachment_id, $attachment_data );

                // Store the attachment ID for future reference
                update_option( $image['option'], $attachment_id );

                // Set as custom logo
                if ( $key === 'logo' ) {
                    set_theme_mod( 'custom_logo', $attachment_id );
                }

                // Set as site icon
                if ( $key === 'icon' ) {
                    update_option( 'site_icon', $attachment_id );
                }

                $results['success'][] = $image['title'];
            } else {
                $results['errors'][] = $image['title'] . ' (insert failed)';
            }
        } else {
            $results['errors'][] = $image['title'] . ' (upload error: ' . $upload_file['error'] . ')';
        }
    }

    // Update the images version
    update_option( 'bestcare_images_version', BESTCARE_IMAGES_VERSION );

    return $results;
}

/**
 * Check if images need to be updated based on version
 */
function bestcare_check_images_version() {
    $current_version = get_option( 'bestcare_images_version', '0' );

    // If version doesn't match or images don't exist, update them
    if ( version_compare( $current_version, BESTCARE_IMAGES_VERSION, '<' ) ||
         !get_option( 'bestcare_default_logo_id' ) ||
         !get_option( 'bestcare_default_icon_id' ) ) {

        // Force update if version changed, otherwise just add missing images
        $force = version_compare( $current_version, BESTCARE_IMAGES_VERSION, '<' ) && $current_version !== '0';
        bestcare_add_default_images_to_media_library( $force );
    }
}

// Run on theme activation and switch
add_action( 'after_switch_theme', 'bestcare_check_images_version' );

// Run on admin init to catch theme updates
add_action( 'admin_init', 'bestcare_check_images_version' );

/**
 * Add admin menu for manual image management
 */
function bestcare_add_images_admin_menu() {
    add_theme_page(
        'BestCare Default Images',
        'Default Images',
        'manage_options',
        'bestcare-default-images',
        'bestcare_render_images_admin_page'
    );
}
add_action( 'admin_menu', 'bestcare_add_images_admin_menu' );

/**
 * Render the admin page for image management
 */
function bestcare_render_images_admin_page() {
    if ( !current_user_can( 'manage_options' ) ) {
        wp_die( 'Unauthorized' );
    }

    // Handle form submission
    if ( isset( $_POST['bestcare_update_images'] ) && check_admin_referer( 'bestcare_update_images' ) ) {
        $force = isset( $_POST['force_update'] );
        $results = bestcare_add_default_images_to_media_library( $force );

        echo '<div class="notice notice-success"><p><strong>Results:</strong></p>';
        if ( !empty( $results['success'] ) ) {
            echo '<p>✓ Successfully uploaded: ' . implode( ', ', $results['success'] ) . '</p>';
        }
        if ( !empty( $results['skipped'] ) ) {
            echo '<p>○ Skipped: ' . implode( ', ', $results['skipped'] ) . '</p>';
        }
        if ( !empty( $results['errors'] ) ) {
            echo '<p style="color: red;">✗ Errors: ' . implode( ', ', $results['errors'] ) . '</p>';
        }
        echo '</div>';
    }

    $images = bestcare_get_theme_images();
    $current_version = get_option( 'bestcare_images_version', 'Not set' );

    ?>
    <div class="wrap">
        <h1>BestCare Default Images</h1>

        <div class="card">
            <h2>Current Status</h2>
            <table class="widefat">
                <thead>
                    <tr>
                        <th>Image</th>
                        <th>Status</th>
                        <th>Attachment ID</th>
                        <th>Preview</th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ( $images as $key => $image ):
                        $attachment_id = get_option( $image['option'] );
                        $exists = $attachment_id && get_post( $attachment_id );
                        $file_path = get_stylesheet_directory() . '/assets/images/' . $image['filename'];
                        $file_exists = file_exists( $file_path );
                    ?>
                    <tr>
                        <td><strong><?php echo esc_html( $image['title'] ); ?></strong><br>
                            <code><?php echo esc_html( $image['filename'] ); ?></code></td>
                        <td>
                            <?php if ( $exists ): ?>
                                <span style="color: green;">✓ Uploaded</span>
                            <?php else: ?>
                                <span style="color: red;">✗ Not uploaded</span>
                            <?php endif; ?>
                            <br>
                            <?php if ( $file_exists ): ?>
                                <span style="color: green;">✓ File exists in theme</span>
                            <?php else: ?>
                                <span style="color: red;">✗ File missing from theme</span>
                            <?php endif; ?>
                        </td>
                        <td><?php echo $attachment_id ? $attachment_id : 'N/A'; ?></td>
                        <td>
                            <?php if ( $exists ):
                                echo wp_get_attachment_image( $attachment_id, 'thumbnail' );
                            elseif ( $file_exists ):
                                $file_url = get_stylesheet_directory_uri() . '/assets/images/' . $image['filename'];
                                echo '<img src="' . esc_url( $file_url ) . '" style="max-width: 150px; height: auto;">';
                            endif; ?>
                        </td>
                    </tr>
                    <?php endforeach; ?>
                </tbody>
            </table>

            <p><strong>Images Version:</strong> <?php echo esc_html( $current_version ); ?>
               (Theme expects: <?php echo BESTCARE_IMAGES_VERSION; ?>)</p>
        </div>

        <div class="card">
            <h2>Update Images</h2>
            <form method="post">
                <?php wp_nonce_field( 'bestcare_update_images' ); ?>

                <p>
                    <label>
                        <input type="checkbox" name="force_update" value="1">
                        Force re-upload all images (will replace existing uploads)
                    </label>
                </p>

                <p>
                    <button type="submit" name="bestcare_update_images" class="button button-primary">
                        Upload/Update Default Images
                    </button>
                </p>

                <p class="description">
                    This will upload the default images from your theme's assets/images folder
                    to the WordPress media library and set them as your site logo and icon.
                    <?php if ( !current_user_can( 'upload_files' ) ): ?>
                        <br><strong>Note:</strong> You need upload permissions to use this feature.
                    <?php endif; ?>
                </p>
            </form>
        </div>

        <div class="card">
            <h2>For End Users</h2>
            <p>When you update the theme with new images:</p>
            <ol>
                <li>Update the <code>BESTCARE_IMAGES_VERSION</code> constant in this file</li>
                <li>The images will automatically update when an admin visits the WordPress admin area</li>
                <li>Or users can click the update button above</li>
            </ol>
        </div>
    </div>
    <?php
}

/**
 * Add admin notice if images need attention
 */
function bestcare_images_admin_notice() {
    if ( !current_user_can( 'manage_options' ) ) {
        return;
    }

    $current_version = get_option( 'bestcare_images_version', '0' );

    if ( version_compare( $current_version, BESTCARE_IMAGES_VERSION, '<' ) ) {
        ?>
        <div class="notice notice-warning is-dismissible">
            <p>
                <strong>BestCare Theme:</strong> Default images need to be updated.
                <a href="<?php echo admin_url( 'themes.php?page=bestcare-default-images' ); ?>">
                    Update now
                </a>
            </p>
        </div>
        <?php
    }
}

add_action( 'admin_notices', 'bestcare_images_admin_notice' );

/**
 * Apply default logo if none is set (fallback)
 *
 * @param string $html The custom logo HTML
 * @return string Modified HTML with default logo if needed
 */
function apply_default_logo( $html ) {
    if ( empty( $html ) ) {
        $logo_id = get_option( 'bestcare_default_logo_id' );
        if ( $logo_id ) {
            $html = wp_get_attachment_image( $logo_id, 'full', false, [
                'class' => 'custom-logo',
                'alt' => get_bloginfo( 'name' )
            ] );
        } else {
            // Fallback to direct file reference
            $logo_url = get_stylesheet_directory_uri() . '/assets/images/_logo.png';
            $html = '<img src="' . esc_url( $logo_url ) . '" alt="' . esc_attr( get_bloginfo( 'name' ) ) . '" class="custom-logo">';
        }
    }
    return $html;
}

add_filter( 'get_custom_logo', 'apply_default_logo' );

/**
 * Lock theme images to prevent deletion or changes by non-admins
 */
function bestcare_lock_default_images( $caps, $cap, $user_id, $args ) {
    // Only apply to delete_post capability
    if ( $cap !== 'delete_post' && $cap !== 'edit_post' ) {
        return $caps;
    }

    // Get the post ID being checked
    if ( isset( $args[0] ) ) {
        $post_id = $args[0];

        // Check if this is one of our protected images
        $protected_ids = [
            get_option( 'bestcare_default_logo_id' ),
            get_option( 'bestcare_default_icon_id' ),
            get_option( 'bestcare_default_label_id' )
        ];

        if ( in_array( $post_id, $protected_ids ) ) {
            // Only allow administrators to modify/delete
            if ( !user_can( $user_id, 'manage_options' ) ) {
                $caps[] = 'do_not_allow';
            }
        }
    }

    return $caps;
}

add_filter( 'map_meta_cap', 'bestcare_lock_default_images', 10, 4 );

/**
 * Prevent logo and icon from being removed via customizer by non-admins
 */
function bestcare_enforce_logo_and_icon( $value, $old_value ) {
    if ( !current_user_can( 'manage_options' ) ) {
        // If someone tries to remove the logo, restore it
        if ( empty( $value ) ) {
            return $old_value;
        }
    }
    return $value;
}

add_filter( 'pre_update_option_theme_mod_custom_logo', 'bestcare_enforce_logo_and_icon', 10, 2 );
add_filter( 'pre_update_option_site_icon', 'bestcare_enforce_logo_and_icon', 10, 2 );

/**
 * Hide logo and icon customizer controls from non-admins
 */
function bestcare_hide_logo_controls_for_non_admins( $wp_customize ) {
    if ( !current_user_can( 'manage_options' ) ) {
        $wp_customize->remove_control( 'custom_logo' );
        $wp_customize->remove_control( 'site_icon' );
    }
}

add_action( 'customize_register', 'bestcare_hide_logo_controls_for_non_admins', 20 );
