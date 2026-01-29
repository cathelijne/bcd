<?php
/**
 * Protocol rendering functions
 */

/**
 * Render protocol information for a single protocol
 */
function render_protocol_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $versie = get_field('protocol_versie', $post_id);
    $datum = get_field('protocol_datum', $post_id);
    $type = get_field('protocol_type', $post_id);
    $beschrijving = get_field('protocol_beschrijving', $post_id);
    $bestand = get_field('protocol_bestand', $post_id);
    $verantwoordelijke = get_field('protocol_verantwoordelijke', $post_id);

    ob_start();
    ?>
    <div class="protocol-info">
        <div class="protocol-meta">
            <?php if ($versie): ?>
                <div class="protocol-meta-item">
                    <strong>Versie:</strong> <?php echo esc_html($versie); ?>
                </div>
            <?php endif; ?>

            <?php if ($datum): ?>
                <div class="protocol-meta-item">
                    <strong>Datum:</strong> <?php echo esc_html(date('d-m-Y', strtotime($datum))); ?>
                </div>
            <?php endif; ?>

            <?php if ($type): ?>
                <div class="protocol-meta-item">
                    <strong>Type:</strong> <?php echo esc_html($type); ?>
                </div>
            <?php endif; ?>

            <?php if ($verantwoordelijke): ?>
                <div class="protocol-meta-item">
                    <strong>Verantwoordelijke:</strong> <?php echo esc_html(get_the_author_meta('display_name', $verantwoordelijke)); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($beschrijving): ?>
            <div class="protocol-beschrijving">
                <h3>Beschrijving</h3>
                <?php echo wpautop(esc_html($beschrijving)); ?>
            </div>
        <?php endif; ?>

        <?php if ($bestand): ?>
            <div class="protocol-download">
                <a href="<?php echo esc_url($bestand['url']); ?>" class="button protocol-download-button" download>
                    <span class="dashicons dashicons-download"></span>
                    Download Protocol (<?php echo esc_html(size_format($bestand['filesize'])); ?>)
                </a>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

/**
 * Render a list of all protocols grouped by type
 */
function render_protocol_list() {
    $args = array(
        'post_type' => 'protocol',
        'posts_per_page' => -1,
        'orderby' => 'title',
        'order' => 'ASC',
    );

    $protocols = get_posts($args);

    if (empty($protocols)) {
        return '<p>Geen protocollen gevonden.</p>';
    }

    // Group protocols by type
    $grouped_protocols = array();
    foreach ($protocols as $protocol) {
        $type = get_field('protocol_type', $protocol->ID);
        if (!$type) {
            $type = 'Overig';
        }
        if (!isset($grouped_protocols[$type])) {
            $grouped_protocols[$type] = array();
        }
        $grouped_protocols[$type][] = $protocol;
    }

    // Sort groups by type name
    ksort($grouped_protocols);

    ob_start();
    ?>
    <div class="protocol-list">
        <?php foreach ($grouped_protocols as $type => $protocols): ?>
            <div class="protocol-group">
                <h2><?php echo esc_html(ucfirst($type)); ?></h2>
                <table class="protocol-table">
                    <thead>
                        <tr>
                            <th>Protocol</th>
                            <th>Versie</th>
                            <th>Datum</th>
                            <th>Document</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php foreach ($protocols as $protocol):
                            $versie = get_field('protocol_versie', $protocol->ID);
                            $datum = get_field('protocol_datum', $protocol->ID);
                            $bestand = get_field('protocol_bestand', $protocol->ID);
                        ?>
                            <tr>
                                <td>
                                    <a href="<?php echo get_permalink($protocol->ID); ?>">
                                        <?php echo esc_html($protocol->post_title); ?>
                                    </a>
                                </td>
                                <td><?php echo $versie ? esc_html($versie) : '-'; ?></td>
                                <td><?php echo $datum ? esc_html(date('d-m-Y', strtotime($datum))) : '-'; ?></td>
                                <td>
                                    <?php if ($bestand): ?>
                                        <a href="<?php echo esc_url($bestand['url']); ?>" class="protocol-download-link" download>
                                            <span class="dashicons dashicons-download"></span>
                                            PDF
                                        </a>
                                    <?php else: ?>
                                        -
                                    <?php endif; ?>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
        <?php endforeach; ?>
    </div>
    <?php
    return ob_get_clean();
}
