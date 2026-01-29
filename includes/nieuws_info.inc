<?php
/**
 * Nieuws rendering functions
 */

function render_nieuws_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $datum = get_field('nieuws_datum', $post_id);
    $samenvatting = get_field('nieuws_samenvatting', $post_id);
    $inhoud = get_field('nieuws_inhoud', $post_id);
    $auteur = get_field('nieuws_auteur', $post_id);
    $uitgelicht = get_field('nieuws_uitgelicht', $post_id);
    $bijlagen = get_field('nieuws_bijlagen', $post_id);

    ob_start();
    ?>
    <div class="nieuws-info <?php echo $uitgelicht ? 'nieuws-uitgelicht' : ''; ?>">
        <div class="nieuws-meta">
            <?php if ($datum): ?>
                <div class="nieuws-meta-item">
                    <strong>Gepubliceerd:</strong> <?php echo esc_html(date('d-m-Y', strtotime($datum))); ?>
                </div>
            <?php endif; ?>

            <?php if ($auteur): ?>
                <div class="nieuws-meta-item">
                    <strong>Auteur:</strong> <?php echo esc_html(get_the_author_meta('display_name', $auteur)); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($samenvatting): ?>
            <div class="nieuws-samenvatting">
                <?php echo wpautop(esc_html($samenvatting)); ?>
            </div>
        <?php endif; ?>

        <?php if ($inhoud): ?>
            <div class="nieuws-inhoud">
                <?php echo $inhoud; ?>
            </div>
        <?php endif; ?>

        <?php if ($bijlagen): ?>
            <div class="nieuws-bijlagen">
                <h3>Bijlagen</h3>
                <ul>
                    <?php foreach ($bijlagen as $bijlage): ?>
                        <li>
                            <a href="<?php echo esc_url($bijlage['bestand']['url']); ?>" download>
                                <span class="dashicons dashicons-download"></span>
                                <?php echo esc_html($bijlage['naam']); ?>
                            </a>
                        </li>
                    <?php endforeach; ?>
                </ul>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

function render_nieuws_list() {
    $args = array(
        'post_type' => 'nieuwsbericht',
        'posts_per_page' => -1,
        'orderby' => 'meta_value',
        'meta_key' => 'nieuws_datum',
        'order' => 'DESC',
    );

    $berichten = get_posts($args);

    if (empty($berichten)) {
        return '<p>Geen nieuwsberichten gevonden.</p>';
    }

    ob_start();
    ?>
    <div class="nieuws-list">
        <?php foreach ($berichten as $bericht):
            $datum = get_field('nieuws_datum', $bericht->ID);
            $samenvatting = get_field('nieuws_samenvatting', $bericht->ID);
            $uitgelicht = get_field('nieuws_uitgelicht', $bericht->ID);
        ?>
            <article class="nieuws-item <?php echo $uitgelicht ? 'nieuws-uitgelicht' : ''; ?>">
                <?php if ($uitgelicht): ?>
                    <span class="nieuws-badge">Uitgelicht</span>
                <?php endif; ?>

                <h2>
                    <a href="<?php echo get_permalink($bericht->ID); ?>">
                        <?php echo esc_html($bericht->post_title); ?>
                    </a>
                </h2>

                <?php if ($datum): ?>
                    <div class="nieuws-datum">
                        <?php echo esc_html(date('d F Y', strtotime($datum))); ?>
                    </div>
                <?php endif; ?>

                <?php if ($samenvatting): ?>
                    <div class="nieuws-excerpt">
                        <?php echo wpautop(esc_html($samenvatting)); ?>
                    </div>
                <?php endif; ?>

                <a href="<?php echo get_permalink($bericht->ID); ?>" class="nieuws-lees-meer">
                    Lees meer →
                </a>
            </article>
        <?php endforeach; ?>
    </div>
    <?php
    return ob_get_clean();
}
