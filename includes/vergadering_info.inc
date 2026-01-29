<?php
// Exit if accessed directly
if ( !defined( 'ABSPATH' ) ) exit;

/**
 * Render vergadering information
 *
 * @param array $vergadering_fields Full array from get_fields()
 * @return void
 */
function render_vergadering_info($vergadering_fields) {
    $datum_tijd = $vergadering_fields['vergadering_datum_en_tijd'] ?? '';
    $agenda_inline = $vergadering_fields['vergadering_agenda_inline'] ?? '';
    $agenda_file = $vergadering_fields['vergadering_agenda_file'] ?? null;
    $notulen = $vergadering_fields['vergadering_notulen'] ?? null;
    $aanwezig = $vergadering_fields['vergadering_aanwezig'] ?? [];
    $beschrijving = $vergadering_fields['vergadering_beschrijving'] ?? '';
    $opmerkingen = $vergadering_fields['vergadering_opmerkingen'] ?? '';

    echo '<div class="vergadering-info">';

    // Date and time
    if ($datum_tijd) {
        echo '<div class="vergadering-datum">';
        echo '<h3><i class="fa-solid fa-calendar"></i> Datum en Tijd</h3>';
        echo '<p>' . esc_html($datum_tijd) . '</p>';
        echo '</div>';
    }

    // Description
    if ($beschrijving) {
        echo '<div class="vergadering-beschrijving">';
        echo '<h3><i class="fa-solid fa-file-lines"></i> Beschrijving</h3>';
        echo '<p>' . nl2br(esc_html($beschrijving)) . '</p>';
        echo '</div>';
    }

    // Agenda inline
    if ($agenda_inline) {
        echo '<div class="vergadering-agenda-inline">';
        echo '<h3><i class="fa-solid fa-list-check"></i> Agenda</h3>';
        echo wp_kses_post($agenda_inline);
        echo '</div>';
    }

    // Agenda file
    if ($agenda_file && is_array($agenda_file)) {
        echo '<div class="vergadering-agenda-file">';
        echo '<h3><i class="fa-solid fa-file-pdf"></i> Agenda Document</h3>';
        echo '<a href="' . esc_url($agenda_file['url']) . '" target="_blank" class="vergadering-file-link">';
        echo '<i class="fa-solid fa-download"></i> ' . esc_html($agenda_file['filename']);
        echo ' (' . size_format($agenda_file['filesize']) . ')';
        echo '</a>';
        echo '</div>';
    }

    // Notulen
    if ($notulen && is_array($notulen)) {
        echo '<div class="vergadering-notulen">';
        echo '<h3><i class="fa-solid fa-file-pdf"></i> Notulen</h3>';
        echo '<a href="' . esc_url($notulen['url']) . '" target="_blank" class="vergadering-file-link">';
        echo '<i class="fa-solid fa-download"></i> ' . esc_html($notulen['filename']);
        echo ' (' . size_format($notulen['filesize']) . ')';
        echo '</a>';
        echo '</div>';
    }

    // Attendees
    if (!empty($aanwezig)) {
        echo '<div class="vergadering-aanwezig">';
        echo '<h3><i class="fa-solid fa-users"></i> Aanwezig</h3>';
        echo '<ul class="vergadering-aanwezig-list">';
        foreach ($aanwezig as $user) {
            $display_name = $user['display_name'] ?? $user['user_nicename'] ?? '';
            echo '<li>' . esc_html($display_name) . '</li>';
        }
        echo '</ul>';
        echo '</div>';
    }

    // Remarks
    if ($opmerkingen) {
        echo '<div class="vergadering-opmerkingen">';
        echo '<h3><i class="fa-solid fa-comment"></i> Opmerkingen</h3>';
        echo '<p>' . esc_html($opmerkingen) . '</p>';
        echo '</div>';
    }

    echo '</div>';
}

/**
 * Render vergadering calendar
 *
 * @return void
 */
