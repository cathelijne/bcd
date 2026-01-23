<?php
/**
 * Title: A single colleague (author) template
 * Slug: BestCare/pattern-collega
 * Inserter: true
 *
 * @package WordPress
 * @subpackage BestCare
 * @since BestCare 0.1
 */

if ( defined( 'WP_DEBUG' ) && WP_DEBUG ) {
  error_log( "Loading" . __FILE__ );
};
?>

<!-- wp:template-part {"slug":"header","theme":"BestCare"} /-->
<main class="wp-block-group" style="margin-top:var(--wp--preset--spacing--60)">
  <!-- wp:group {"align":"full","style":{"spacing":{"padding":{"top":"var:preset|spacing|60","bottom":"var:preset|spacing|60"}}},"layout":{"type":"constrained"}} -->
  <div class="wp-block-group alignfull" style="padding-top:var(--wp--preset--spacing--60);padding-bottom:var(--wp--preset--spacing--60)">
    <?php
    $author = get_queried_object();
    if ($author && isset($author->display_name)) {
      echo '<h1 class="wp-block-post-title">' . esc_html($author->display_name) . '</h1>';
    }
    ?>
    <!-- wp:post-content {"align":"full","layout":{"type":"constrained"}} /-->
    <!-- wp:group {"style":{"spacing":{"padding":{"top":"var:preset|spacing|60","bottom":"var:preset|spacing|60"}}},"layout":{"type":"constrained"}} -->
      <div class="wp-block-group" style="padding-top:var(--wp--preset--spacing--60);padding-bottom:var(--wp--preset--spacing--60)">
        <!-- wp:columns {"style":{"spacing":{"blockGap":{"top":"var:preset|spacing|40","left":"var:preset|spacing|40"}}}} -->
        <div class="wp-block-columns">
          <!-- wp:column {"width":"33.33%"} -->
          <div class="wp-block-column" style="flex-basis:33.33%">
            <!-- wp:avatar {"size":150,"isLink":false,"align":"center"} /-->
          </div>
          <!-- /wp:column -->

          <!-- wp:column {"width":"66.66%"} -->
          <div class="wp-block-column" style="flex-basis:66.66%">
            <!-- wp:acf/collega /-->
          </div>
          <!-- /wp:column -->
        </div>
        <!-- /wp:columns -->
      </div>
      <!-- /wp:group -->
  </div>
  <!-- /wp:group -->
</main>
<!-- wp:template-part {"slug":"footer","theme":"BestCare"} /-->
