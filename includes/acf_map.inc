<?php
/**
 * Internal: Display a Leaflet map from ACF OpenStreetMap field data
 * Should only be called by render_praktijk_map()
 *
 * @param array $map_data The map data array from ACF field
 * @param string $praktijk_name Optional praktijk name to include in marker labels
 * @return void
 * @access private
 */
function _render_leaflet_map($map_data, $praktijk_name = '') {
    // Verify this is called from an allowed function
    $backtrace = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 2);
    $caller = $backtrace[1]['function'] ?? '';
    if ($caller !== 'render_praktijk_map') {
        trigger_error('_render_leaflet_map() is an internal function and should not be called directly', E_USER_WARNING);
        return;
    }

    if (empty($map_data)) {
        return;
    }

    $center_lat = $map_data['center_lat'] ?? $map_data['lat'] ?? 0;
    $center_lng = $map_data['center_lng'] ?? $map_data['lng'] ?? 0;
    $zoom = $map_data['zoom'] ?? 14;
    $markers = $map_data['markers'] ?? [];
    $layers = $map_data['layers'] ?? ['OpenStreetMap.Mapnik'];

    // Enrich marker labels with address information
    foreach ($markers as &$marker) {
        $address = _get_marker_address_components($marker);
        $location_name = $marker['label'] ?? 'Locatie';

        // Use marker's own praktijk name if available (for combined maps)
        $marker_praktijk_name = $marker['_praktijk_name'] ?? $praktijk_name;

        // Build enhanced label with HTML
        $label_parts = [];

        if (!empty($marker_praktijk_name)) {
            $label_parts[] = '<strong>' . esc_html($marker_praktijk_name) . '</strong>';
        }

        if (!empty($location_name)) {
            $label_parts[] = '<em>' . esc_html($location_name) . '</em>';
        }

        if ($address['road'] || $address['house_number']) {
            $label_parts[] = esc_html(trim($address['road'] . ' ' . $address['house_number']));
        }

        if ($address['postcode'] || $address['city']) {
            $label_parts[] = esc_html(trim($address['postcode'] . ' ' . $address['city']));
        }

        // Join with line breaks
        $marker['label'] = implode('<br>', $label_parts);
    }
    unset($marker); // Break reference

    $layers_json = json_encode($layers);
    $markers_json = json_encode($markers);
    $auto_fit = count($markers) > 1; // Auto-fit bounds when multiple markers
    ?>
<div class="leaflet-map leaflet-container leaflet-touch leaflet-retina leaflet-fade-anim leaflet-grab leaflet-touch-drag leaflet-touch-zoom"
	data-height="400"
	data-map="leaflet"
	data-map-lng="<?php echo esc_attr($center_lng); ?>"
	data-map-lat="<?php echo esc_attr($center_lat); ?>"
	data-map-zoom="<?php echo esc_attr($zoom); ?>"
	data-map-layers="<?php echo htmlspecialchars($layers_json, ENT_QUOTES, 'UTF-8'); ?>"
	data-map-markers='<?php echo htmlspecialchars($markers_json, ENT_QUOTES, 'UTF-8'); ?>'
	data-auto-fit-bounds="<?php echo $auto_fit ? '1' : '0'; ?>"
	style="height: 400px; position: relative;">

	<div class="leaflet-pane leaflet-map-pane" style="transform: translate3d(0px, 0px, 0px);">
		<div class="leaflet-pane leaflet-tile-pane">
			<div class="leaflet-layer " style="z-index: 1; opacity: 1;">
				<div class="leaflet-tile-container leaflet-zoom-animated" style="z-index: 19; transform: translate3d(0px, 0px, 0px) scale(1);"></div>
			</div>
		</div>
		<div class="leaflet-pane leaflet-overlay-pane"></div>
		<div class="leaflet-pane leaflet-shadow-pane"></div>
		<div class="leaflet-pane leaflet-marker-pane"></div>
		<div class="leaflet-pane leaflet-tooltip-pane"></div>
		<div class="leaflet-pane leaflet-popup-pane"></div>
		<div class="leaflet-proxy leaflet-zoom-animated"></div>
	</div>

	<div class="leaflet-control-container">
		<div class="leaflet-top leaflet-left">
			<div class="leaflet-control-zoom leaflet-bar leaflet-control">
				<a class="leaflet-control-zoom-in" href="#" title="Zoom in" role="button" aria-label="Zoom in" aria-disabled="false">
					<span aria-hidden="true">+</span>
				</a>
				<a class="leaflet-control-zoom-out" href="#" title="Zoom out" role="button" aria-label="Zoom out" aria-disabled="false">
					<span aria-hidden="true">−</span>
				</a>
			</div>
		</div>
		<div class="leaflet-top leaflet-right"></div>
		<div class="leaflet-bottom leaflet-left"></div>
		<div class="leaflet-bottom leaflet-right">
			<div class="leaflet-control-attribution leaflet-control">
				<a href="https://leafletjs.com" title="A JavaScript library for interactive maps">
					<svg aria-hidden="true" xmlns="http://www.w3.org/2000/svg" width="12" height="8" viewBox="0 0 12 8" class="leaflet-attribution-flag">
						<path fill="#4C7BE1" d="M0 0h12v4H0z"></path>
						<path fill="#FFD500" d="M0 4h12v3H0z"></path>
						<path fill="#E0BC00" d="M0 7h12v1H0z"></path>
					</svg>
					Leaflet
				</a>
				<span aria-hidden="true">|</span>
				© <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors
			</div>
		</div>
	</div>
