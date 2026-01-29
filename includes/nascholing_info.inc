<?php
/**
 * Nascholing rendering functions
 */

function render_nascholing_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $datum = get_field('nascholing_datum', $post_id);
    $tijd = get_field('nascholing_tijd', $post_id);
    $docent = get_field('nascholing_docent', $post_id);
    $locatie = get_field('nascholing_locatie', $post_id);
    $type = get_field('nascholing_type', $post_id);
    $beschrijving = get_field('nascholing_beschrijving', $post_id);
    $accreditatiepunten = get_field('nascholing_accreditatiepunten', $post_id);
    $inschrijflink = get_field('nascholing_inschrijflink', $post_id);
    $materiaal = get_field('nascholing_materiaal', $post_id);

    ob_start();
    ?>
    <div class="nascholing-info">
        <div class="nascholing-meta">
            <?php if ($datum): ?>
                <div class="nascholing-meta-item">
                    <strong>Datum:</strong> <?php echo esc_html(date('d-m-Y', strtotime($datum))); ?>
                </div>
            <?php endif; ?>

            <?php if ($tijd): ?>
                <div class="nascholing-meta-item">
                    <strong>Tijd:</strong> <?php echo esc_html($tijd); ?>
                </div>
            <?php endif; ?>

            <?php if ($docent): ?>
                <div class="nascholing-meta-item">
                    <strong>Docent:</strong> <?php echo esc_html($docent); ?>
                </div>
            <?php endif; ?>

            <?php if ($locatie): ?>
                <div class="nascholing-meta-item">
                    <strong>Locatie:</strong> <?php echo esc_html($locatie); ?>
                </div>
            <?php endif; ?>

            <?php if ($type): ?>
                <div class="nascholing-meta-item">
                    <strong>Type:</strong> <?php echo esc_html(ucfirst($type)); ?>
                </div>
            <?php endif; ?>

            <?php if ($accreditatiepunten): ?>
                <div class="nascholing-meta-item nascholing-accreditatie">
                    <strong>Accreditatiepunten:</strong> <?php echo esc_html($accreditatiepunten); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($beschrijving): ?>
            <div class="nascholing-beschrijving">
                <h3>Beschrijving</h3>
                <?php echo $beschrijving; ?>
            </div>
        <?php endif; ?>

        <?php if ($materiaal): ?>
            <div class="nascholing-download">
                <a href="<?php echo esc_url($materiaal['url']); ?>" class="button nascholing-download-button" download>
                    <span class="dashicons dashicons-download"></span>
                    Download Cursusmateriaal
                </a>
            </div>
        <?php endif; ?>

        <?php if ($inschrijflink): ?>
            <div class="nascholing-inschrijven">
                <a href="<?php echo esc_url($inschrijflink); ?>" class="button nascholing-inschrijf-button" target="_blank">
                    Inschrijven
                </a>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

function render_nascholing_list() {
    $args = array(
        'post_type' => 'nascholing',
        'posts_per_page' => -1,
        'orderby' => 'meta_value',
        'meta_key' => 'nascholing_datum',
        'order' => 'DESC',
    );

    $nascholingen = get_posts($args);

    if (empty($nascholingen)) {
        return '<p>Geen nascholingen gevonden.</p>';
    }

    // Group by type
    $grouped = array();
    foreach ($nascholingen as $nascholing) {
        $type = get_field('nascholing_type', $nascholing->ID);
        if (!$type) $type = 'andere';
        if (!isset($grouped[$type])) {
            $grouped[$type] = array();
        }
        $grouped[$type][] = $nascholing;
    }

    ksort($grouped);

    ob_start();
    ?>
    <div class="nascholing-list">
        <?php foreach ($grouped as $type => $nascholingen): ?>
            <div class="nascholing-group">
                <h2><?php echo esc_html(ucfirst($type)); ?></h2>
                <table class="nascholing-table">
                    <thead>
                        <tr>
                            <th>Nascholing</th>
                            <th>Datum</th>
                            <th>Docent</th>
                            <th>Punten</th>
                            <th>Inschrijven</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php foreach ($nascholingen as $nascholing):
                            $datum = get_field('nascholing_datum', $nascholing->ID);
                            $docent = get_field('nascholing_docent', $nascholing->ID);
                            $punten = get_field('nascholing_accreditatiepunten', $nascholing->ID);
                            $link = get_field('nascholing_inschrijflink', $nascholing->ID);
                        ?>
                            <tr>
                                <td>
                                    <a href="<?php echo get_permalink($nascholing->ID); ?>">
                                        <?php echo esc_html($nascholing->post_title); ?>
                                    </a>
                                </td>
                                <td><?php echo $datum ? esc_html(date('d-m-Y', strtotime($datum))) : '-'; ?></td>
                                <td><?php echo $docent ? esc_html($docent) : '-'; ?></td>
                                <td><?php echo $punten ? esc_html($punten) : '-'; ?></td>
                                <td>
                                    <?php if ($link): ?>
                                        <a href="<?php echo esc_url($link); ?>" target="_blank">Inschrijven</a>
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