function render_vergadering_calendar() {
    $args = array(
        'post_type' => 'vergadering',
        'posts_per_page' => -1,
        'post_status' => 'publish',
        'orderby' => 'meta_value',
        'meta_key' => 'vergadering_datum_en_tijd',
        'order' => 'ASC'
    );

    $vergaderingen = new WP_Query($args);

    if (!$vergaderingen->have_posts()) {
        echo '<p>Geen vergaderingen gepland.</p>';
        return;
    }

    echo '<div class="vergadering-calendar">';
    echo '<h2><i class="fa-solid fa-calendar-days"></i> Vergadering Kalender</h2>';

    // Group vergaderingen by month
    $months = array();
    while ($vergaderingen->have_posts()) {
        $vergaderingen->the_post();
        $datum_tijd = get_field('vergadering_datum_en_tijd');

        if ($datum_tijd) {
            // Parse the date
            $date = DateTime::createFromFormat('d/m/Y g:i a', $datum_tijd);
            if ($date) {
                $month_key = $date->format('Y-m');
                $month_name = $date->format('F Y');

                if (!isset($months[$month_key])) {
                    $months[$month_key] = array(
                        'name' => $month_name,
                        'meetings' => array()
                    );
                }

                $months[$month_key]['meetings'][] = array(
                    'id' => get_the_ID(),
                    'title' => get_the_title(),
                    'date' => $date,
                    'datum_tijd' => $datum_tijd,
                    'permalink' => get_permalink()
                );
            }
        }
    }

    // Display by month
    foreach ($months as $month_data) {
        echo '<div class="vergadering-month">';
        echo '<h3>' . esc_html($month_data['name']) . '</h3>';
        echo '<ul class="vergadering-month-list">';

        foreach ($month_data['meetings'] as $meeting) {
            echo '<li class="vergadering-item">';
            echo '<span class="vergadering-date">' . esc_html($meeting['date']->format('d M')) . '</span>';
            echo '<a href="' . esc_url($meeting['permalink']) . '">' . esc_html($meeting['title']) . '</a>';
            echo '<span class="vergadering-time">' . esc_html($meeting['date']->format('H:i')) . '</span>';
            echo '</li>';
        }

        echo '</ul>';
        echo '</div>';
    }

    echo '</div>';

    wp_reset_postdata();
}

/**
 * Render vergadering list
 *
 * @return void
 */
function render_vergadering_list() {
    $args = array(
        'post_type' => 'vergadering',
        'posts_per_page' => 20,
        'post_status' => 'publish',
        'orderby' => 'meta_value',
        'meta_key' => 'vergadering_datum_en_tijd',
        'order' => 'DESC'
    );

    $vergaderingen = new WP_Query($args);

    if (!$vergaderingen->have_posts()) {
        echo '<p>Geen vergaderingen gevonden.</p>';
        return;
    }

    echo '<div class="vergadering-list">';
    echo '<h2><i class="fa-solid fa-list"></i> Alle Vergaderingen</h2>';
    echo '<table class="vergadering-table">';
    echo '<thead>';
    echo '<tr>';
    echo '<th>Datum</th>';
    echo '<th>Vergadering</th>';
    echo '<th>Documenten</th>';
    echo '</tr>';
    echo '</thead>';
    echo '<tbody>';

    while ($vergaderingen->have_posts()) {
        $vergaderingen->the_post();
        $datum_tijd = get_field('vergadering_datum_en_tijd');
        $agenda_file = get_field('vergadering_agenda_file');
        $notulen = get_field('vergadering_notulen');

        echo '<tr>';
        echo '<td class="vergadering-datum">' . esc_html($datum_tijd) . '</td>';
        echo '<td class="vergadering-titel">';
        echo '<a href="' . esc_url(get_permalink()) . '">' . esc_html(get_the_title()) . '</a>';
        echo '</td>';
        echo '<td class="vergadering-documenten">';

        if ($agenda_file && is_array($agenda_file)) {
            echo '<a href="' . esc_url($agenda_file['url']) . '" target="_blank" title="Agenda">';
            echo '<i class="fa-solid fa-file-pdf"></i> Agenda';
            echo '</a> ';
        }

        if ($notulen && is_array($notulen)) {
            echo '<a href="' . esc_url($notulen['url']) . '" target="_blank" title="Notulen">';
            echo '<i class="fa-solid fa-file-pdf"></i> Notulen';
            echo '</a>';
        }

        echo '</td>';
        echo '</tr>';
    }

    echo '</tbody>';
    echo '</table>';
    echo '</div>';

    wp_reset_postdata();
}
