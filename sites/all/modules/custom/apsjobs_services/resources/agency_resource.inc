<?php

function apsjobs_agency_resource_definition() {
  $definition['agency'] = array(
    'operations' => array(
      'retrieve' => array(
        'help' => 'Retrieve a agency',
        'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/node_resource'),
        'callback' => 'apsjobs_services_agency_resource_retrieve',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the agency to retrieve',
          ),
        ),
        'access callback' => 'apsjobs_services_agency_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'index' => array(
        'help' => 'List all agencies',
        'file' => array('type' => 'inc', 'module' => 'apsjobs_services', 'name' => 'resources/agency_resource'),
        'callback' => 'apsjobs_services_agency_resource_index',
        'args' => array(
        ),
        'access callback' => 'apsjobs_services_agency_resource_access',
        'access arguments' => array('access content'),
      ),
    ),
  );

	return $definition;
}

function apsjobs_services_agency_resource_access($op = 'view', $args = array()) {
  // Adds backwards compatability with regression fixed in #1083242
  if (isset($args[0])) {
    $args[0] = _services_access_value($args[0], 'agency');
  }
  if (user_is_logged_in())
    return true;
  return false;
}


function apsjobs_services_agency_resource_retrieve($nid) {
  $agencies = apsjobs_services_agency_resource_index();
  $aids = array_keys($agencies);
  if (in_array($nid, $aids)) {
    $node = node_load($nid);
    if ($node) {
      $uri = entity_uri('node', $node);
      $node->path = url($uri['path'], array('absolute' => TRUE));
      unset($node->uri);
    }
    //Lets check field_permissions
    $node = services_field_permissions_clean('view', 'node', $node);
    return $node;
  }
  else {
    return services_error(t('Cannot get this agency'), 403);
  }
}

function apsjobs_services_agency_resource_index() {
  global $user;
  // get agency id of user
  $agency_id = db_select('field_data_field_agency_register', 'far')
    ->fields('far', array('field_agency_register_target_id'))
    ->condition('entity_id', $user->uid, '=')
    ->execute()
    ->fetchAssoc();

  $query = db_select('node')
    ->fields('node', array('nid', 'title'))
    ->condition('type', 'aps_agencies');

  if (!empty($agency_id)) {
    $query = $query->condition('nid', $agency_id, 'IN');
  }

  $result = $query->execute()->fetchAll();

  $agencies = array();
  foreach ($result as $value) {
    $agencies[$value->nid] = array(
      'id' => $value->nid,
      'title' => $value->title,
    );
  }
  if (!empty($agency_id)) {
    $agency = node_load($agency_id['field_agency_register_target_id']);
    if (!empty($agency->field_managed_agencies)) {
      $managed_agencies_arr = $agency->field_managed_agencies[LANGUAGE_NONE];
      foreach ($managed_agencies_arr as $item) {
        $aid = $item['target_id'];
        $m_agency = node_load($aid);
        $agencies[$m_agency->nid] = array(
          'id' => $m_agency->nid,
          'title' => $m_agency->title,
        );
      }
    }
  }
  $results = array(
    'agencies' => $agencies,
    'isSuccess' => true,
    'amount' => count($agencies),
  );
  return $results;
}