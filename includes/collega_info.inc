<?php
/**
 * Collega Information Display Functions
 *
 * Functions for displaying colleague (user) information from ACF fields
 *
 * @package BestCare
 */

/**
 * Render complete colleague information
 * Displays all relevant colleague data from ACF user fields
 *
 * @param int $user_id The user ID
 * @return void
 */
function render_collega_info($user_id = null) {
    if (!$user_id) {
        $user_id = get_queried_object_id();
    }

    $user = get_userdata($user_id);
    if (!$user) {
        return;
    }

    // Get ACF fields
    $praktijk_ids = get_field('collega_praktijk', 'user_' . $user_id);
    $telefoon_praktijk = get_field('collega_telefoon_praktijk', 'user_' . $user_id);
    $functies = get_field('collega_functie', 'user_' . $user_id);
    $specialisaties = get_field('collega_diersoortspecialisatie', 'user_' . $user_id);
    $afstudeerjaar = get_field('collega_afstudeerjaar', 'user_' . $user_id);
    $wat_zeggen_anderen = get_field('collega_wat_zeggen_anderen_over_jou', 'user_' . $user_id);

    echo '<div class="collega-info">';

    // Contact information
    if ($telefoon_praktijk) {
        echo '<div class="collega-contact">';
        echo '<h3>Contact</h3>';
        echo '<div class="collega-phone">';
        echo '<i class="fa-solid fa-phone"></i> ' . esc_html($telefoon_praktijk);
        echo '</div>';
        echo '</div>';
    }

    // Praktijk association
    if (!empty($praktijk_ids)) {
        echo '<div class="collega-praktijk">';
        echo '<h3>Praktijk</h3>';
        echo '<ul class="collega-praktijk-list">';
        foreach ($praktijk_ids as $praktijk_id) {
            $praktijk_url = get_permalink($praktijk_id);
            $praktijk_name = get_the_title($praktijk_id);
            echo '<li><a href="' . esc_url($praktijk_url) . '">' . esc_html($praktijk_name) . '</a></li>';
        }
        echo '</ul>';
        echo '</div>';
    }

    // Function/Role
    if (!empty($functies)) {
        echo '<div class="collega-functie">';
        echo '<h3>Functie</h3>';
        echo '<ul class="collega-functie-list">';
        foreach ($functies as $functie) {
            echo '<li>' . esc_html($functie) . '</li>';
        }
        echo '</ul>';
        echo '</div>';
    }

    // Specializations
    if (!empty($specialisaties)) {
        echo '<div class="collega-specialisaties">';
        echo '<h3>Diersoortspecialisatie</h3>';
        echo '<ul class="collega-specialisaties-list">';
        foreach ($specialisaties as $specialisatie) {
            echo '<li>' . esc_html($specialisatie) . '</li>';
        }
        echo '</ul>';
        echo '</div>';
    }

    // Graduation year
    if ($afstudeerjaar) {
        echo '<div class="collega-afstudeerjaar">';
        echo '<h3>Afgestudeerd</h3>';
        echo '<p>' . esc_html($afstudeerjaar) . '</p>';
        echo '</div>';
    }

    // What others say
    if ($wat_zeggen_anderen) {
        echo '<div class="collega-testimonial">';
        echo '<h3>Wat zeggen anderen over mij?</h3>';
        echo '<blockquote>' . esc_html($wat_zeggen_anderen) . '</blockquote>';
        echo '</div>';
    }

    echo '</div>';
}

/**
 * Render a list of all colleagues
 * Displays each colleague as a list item with link to their page
 *
 * @param array $args Optional arguments to filter the colleague list
 * @return void
 */