</div>
<?php
}

/**
 * Internal: Extract address components from a marker
 * Should only be called by internal and public functions in this file
 *
 * @param array $marker The marker data
 * @return array Array with keys: road, house_number, postcode, city
 * @access private
 */
function _get_marker_address_components($marker) {
    // Verify this is called from allowed functions
    $backtrace = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 2);
    $caller = $backtrace[1]['function'] ?? '';
    $allowed_callers = ['_render_leaflet_map', 'render_praktijk_info', 'render_combined_praktijk_list'];
    if (!in_array($caller, $allowed_callers)) {
        trigger_error('_get_marker_address_components() is an internal function and should not be called directly', E_USER_WARNING);
        return ['road' => '', 'house_number' => '', 'postcode' => '', 'city' => ''];
    }

    $components = [
        'road' => '',
        'house_number' => '',
        'postcode' => '',
        'city' => '',
    ];

    if (isset($marker['geocode'][0]['properties']['address'])) {
        $address = $marker['geocode'][0]['properties']['address'];
        $components['road'] = $address['road'] ?? '';
        $components['house_number'] = $address['house_number'] ?? '';
        $components['postcode'] = $address['postcode'] ?? '';
        $components['city'] = (!empty($address['city']) ? $address['city'] :
                              (!empty($address['town']) ? $address['town'] :
                              (!empty($address['village']) ? $address['village'] :
                              (!empty($address['municipality']) ? $address['municipality'] : ''))));
    }

    return $components;
}

/**
 * Render map from full praktijk post fields array
 *
 * @param array $praktijk_fields Full array from get_fields()
 * @param int $post_id Optional post ID to get the praktijk name (defaults to current post)
 * @return void
 */
function render_praktijk_map($praktijk_fields, $post_id = null) {
    if (empty($praktijk_fields['praktijk_coordinaten'])) {
        return;
    }

    // Use current post ID if not provided
    if ($post_id === null) {
        $post_id = get_the_ID();
    }

    // Get praktijk name from post title
    $praktijk_name = $post_id ? get_the_title($post_id) : '';

    _render_leaflet_map($praktijk_fields['praktijk_coordinaten'], $praktijk_name);
}

/**
 * Collect map markers from a praktijk for combined archive map
 * Outputs a hidden data element that JavaScript can collect
 *
 * @param array $praktijk_fields Full array from get_fields()
 * @param int $post_id Post ID to get the praktijk name
 * @return void
 */
function output_praktijk_markers_data($praktijk_fields, $post_id = null) {
    if (empty($praktijk_fields['praktijk_coordinaten']['markers'])) {
        return;
    }

    // Use current post ID if not provided
    if ($post_id === null) {
        $post_id = get_the_ID();
    }

    $praktijk_name = $post_id ? get_the_title($post_id) : '';
    $markers = $praktijk_fields['praktijk_coordinaten']['markers'];

    // Add praktijk name to each marker
    foreach ($markers as &$marker) {
        $marker['_praktijk_name'] = $praktijk_name;
        $marker['_praktijk_id'] = $post_id;
    }

    // Output as hidden data element
    echo '<div class="praktijk-markers-data" data-markers="' . esc_attr(json_encode($markers)) . '"></div>';
}

