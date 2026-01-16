(function() {
	console.log('ACF OSM Autofit: Script loaded');

	// Listen for the acf-osm-map-created event (DOM event, not ACF action)
	document.addEventListener('acf-osm-map-created', function(event) {
		console.log('ACF OSM Autofit: acf-osm-map-created event triggered', event);

		var mapContainer = event.target;
		var map = event.detail.map;
		var L = event.detail.L;

		console.log('ACF OSM Autofit: Map container:', mapContainer);
		console.log('ACF OSM Autofit: Map object:', map);

		var shouldAutoFit = mapContainer.getAttribute('data-auto-fit-bounds') === '1';
		console.log('ACF OSM Autofit: data-auto-fit-bounds =', mapContainer.getAttribute('data-auto-fit-bounds'), 'shouldAutoFit =', shouldAutoFit);

		if (shouldAutoFit) {
			var markers = [];

			// Collect all markers from the map
			map.eachLayer(function(layer) {
				if (layer instanceof L.Marker) {
					markers.push(layer);
					console.log('ACF OSM Autofit: Found marker at', layer.getLatLng());
				}
			});

			console.log('ACF OSM Autofit: Total markers found:', markers.length);

			if (markers.length > 1) {
				var bounds = new L.LatLngBounds();
				markers.forEach(function(marker) {
					bounds.extend(marker.getLatLng());
				});
				console.log('ACF OSM Autofit: Fitting bounds', bounds);
				map.fitBounds(bounds, { padding: [50, 50] });
			} else {
				console.log('ACF OSM Autofit: Not enough markers to auto-fit (need > 1)');
			}
		} else {
			console.log('ACF OSM Autofit: Auto-fit disabled for this map');
		}
	});

	console.log('ACF OSM Autofit: Event listener registered for acf-osm-map-created');
})();
