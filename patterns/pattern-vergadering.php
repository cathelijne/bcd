<?php
/**
 * Title: A single vergadering template with date, agenda, notulen and attendees
 * Slug: BestCare/pattern-vergadering
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
  <!-- wp:group { "align":"full", "style":{ "spacing":{ "padding":{ "top":"var:preset|spacing|60", "bottom":"var:preset|spacing|60" } } }, "layout":{ "type":"constrained" } } -->
  <div class="wp-block-group alignfull" style="padding-top:var(--wp--preset--spacing--60);padding-bottom:var(--wp--preset--spacing--60)">
    <!-- wp:post-title /-->
    <!-- wp:post-content {"align":"full","layout":{"type":"constrained"}} /-->
    <!-- wp:group {"style":{"spacing":{"padding":{"top":"var:preset|spacing|60","bottom":"var:preset|spacing|60"}}},"layout":{"type":"constrained"}} -->
      <div class="wp-block-group" style="padding-top:var(--wp--preset--spacing--60);padding-bottom:var(--wp--preset--spacing--60)">
        <!-- wp:acf/vergadering /-->
      </div>
      <!-- /wp:group -->
  </div>
<!-- /wp:group -->
</main>
<!-- wp:template-part {"slug":"footer","theme":"BestCare"} /-->
