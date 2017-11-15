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
global $user;
$current_user = user_load($user->uid);
if (in_array('APS Agency User', $current_user->roles) || in_array('APS Agency Approver', $current_user->roles)){
  $agency_id = $current_user->field_agency_register[LANGUAGE_NONE][0]['target_id'];
  if (isset($agency_id)) {
    $agency = node_load($agency_id);
    $agency_name = $agency->title;
  }
}

?>
<?php isset($agency_name) ? print ($agency_name . ' - Jobs') : print $output; ?>