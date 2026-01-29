(function() {
	// Listen for the acf-osm-map-created event (DOM event, not ACF action)
	document.addEventListener('acf-osm-map-created', function(event) {
		var mapContainer = event.target;
		var map = event.detail.map;
		var L = event.detail.L;

		// Auto-fit by default when markers exist (ignoring data-auto-fit-bounds attribute)
		var markers = [];

		// Collect all markers from the map
		map.eachLayer(function(layer) {
			if (layer instanceof L.Marker) {
				markers.push(layer);
			}
		});

		if (markers.length > 1) {
			var bounds = new L.LatLngBounds();
			markers.forEach(function(marker) {
				bounds.extend(marker.getLatLng());
			});
			map.fitBounds(bounds, { padding: [50, 50] });
		} else if (markers.length === 1) {
			// For a single marker, center on it with a reasonable zoom level
			var markerLatLng = markers[0].getLatLng();
			map.setView(markerLatLng, 15); // Zoom level 15 is good for street-level view
		}
	});
})();
