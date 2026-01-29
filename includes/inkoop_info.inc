<?php
/**
 * Inkoop rendering functions
 */

function render_inkoop_info($post_id = null) {
    if (!$post_id) {
        $post_id = get_the_ID();
    }

    $leverancier = get_field('inkoop_leverancier', $post_id);
    $product = get_field('inkoop_product', $post_id);
    $categorie = get_field('inkoop_categorie', $post_id);
    $startdatum = get_field('inkoop_startdatum', $post_id);
    $einddatum = get_field('inkoop_einddatum', $post_id);
    $contactpersoon = get_field('inkoop_contactpersoon', $post_id);
    $telefoon = get_field('inkoop_telefoon', $post_id);
    $email = get_field('inkoop_email', $post_id);
    $beschrijving = get_field('inkoop_beschrijving', $post_id);
    $overeenkomst = get_field('inkoop_overeenkomst', $post_id);
    $verantwoordelijke = get_field('inkoop_verantwoordelijke', $post_id);

    ob_start();
    ?>
    <div class="inkoop-info">
        <div class="inkoop-meta">
            <?php if ($leverancier): ?>
                <div class="inkoop-meta-item">
                    <strong>Leverancier:</strong> <?php echo esc_html($leverancier); ?>
                </div>
            <?php endif; ?>

            <?php if ($product): ?>
                <div class="inkoop-meta-item">
                    <strong>Product/Dienst:</strong> <?php echo esc_html($product); ?>
                </div>
            <?php endif; ?>

            <?php if ($categorie): ?>
                <div class="inkoop-meta-item">
                    <strong>Categorie:</strong> <?php echo esc_html(ucfirst($categorie)); ?>
                </div>
            <?php endif; ?>

            <?php if ($startdatum || $einddatum): ?>
                <div class="inkoop-meta-item">
                    <strong>Looptijd:</strong>
                    <?php
                    if ($startdatum) echo esc_html(date('d-m-Y', strtotime($startdatum)));
                    if ($startdatum && $einddatum) echo ' - ';
                    if ($einddatum) echo esc_html(date('d-m-Y', strtotime($einddatum)));
                    ?>
                </div>
            <?php endif; ?>

            <?php if ($verantwoordelijke): ?>
                <div class="inkoop-meta-item">
                    <strong>Verantwoordelijke:</strong> <?php echo esc_html(get_the_author_meta('display_name', $verantwoordelijke)); ?>
                </div>
            <?php endif; ?>
        </div>

        <?php if ($beschrijving): ?>
            <div class="inkoop-beschrijving">
                <h3>Beschrijving</h3>
                <?php echo wpautop(esc_html($beschrijving)); ?>
            </div>
        <?php endif; ?>

        <?php if ($contactpersoon || $telefoon || $email): ?>
            <div class="inkoop-contact">
                <h3>Contactgegevens</h3>
                <?php if ($contactpersoon): ?>
                    <p><strong>Contactpersoon:</strong> <?php echo esc_html($contactpersoon); ?></p>
                <?php endif; ?>
                <?php if ($telefoon): ?>
                    <p><strong>Telefoon:</strong> <a href="tel:<?php echo esc_attr($telefoon); ?>"><?php echo esc_html($telefoon); ?></a></p>
                <?php endif; ?>
                <?php if ($email): ?>
                    <p><strong>E-mail:</strong> <a href="mailto:<?php echo esc_attr($email); ?>"><?php echo esc_html($email); ?></a></p>
                <?php endif; ?>
            </div>
        <?php endif; ?>

        <?php if ($overeenkomst): ?>
            <div class="inkoop-download">
                <a href="<?php echo esc_url($overeenkomst['url']); ?>" class="button inkoop-download-button" download>
                    <span class="dashicons dashicons-download"></span>
                    Download Overeenkomst
                </a>
            </div>
        <?php endif; ?>
    </div>
    <?php
    return ob_get_clean();
}

function render_inkoop_list() {
    $args = array(
        'post_type' => 'inkoopovereenkomst',
        'posts_per_page' => -1,
        'orderby' => 'title',
        'order' => 'ASC',
    );

    $overeenkomsten = get_posts($args);

    if (empty($overeenkomsten)) {
        return '<p>Geen inkoopovereenkomsten gevonden.</p>';
    }

    // Group by categorie
    $grouped = array();
    foreach ($overeenkomsten as $overeenkomst) {
        $categorie = get_field('inkoop_categorie', $overeenkomst->ID);
        if (!$categorie) $categorie = 'andere';
        if (!isset($grouped[$categorie])) {
            $grouped[$categorie] = array();
        }
        $grouped[$categorie][] = $overeenkomst;
    }

    ksort($grouped);

    ob_start();
    ?>
    <div class="inkoop-list">
        <?php foreach ($grouped as $categorie => $overeenkomsten): ?>
            <div class="inkoop-group">
                <h2><?php echo esc_html(ucfirst($categorie)); ?></h2>
                <table class="inkoop-table">
                    <thead>
                        <tr>
                            <th>Overeenkomst</th>
                            <th>Leverancier</th>
                            <th>Product/Dienst</th>
                            <th>Einddatum</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php foreach ($overeenkomsten as $overeenkomst):
                            $leverancier = get_field('inkoop_leverancier', $overeenkomst->ID);
                            $product = get_field('inkoop_product', $overeenkomst->ID);
                            $einddatum = get_field('inkoop_einddatum', $overeenkomst->ID);
                        ?>
                            <tr>
                                <td>
                                    <a href="<?php echo get_permalink($overeenkomst->ID); ?>">
                                        <?php echo esc_html($overeenkomst->post_title); ?>
                                    </a>
                                </td>
                                <td><?php echo $leverancier ? esc_html($leverancier) : '-'; ?></td>
                                <td><?php echo $product ? esc_html($product) : '-'; ?></td>
                                <td><?php echo $einddatum ? esc_html(date('d-m-Y', strtotime($einddatum))) : '-'; ?></td>
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
