<?php
/**
 * Symposium rendering functions
 */

function render_symposium_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $datum = get_field('symposium_datum', $post_id);
    $tijd = get_field('symposium_tijd', $post_id);
    $locatie = get_field('symposium_locatie', $post_id);
    $organisator = get_field('symposium_organisator', $post_id);
    $beschrijving = get_field('symposium_beschrijving', $post_id);
    $programma = get_field('symposium_programma', $post_id);
    $inschrijflink = get_field('symposium_inschrijflink', $post_id);
    $documenten = get_field('symposium_documenten', $post_id);

    ob_start();
    ?>
    <div class="symposium-info">
        <div class="symposium-meta">
            <?php if ($datum): ?>
                <div class="symposium-meta-item">
                    <strong>Datum:</strong> <?php echo esc_html(date('d-m-Y', strtotime($datum))); ?>
                </div>
            <?php endif; ?>

            <?php if ($tijd): ?>
                <div class="symposium-meta-item">
                    <strong>Tijd:</strong> <?php echo esc_html($tijd); ?>
                </div>
            <?php endif; ?>

            <?php if ($locatie): ?>
                <div class="symposium-meta-item">
                    <strong>Locatie:</strong> <?php echo esc_html($locatie); ?>
                </div>
            <?php endif; ?>

            <?php if ($organisator): ?>
                <div class="symposium-meta-item">
                    <strong>Organisator:</strong> <?php echo esc_html($organisator); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($beschrijving): ?>
            <div class="symposium-beschrijving">
                <h3>Beschrijving</h3>
                <?php echo wpautop(esc_html($beschrijving)); ?>
            </div>
        <?php endif; ?>

        <?php if ($programma): ?>
            <div class="symposium-programma">
                <h3>Programma</h3>
                <?php echo $programma; ?>
            </div>
        <?php endif; ?>

        <?php if ($documenten): ?>
            <div class="symposium-documenten">
                <h3>Documenten</h3>
                <ul>
                    <?php foreach ($documenten as $doc): ?>
                        <li>
                            <a href="<?php echo esc_url($doc['bestand']['url']); ?>" download>
                                <?php echo esc_html($doc['naam']); ?>
                            </a>
                        </li>
                    <?php endforeach; ?>
                </ul>
            </div>
        <?php endif; ?>

        <?php if ($inschrijflink): ?>
            <div class="symposium-inschrijven">
                <a href="<?php echo esc_url($inschrijflink); ?>" class="button symposium-inschrijf-button" target="_blank">
                    Inschrijven
                </a>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

function render_symposium_list() {
    $args = array(
        'post_type' => 'symposium',
        'posts_per_page' => -1,
        'orderby' => 'meta_value',
        'meta_key' => 'symposium_datum',
        'order' => 'DESC',
    );

    $symposia = get_posts($args);

    if (empty($symposia)) {
        return '<p>Geen symposia gevonden.</p>';
    }

    ob_start();
    ?>
    <div class="symposium-list">
        <table class="symposium-table">
            <thead>
                <tr>
                    <th>Symposium</th>
                    <th>Datum</th>
                    <th>Locatie</th>
                    <th>Inschrijven</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($symposia as $symposium):
                    $datum = get_field('symposium_datum', $symposium->ID);
                    $locatie = get_field('symposium_locatie', $symposium->ID);
                    $inschrijflink = get_field('symposium_inschrijflink', $symposium->ID);
                ?>
                    <tr>
                        <td>
                            <a href="<?php echo get_permalink($symposium->ID); ?>">
                                <?php echo esc_html($symposium->post_title); ?>
                            </a>
                        </td>
                        <td><?php echo $datum ? esc_html(date('d-m-Y', strtotime($datum))) : '-'; ?></td>
                        <td><?php echo $locatie ? esc_html($locatie) : '-'; ?></td>
                        <td>
                            <?php if ($inschrijflink): ?>
                                <a href="<?php echo esc_url($inschrijflink); ?>" target="_blank">Inschrijven</a>
                            <?php else: ?>
                                -
                            <?php endif; ?>
                        </td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
    </div>
    <?php
    return ob_get_clean();
}
