<?php

function apsjobs_count_agency() {

  global $user;
  if (in_array('APS Agency Approver', $user->roles)) {
    $link_awaiting = 'my-job-vacancies/unapproved-vacancies';
    $class_user_role = '';
  }
  else {
    $link_awaiting = 'my-job-vacancies/awaiting-approval';
    $class_user_role = 'au-grey';
  }
  return $items = array(
    array(
      'value' => apsjobs_count_current_job(),
      'link'  => 'my-job-vacancies',
      'title' => 'Current Jobs Vacancies>',
      'type'  => 'current_vacancy'
    ),
    array(
      'value' => apsjobs_count_awaiting_approval(),
      'link'  => $link_awaiting,
      'title' => '(Drafts) Awaiting Approval>',
      'type'  => 'awaiting_approval',
      'class' => $class_user_role
    ),
    array(
      'value' => apsjobs_count_awaiting_outcome(),
      'link'  => 'my-job-vacancies/awaiting-outcome',
      'title' => 'Jobs Awaiting Outcome>',
      'type'  => 'awaiting_outcome'
    ),
    array(
      'value' => apsjobs_count_current_gazette(),
      'link'  => 'aps-gazette',
      'title' => 'Current  Gazette>',
      'type'  => 'current_gazette'
    ),
  );
}

// count all job published of user if user is agency user
// or all job published of agency if user is agency approver
function apsjobs_count_current_job() {
  //return '6';
  global $user;

  $user_is_agency_approver = in_array('APS Agency Approver', $user->roles);
  if ($user_is_agency_approver) {
    // get agency of user
    $agency_id = db_select('field_data_field_agency_register', 'far')
    ->fields('far', array('field_agency_register_target_id'))
    ->condition('entity_id', $user->uid, '=')
    ->execute()
    ->fetchAssoc();
    if (empty($agency_id)) return 0;
    // get count all job published of agency
    $query = db_select('workbench_moderation_node_history', 'wmnh');
    $query->join('field_data_field_agency_owner', 'fdfa', 'wmnh.nid = fdfa.entity_id');
    $query->join('node', 'n', 'wmnh.nid = n.nid AND n.type = :type', array(':type' => 'aps_vacancies'));
    $query->fields('wmnh', array('nid'))
    ->condition('wmnh.is_current', 1,'=')
    ->condition('wmnh.state', 'published', '=')
    ->condition('fdfa.field_agency_owner_target_id', $agency_id, 'IN');
    $result = $query->execute()->fetchAll();
  }
  else {
    // get count all job published of user
    $query = db_select('workbench_moderation_node_history', 'wmnh');
    $query->join('node', 'n', 'wmnh.nid = n.nid AND n.type = :type', array(':type' => 'aps_vacancies'));
    $result = $query->fields('wmnh', array('nid'))
    ->condition('n.uid', $user->uid, '=')
    ->condition('wmnh.is_current', 1, '=')
    ->condition('wmnh.state', 'published', '=')
    ->execute()
    ->fetchAll();
  }

  return count($result);
}

// All vacancies create by user that are not approved if user is agency user
// All vacancies of agency that are not approved if user is agency approver
function apsjobs_count_awaiting_approval() {
  //return '5';
  global $user;
  $user_is_agency_approver = in_array('APS Agency Approver', $user->roles);
  if ($user_is_agency_approver) {
    // get agency of user
    $agency_id = db_select('field_data_field_agency_register', 'far')
    ->fields('far', array('field_agency_register_target_id'))
    ->condition('entity_id', $user->uid, '=')
    ->execute()
    ->fetchAssoc();
    if (empty($agency_id)) return 0;
    // get count all job needs_review of agency
    $query = db_select('workbench_moderation_node_history', 'wmnh');
    $query->join('field_data_field_agency_owner', 'fdfa', 'wmnh.nid = fdfa.entity_id');
    $query->join('node', 'n', 'wmnh.nid = n.nid AND n.type = :type', array(':type' => 'aps_vacancies'));
    $query->fields('wmnh', array('nid'))
    ->condition('wmnh.is_current', 1,'=')
    ->condition('wmnh.state', 'needs_review', '=')
    ->condition('fdfa.field_agency_owner_target_id', $agency_id, 'IN');
    $result = $query->execute()->fetchAll();
  }
  else {
    // get count all job needs_review of user
    $query = db_select('workbench_moderation_node_history', 'wmnh');
    $query->join('node', 'n', 'wmnh.nid = n.nid AND n.type = :type', array(':type' => 'aps_vacancies'));
    $result = $query->fields('wmnh', array('nid'))
    ->condition('n.uid', $user->uid, '=')
    ->condition('wmnh.is_current', 1, '=')
    ->condition('wmnh.state', 'needs_review', '=')
    ->execute()
    ->fetchAll();
  }

  return count($result);
}

function apsjobs_count_awaiting_outcome() {
  // global $user;
  // $query = "SELECT * FROM node n_aps_outcomes WHERE n_aps_outcomes.type = 'aps_outcomes'";
  // $result = db_query($query)->fetchAll();
  // return count($result);
  return '0';
}

function apsjobs_count_current_gazette() {
  return '0';
  global $user;
  // get agency id of user
  $agency_id = db_select('field_data_field_agency_register', 'far')
    ->fields('far', array('field_agency_register_target_id'))
    ->condition('entity_id', $user->uid, '=')
    ->execute()
    ->fetchAssoc();
  if (empty($agency_id)) return 0;
  // get count gazette by agency id
  $result = db_select('field_data_field_agency_name', 'fan')
    ->fields('fan', array('entity_id'))
    ->condition('field_agency_name_target_id', $agency_id, 'IN')
    ->execute()
    ->fetchAll();
  return count($result);
}