/**
 * Render combined map container that will be populated by JavaScript
 * Place this where you want the combined map to appear in your archive template
 *
 * @return void
 */
function render_combined_praktijk_map_container() {
    if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
        error_log( "Starting function render_combined_praktijk_map_container() from " . __FILE__ );
    };
    ?>
    <div id="combined-praktijk-map-container">
        <!-- Map will be rendered here by JavaScript -->
    </div>
    <?php
}

/**
 * Render a semantically correct list of all praktijken
 * Displays each praktijk as a list item with link to its page
 * Distinguishes between praktijken with single vs multiple locations
 *
 * @return void
 */
function render_combined_praktijk_list() {
    if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
        error_log( "Starting function render_combined_praktijk_list() from " . __FILE__ );
    };

    $query = new WP_Query(array(
        'post_type' => 'praktijken',
        'posts_per_page' => -1,
        'orderby' => 'title',
        'order' => 'ASC'
    ));

    if (!$query->have_posts()) {
        if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
            error_log( "No praktijken found for list" );
        }
        return;
    }

    echo '<div class="praktijken-list-container">';
    echo '<ul class="praktijken-list">';

    while ($query->have_posts()) {
        $query->the_post();
        $post_id = get_the_ID();
        $post_url = get_permalink($post_id);
        $praktijk_name = get_the_title($post_id);
        $praktijk_fields = get_fields();

        $markers = $praktijk_fields['praktijk_coordinaten']['markers'] ?? [];
        $location_count = count($markers);
        $has_multiple_locations = $location_count > 1;

        // Extract city/cities from markers
        $cities = [];
        foreach ($markers as $marker) {
            $address = _get_marker_address_components($marker);
            if (!empty($address['city'])) {
                $cities[] = $address['city'];
            }
        }
        $cities = array_unique($cities); // Remove duplicates
        $city_display = !empty($cities) ? implode(', ', $cities) : '';

        // Build CSS class for styling
        $item_class = 'praktijk-list-item';
        if ($has_multiple_locations) {
            $item_class .= ' praktijk-multiple-locations';
        } else {
            $item_class .= ' praktijk-single-location';
        }

        echo '<li class="' . esc_attr($item_class) . '" data-location-count="' . esc_attr($location_count) . '">';
        echo '<a href="' . esc_url($post_url) . '" class="praktijk-link">';
        echo '<span class="praktijk-name">' . esc_html($praktijk_name) . '</span>';

        // Display location information
        if ($city_display) {
            if ($has_multiple_locations) {
                // Multiple locations: (X lokaties in City1, City2)
                echo ' <span class="praktijk-location-info" aria-label="' . esc_attr($location_count . ' locaties') . '">';
                echo '(' . esc_html($location_count) . ' lokaties in ' . esc_html($city_display) . ')';
                echo '</span>';
            } else {
                // Single location: (City)
                echo ' <span class="praktijk-city">(' . esc_html($city_display) . ')</span>';
            }
        }

        echo '</a>';
        echo '</li>';
    }

    echo '</ul>';
    echo '</div>';

    wp_reset_postdata();
}

/**
 * Render complete praktijk information with smart layout
 * Handles single vs multiple locations with appropriate headers and data distribution
 *
 * @param array $praktijk_fields Full array from get_fields()
 * @return void
 */
