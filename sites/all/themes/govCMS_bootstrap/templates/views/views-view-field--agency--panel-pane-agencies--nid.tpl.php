<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
  $agency_id = $output;
  $agency = node_load($agency_id);
  $agency_ref = $agency->field_agency_ref[LANGUAGE_NONE][0]['tid'];

  $query = db_select('node', 'n');
  $query->join('field_data_field_agency', 'f', 'n.nid = f.entity_id AND f.field_agency_target_id = :aid', array(':aid' => $agency_id));
  $query->fields('n', array('nid'));
  $query->condition('n.status', 1, '=');
  $result = count($query->execute()->fetchAll());
?>
<a href="/search-jobs?agency_ref=<?php print $agency_ref; ?>">View <?php print $result; ?> jobs</a>

