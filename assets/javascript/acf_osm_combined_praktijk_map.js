(function() {
    document.addEventListener('DOMContentLoaded', function() {
        // Collect all marker data from the page
        var allMarkers = [];
        var markerElements = document.querySelectorAll('.praktijk-markers-data');

        markerElements.forEach(function(element) {
            try {
                var markers = JSON.parse(element.getAttribute('data-markers'));
                if (Array.isArray(markers)) {
                    allMarkers = allMarkers.concat(markers);
                }
            } catch (e) {
                console.error('Error parsing marker data:', e);
            }
        });

        if (allMarkers.length === 0) {
            console.log('No markers found for combined map');
            return;
        }

        // Calculate center point
        var totalLat = 0, totalLng = 0;
        allMarkers.forEach(function(marker) {
            totalLat += parseFloat(marker.lat);
            totalLng += parseFloat(marker.lng);
        });
        var centerLat = totalLat / allMarkers.length;
        var centerLng = totalLng / allMarkers.length;

        // Create map data structure
        var mapData = {
            center_lat: centerLat,
            center_lng: centerLng,
            zoom: 10,
            markers: allMarkers,
            layers: ['OpenStreetMap.Mapnik']
        };

        // Build and inject the map HTML
        var container = document.getElementById('combined-praktijk-map-container');
        if (container) {
            var mapHtml = buildMapHtml(mapData);
            container.innerHTML = mapHtml;

            // Trigger ACF OSM initialization
            var mapElement = container.querySelector('[data-map="leaflet"]');
            if (mapElement) {
                mapElement.dispatchEvent(new CustomEvent('acf-osm-map-added', {bubbles: true}));

                // Wait for the map to be created, then trigger autofit
                var checkMapCreated = setInterval(function() {
                    if (window.L && mapElement._leaflet_map) {
                        clearInterval(checkMapCreated);
                        var mapCreatedEvent = new CustomEvent('acf-osm-map-created', {
                            bubbles: true,
                            detail: {
                                map: mapElement._leaflet_map,
                                L: window.L
                            }
                        });
                        mapElement.dispatchEvent(mapCreatedEvent);
                    }
                }, 100);
            }
        }
    });

    function buildMapHtml(mapData) {
        // Enrich marker labels
        mapData.markers.forEach(function(marker) {
            var address = extractAddress(marker);
            var parts = [];

            if (marker._praktijk_name) {
                parts.push('<strong>' + escapeHtml(marker._praktijk_name) + '</strong>');
            }
            if (marker.label) {
                parts.push('<em>' + escapeHtml(marker.label) + '</em>');
            }
            if (address.road || address.house_number) {
                parts.push(escapeHtml((address.road + ' ' + address.house_number).trim()));
            }
            if (address.postcode || address.city) {
                parts.push(escapeHtml((address.postcode + ' ' + address.city).trim()));
            }

            marker.label = parts.join('<br>');
        });

        var autoFit = mapData.markers.length > 1 ? '1' : '0';

        return '<div class="leaflet-map leaflet-container leaflet-touch leaflet-retina leaflet-fade-anim leaflet-grab leaflet-touch-drag leaflet-touch-zoom" ' +
            'data-height="400" ' +
            'data-map="leaflet" ' +
            'data-map-lng="' + mapData.center_lng + '" ' +
            'data-map-lat="' + mapData.center_lat + '" ' +
            'data-map-zoom="' + mapData.zoom + '" ' +
            'data-map-layers=\'' + JSON.stringify(mapData.layers) + '\' ' +
            'data-map-markers=\'' + JSON.stringify(mapData.markers) + '\' ' +
            'data-auto-fit-bounds="' + autoFit + '" ' +
            'style="height: 400px; position: relative;">' +
            '<div class="leaflet-pane leaflet-map-pane" style="transform: translate3d(0px, 0px, 0px);"></div>' +
            '</div>';
    }

    function extractAddress(marker) {
        var address = {road: '', house_number: '', postcode: '', city: ''};
        if (marker.geocode && marker.geocode[0] && marker.geocode[0].properties && marker.geocode[0].properties.address) {
            var addr = marker.geocode[0].properties.address;
            address.road = addr.road || '';
            address.house_number = addr.house_number || '';
            address.postcode = addr.postcode || '';
            address.city = addr.city || addr.town || addr.village || addr.municipality || '';
        }
        return address;
    }

    function escapeHtml(text) {
        var div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }
})();