function render_praktijk_info($praktijk_fields) {
    $markers = $praktijk_fields['praktijk_coordinaten']['markers'] ?? [];
    $extra_locations = $praktijk_fields['praktijk_extra'] ?? [];
    $has_multiple_locations = count($markers) > 1;

    // Build location-specific contact info from extra fields
    $location_contacts = [];
    foreach ($extra_locations as $extra) {
        $location_name = $extra['praktijk_extra_lokatienaam'] ?? '';
        if ($location_name) {
            $location_contacts[$location_name] = [
                'phone' => $extra['praktijk_extra_telefoonnummer'] ?? '',
                'email' => $extra['praktijk_extra_email'] ?? '',
            ];
        }
    }

    // Get main contact info
    $main_email = $praktijk_fields['praktijk_email'] ?? '';
    $main_phone = $praktijk_fields['praktijk_telefoonnummer'] ?? '';
    $main_url = $praktijk_fields['praktijk_url'] ?? '';

    if (!$has_multiple_locations) {
        // Single location: show everything together
        echo '<h3>Praktijkgegevens</h3>';
        echo '<div class="praktijk-info-single">';

        // Address from first marker
        if (!empty($markers[0])) {
            $address = _get_marker_address_components($markers[0]);
            if ($address['road'] || $address['house_number']) {
                echo '<div class="praktijk-address">';
                echo esc_html(trim($address['road'] . ' ' . $address['house_number'])) . '<br>';
                if ($address['postcode'] || $address['city']) {
                    echo esc_html(trim($address['postcode'] . '  ' . $address['city'])) . '<br>';
                }
                echo '</div>';
            }
        }

        // Contact info
        echo '<div class="praktijk-contact">';
        if ($main_phone) {
            echo '<div class="praktijk-phone">';
            echo '<i class="fa-solid fa-phone"></i> ' . esc_html($main_phone) . '<br>';
            echo '</div>';
        }
        if ($main_email) {
            echo '<div class="praktijk-email">';
            echo '<i class="fa-solid fa-at"></i> ' . esc_html($main_email) . '<br>';
            echo '</div>';
        }
        if ($main_url) {
            echo '<div class="praktijk-url">';
            echo '<i class="fa-solid fa-earth-europe"></i> <a href="' . esc_url($main_url) . '" target="_blank">' . esc_html($main_url) . '</a><br>';
            echo '</div>';
        }
        echo '</div>';
        echo '</div>';

    } else {
        // Multiple locations: separate general and location-specific info
        echo '<h3>Algemene praktijkgegevens</h3>';
        echo '<div class="praktijk-info-general">';

        // Always show main phone/email if they exist
        if ($main_phone) {
            echo '<div class="praktijk-phone">';
            echo '<i class="fa-solid fa-phone"></i> ' . esc_html($main_phone) . '<br>';
            echo '</div>';
        }
        if ($main_email) {
            echo '<div class="praktijk-email">';
            echo '<i class="fa-solid fa-at"></i> ' . esc_html($main_email) . '<br>';
            echo '</div>';
        }
        if ($main_url) {
            echo '<div class="praktijk-url">';
            echo '<i class="fa-solid fa-earth-europe"></i> <a href="' . esc_url($main_url) . '" target="_blank">' . esc_html($main_url) . '</a><br>';
            echo '</div>';
        }
        echo '</div>';

        // Location-specific info
        echo '<h4>Locaties</h4>';
        echo '<div class="praktijk-locations">';

        foreach ($markers as $marker) {
            $location_label = $marker['label'] ?? 'Locatie';
            $address = _get_marker_address_components($marker);

            echo '<div class="praktijk-location-item">';
            echo '<h5>' . esc_html($location_label) . '</h5>';

            // Address
            if ($address['road'] || $address['house_number']) {
                echo '<div class="location-address">';
                echo esc_html(trim($address['road'] . ' ' . $address['house_number'])) . '<br>';
                if ($address['postcode'] || $address['city']) {
                    echo esc_html(trim($address['postcode'] . '  ' . $address['city'])) . '<br>';
                }
                echo '</div>';
            }

            // Location-specific contact info if exists
            if (isset($location_contacts[$location_label])) {
                $contact = $location_contacts[$location_label];
                if (!empty($contact['phone'])) {
                    echo '<div class="location-phone">';
                    echo '<i class="fa-solid fa-phone"></i> ' . esc_html($contact['phone']) . '<br>';
                    echo '</div>';
                }
                if (!empty($contact['email'])) {
                    echo '<div class="location-email">';
                    echo '<i class="fa-solid fa-at"></i> ' . esc_html($contact['email']) . '<br>';
                    echo '</div>';
                }
            }

            echo '</div>';
        }

        echo '</div>';
    }
}

/**
 * Example usage:
 *
 * // Single praktijk page:
 * $praktijk_fields = get_fields();
 * render_praktijk_info($praktijk_fields);
 * render_praktijk_map($praktijk_fields);
 *
 * // Archive pattern - in each post template block:
 * // Call this in each post within the query loop
 * $praktijk_fields = get_fields();
 * output_praktijk_markers_data($praktijk_fields);
 *
 * // Archive pattern - after the query loop:
 * // Place this where you want the combined map
 * render_combined_praktijk_map_container();
 *
 * // Archive pattern - list all praktijken:
 * // Place this where you want the list of praktijken
 * render_combined_praktijk_list();
 */
