<?php
/**
 * Title: The special query loop for the praktijklijst
 * Slug: BestCare/pattern-praktijken-query-loop
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
  <div class="wp-block-row" style="padding-top:var(--wp--preset--spacing--60);padding-bottom:var(--wp--preset--spacing--60)">
  <?php
  $query = new WP_Query(array(
    'post_type' => 'praktijken',
    'posts_per_page' => -1,
    'orderby' => 'title',
    'order' => 'DESC'
  ));

  if (defined('WP_DEBUG') && WP_DEBUG) {
    error_log('Query found ' . $query->found_posts . ' praktijk posts');
  }

  if ($query->have_posts()) {
    while ($query->have_posts()) {
      $query->the_post();
      $praktijk_fields = get_fields();

      if (defined('WP_DEBUG') && WP_DEBUG) {
        error_log('Processing post: ' . get_the_title() . ' (ID: ' . get_the_ID() . ')');
        error_log('Has markers: ' . (!empty($praktijk_fields['praktijk_coordinaten']['markers']) ? 'yes' : 'no'));
      }

      output_praktijk_markers_data($praktijk_fields);
    }
    wp_reset_postdata();
  } else {
    if (defined('WP_DEBUG') && WP_DEBUG) {
      error_log('No praktijk posts found');
    }
    echo '<!-- No praktijk posts found -->';
  }
  ?>
	<!-- wp:acf/praktijklijst /-->
  </div>
  <!-- /wp:group -->
</main>
<!-- wp:template-part {"slug":"footer","theme":"BestCare"} /-->
