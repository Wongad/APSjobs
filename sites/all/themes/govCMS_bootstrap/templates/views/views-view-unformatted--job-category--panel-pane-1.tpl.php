<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif;?>
<?php foreach ($rows as $id => $row): ?>
  <?php 
    $add_classes = array();
    if($id < 16) {
      $add_classes[] = "desktop-show";
    }
    if($id < 8) {
      $add_classes[] = "tablet-show";
    }
    if($id < 2) {
      $add_classes[] = "mobile-show";
    }
    $additional_class = implode(" ", $add_classes);
  ?>
  <div<?php if ($classes_array[$id]) { print ' class="' . $classes_array[$id] . ' ' . $additional_class . '"';  } ?>>
    <?php print $row; ?>
  </div>
<?php endforeach; ?>