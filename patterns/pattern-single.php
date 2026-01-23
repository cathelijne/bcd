<?php
/**
 * Title: A single template with a logo and an address block
 * Slug: BestCare/pattern-single
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
    <!-- wp:acf/allfields /-->

    <!-- wp:pattern {"slug":"BestCare/post-navigation"} /-->
    <!-- wp:pattern {"slug":"BestCare/more-posts"} /-->
  </div>
<!-- /wp:group -->
</main>
<!-- wp:template-part {"slug":"footer","theme":"BestCare"} /-->