function render_collega_list($args = array()) {
    $defaults = array(
        'role' => '', // Filter by role if needed
        'orderby' => 'display_name',
        'order' => 'ASC',
        'number' => -1, // All users
    );

    $args = wp_parse_args($args, $defaults);

    $users = get_users($args);

    if (empty($users)) {
        return;
    }

    echo '<div class="collega-list-container">';
    echo '<ul class="collega-list">';

    foreach ($users as $user) {
        $user_id = $user->ID;
        $user_url = get_author_posts_url($user_id);
        $display_name = $user->display_name;

        // Get ACF fields
        $praktijk_ids = get_field('collega_praktijk', 'user_' . $user_id);
        $functies = get_field('collega_functie', 'user_' . $user_id);

        // Build praktijk info
        $praktijk_names = array();
        if (!empty($praktijk_ids)) {
            foreach ($praktijk_ids as $praktijk_id) {
                $praktijk_names[] = get_the_title($praktijk_id);
            }
        }

        // Build CSS classes
        $item_class = 'collega-list-item';
        if (!empty($praktijk_ids)) {
            if (count($praktijk_ids) > 1) {
                $item_class .= ' collega-multiple-praktijken';
            } else {
                $item_class .= ' collega-single-praktijk';
            }
        }

        echo '<li class="' . esc_attr($item_class) . '">';
        echo '<a href="' . esc_url($user_url) . '" class="collega-link">';
        echo '<span class="collega-name">' . esc_html($display_name) . '</span>';

        // Display primary function if exists
        if (!empty($functies)) {
            $primary_functie = is_array($functies) ? $functies[0] : $functies;
            echo ' <span class="collega-functie-tag">' . esc_html($primary_functie) . '</span>';
        }

        // Display praktijk info
        if (!empty($praktijk_names)) {
            echo ' <span class="collega-praktijk-info">(' . esc_html(implode(', ', $praktijk_names)) . ')</span>';
        }

        echo '</a>';
        echo '</li>';
    }

    echo '</ul>';
    echo '</div>';
}

/**
 * Get colleagues associated with a specific praktijk
 *
 * @param int $praktijk_id The praktijk post ID
 * @return array Array of user objects
 */
function get_collega_by_praktijk($praktijk_id) {
    $all_users = get_users(array('number' => -1));
    $matched_users = array();

    foreach ($all_users as $user) {
        $user_praktijken = get_field('collega_praktijk', 'user_' . $user->ID);
        if (!empty($user_praktijken) && in_array($praktijk_id, $user_praktijken)) {
            $matched_users[] = $user;
        }
    }

    return $matched_users;
}

/**
 * Display colleagues for a specific praktijk
 * Useful for showing on praktijk pages
 *
 * @param int $praktijk_id The praktijk post ID
 * @return void
 */
function render_praktijk_collega_list($praktijk_id = null) {
    if (!$praktijk_id) {
        $praktijk_id = get_the_ID();
    }

    $colleagues = get_collega_by_praktijk($praktijk_id);

    if (empty($colleagues)) {
        return;
    }

    echo '<div class="praktijk-collega-list">';
    echo '<h3>Ons team</h3>';
    echo '<ul class="collega-simple-list">';

    foreach ($colleagues as $user) {
        $user_url = get_author_posts_url($user->ID);
        $display_name = $user->display_name;
        $functies = get_field('collega_functie', 'user_' . $user->ID);

        echo '<li class="collega-item">';
        echo '<a href="' . esc_url($user_url) . '">';
        echo '<span class="collega-name">' . esc_html($display_name) . '</span>';

        if (!empty($functies)) {
            $primary_functie = is_array($functies) ? $functies[0] : $functies;
            echo ' <span class="collega-role">(' . esc_html($primary_functie) . ')</span>';
        }

        echo '</a>';
        echo '</li>';
    }

    echo '</ul>';
    echo '</div>';
}

/**
 * Example usage:
 *
 * // Single colleague page (author.php or similar):
 * $user_id = get_queried_object_id();
 * render_collega_info($user_id);
 *
 * // List all colleagues:
 * render_collega_list();
 *
 * // List colleagues from a specific praktijk:
 * render_praktijk_collega_list(123); // where 123 is the praktijk post ID
 *
 * // On a praktijk page, show team members:
 * render_praktijk_collega_list(); // uses current post ID
 */
