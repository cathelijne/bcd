<?php
/**
 * Lezing rendering functions
 */

function render_lezing_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $datum = get_field('lezing_datum', $post_id);
    $tijd = get_field('lezing_tijd', $post_id);
    $spreker = get_field('lezing_spreker', $post_id);
    $locatie = get_field('lezing_locatie', $post_id);
    $onderwerp = get_field('lezing_onderwerp', $post_id);
    $beschrijving = get_field('lezing_beschrijving', $post_id);
    $presentatie = get_field('lezing_presentatie', $post_id);

    ob_start();
    ?>
    <div class="lezing-info">
        <div class="lezing-meta">
            <?php if ($datum): ?>
                <div class="lezing-meta-item">
                    <strong>Datum:</strong> <?php echo esc_html(date('d-m-Y', strtotime($datum))); ?>
                </div>
            <?php endif; ?>

            <?php if ($tijd): ?>
                <div class="lezing-meta-item">
                    <strong>Tijd:</strong> <?php echo esc_html($tijd); ?>
                </div>
            <?php endif; ?>

            <?php if ($spreker): ?>
                <div class="lezing-meta-item">
                    <strong>Spreker:</strong> <?php echo esc_html($spreker); ?>
                </div>
            <?php endif; ?>

            <?php if ($locatie): ?>
                <div class="lezing-meta-item">
                    <strong>Locatie:</strong> <?php echo esc_html($locatie); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($onderwerp): ?>
            <div class="lezing-onderwerp">
                <h3>Onderwerp</h3>
                <?php echo wpautop(esc_html($onderwerp)); ?>
            </div>
        <?php endif; ?>

        <?php if ($beschrijving): ?>
            <div class="lezing-beschrijving">
                <h3>Beschrijving</h3>
                <?php echo $beschrijving; ?>
            </div>
        <?php endif; ?>

        <?php if ($presentatie): ?>
            <div class="lezing-download">
                <a href="<?php echo esc_url($presentatie['url']); ?>" class="button lezing-download-button" download>
                    <span class="dashicons dashicons-download"></span>
                    Download Presentatie
                </a>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

function render_lezing_list() {
    $args = array(
        'post_type' => 'lezing',
        'posts_per_page' => -1,
        'orderby' => 'meta_value',
        'meta_key' => 'lezing_datum',
        'order' => 'DESC',
    );

    $lezingen = get_posts($args);

    if (empty($lezingen)) {
        return '<p>Geen lezingen gevonden.</p>';
    }

    ob_start();
    ?>
    <div class="lezing-list">
        <table class="lezing-table">
            <thead>
                <tr>
                    <th>Lezing</th>
                    <th>Datum</th>
                    <th>Spreker</th>
                    <th>Locatie</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($lezingen as $lezing):
                    $datum = get_field('lezing_datum', $lezing->ID);
                    $spreker = get_field('lezing_spreker', $lezing->ID);
                    $locatie = get_field('lezing_locatie', $lezing->ID);
                ?>
                    <tr>
                        <td>
                            <a href="<?php echo get_permalink($lezing->ID); ?>">
                                <?php echo esc_html($lezing->post_title); ?>
                            </a>
                        </td>
                        <td><?php echo $datum ? esc_html(date('d-m-Y', strtotime($datum))) : '-'; ?></td>
                        <td><?php echo $spreker ? esc_html($spreker) : '-'; ?></td>
                        <td><?php echo $locatie ? esc_html($locatie) : '-'; ?></td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
    </div>
    <?php
    return ob_get_clean();
}
