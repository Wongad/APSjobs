<?php
function apsjobs_vacancy_resource_definition() {
  $definition['vacancy'] = array(
    'operations' => array(
      'retrieve' => array(
        'help' => 'Retrieve a vacancy',
        'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/node_resource'),
        'callback' => 'apsjobs_services_vacancy_resource_retrieve',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the vacancy to retrieve',
          ),
        ),
        'access callback' => 'apsjobs_services_vacancy_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'index' => array(
        'help' => 'List all vacancies',
        'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/node_resource'),
        'callback' => 'apsjobs_services_vacancy_resource_index',
        'args' => array(
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get.',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
          array(
            'name' => 'parameters',
            'optional' => TRUE,
            'type' => 'array',
            'description' => 'Parameters array',
            'default value' => array(),
            'source' => array('param' => 'parameters'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('services_node_index_page_size', 20),
            'source' => array('param' => 'pagesize'),
          ),
          array(
            'name' => 'options',
            'optional' => TRUE,
            'type' => 'array',
            'description' => 'Additional query options.',
            'default value' => array(
              'orderby' => array(
                'sticky' => 'DESC',
                'created' => 'DESC'
              )
            ),
            'source' => array('param' => 'options'),
          ),
        ),
        'access arguments' => array('access content'),
      ),
      'create' => array(
        'help' => 'REST API for creating vacancy',
        'file' => array('type' => 'inc', 'module' => 'apsjobs_services', 'name' => 'resources/vacancy_resource'),
        'callback' => 'apsjobs_services_create_vacancy',
        'args' => array(
          array(
            'name' => 'node',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The vacancy data to create',
            'type' => 'array',
          ),
        ),
        'access callback' => 'apsjobs_services_vacancy_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),
      'update' => array(
        'help' => 'REST for updating vacancy',
        'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/vacancy_resource'),
        'callback' => 'apsjobs_services_update_vacancy',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'Unique identifier for this vacancy',
          ),
          array(
            'name' => 'node',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The vacancy with updated information',
            'type' => 'array',
          ),
        ),
        'access callback' => 'apsjobs_services_vacancy_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ),
      'delete' => array(
        'help' => t('Delete a vacancy'),
        'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/node_resource'),
        'callback' => 'apsjobs_services_vacancy_resource_delete',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to delete',
          ),
        ),
        'access callback' => 'apsjobs_services_vacancy_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
      ),
    ),
  );

	return $definition;
}

function apsjobs_services_vacancy_resource_access($op = 'view', $args = array()) {
  // Adds backwards compatability with regression fixed in #1083242
  if (isset($args[0])) {
    $args[0] = _services_access_value($args[0], 'vacancy');
  }

  // Make sure we have an object or this all fails, some servers can
  // mess up the types.
  if (is_array($args[0])) {
    $args[0] = (object) $args[0];
  }
  elseif (!is_array($args[0]) && !is_object($args[0])) {  //This is to determine if it is just a string happens on node/%NID
    $args[0] = (object)array('nid' => $args[0]);
  }
  if ($op != 'create' && !empty($args)) {
    $node = node_load($args[0]->nid);
  }
  elseif ($op == 'create') {
    if (isset($args[0]->type)) {
      $node = $args[0]->type;
      return node_access($op, $node);
    }
    else {
      return services_error(t('Node type is required'), 406);
    }
  }
  if (isset($node->nid) && $node->nid) {
    return node_access($op, $node);
  }
  else {
    return services_error(t('Vacancy @nid could not be found', array('@nid' => $args[0]->nid)), 404);
  }
}

function apsjobs_services_vacancy_resource_retrieve($nid) {
	$node = node_load($nid);
  
  if ($node) {
    $uri = entity_uri('node', $node);
    $node->path = url($uri['path'], array('absolute' => TRUE));
    // Unset uri as it has complete entity and this
    // cause never ending recursion in rendering.
    unset($node->uri);
  }
  //Lets check field_permissions
  $node = services_field_permissions_clean('view', 'node', $node);
  return $node;
}

function apsjobs_services_create_vacancy($node) {
  global $user;
  if (!in_array('APS Agency User', $user->roles)) {
    return services_error(t('Permission denied.'), 403);
  }
  // Adds backwards compatability with regression fixed in #1083242
  $node = _services_arg_value($node, 'node');
  if (!isset($node['name'])) {
    // Assign username to the node from $user created at auth step.
    if (isset($user->name)) {
      $current_user = user_load($user->uid);
      $node['uid'] = $user->uid;
      $node['agency_owner'] = $current_user->field_agency_register[LANGUAGE_NONE][0]['target_id'];
    }
  }
  if(!isset($node['language'])) {
    $node['language'] = LANGUAGE_NONE;
  }
  // Validate the node. If there is validation error Exception will be thrown
  // so code below won't be executed.
  apsjobs_services_vacancy_resource_validate($node);

  $vacancy_nid = apsjobs_services_vacancy_resource_save($node);
  if (!empty($vacancy_nid)) {
    $result = array(
      'nid' => $vacancy_nid,
      'status' => TRUE,
      'message' => 'Success'
    );
  }
  else {
    $result = array(
      'status' => FALSE,
      'message' => 'Fail'
    );
  }
  return $result;
}

function apsjobs_services_update_vacancy($nid, $node) {
  // Adds backwards compatability with regression fixed in #1083242
  global $user;

  $node = _services_arg_value($node, 'node');
  $node['nid'] = $nid;
  $old_node = node_load($nid);

  if (empty($old_node->nid)) {
    return services_error(t('Node @nid not found', array('@nid' => $old_node->nid)), 404);
  }
  if ($old_node->status == 1) {
    return services_error(t('This vacancy published'), 406);
  }

  $current_state = $old_node->workbench_moderation['current']->state;
  
  if (in_array('APS Agency User', $user->roles)) {
    if (!($current_state == 'draft' || $current_state == 'needs_review' || $current_state == 'rejected')) {
      return services_error(t('This vacancy cannot edit'), 406);
    }
  }
  else if (in_array('APS Agency Approver', $user->roles)) {
    if ($current_state != 'needs_review') {
      return services_error(t('This vacancy cannot edit'), 406);
    }
  }
  else {
    return services_error(t('Permission denied.'), 403);
  }
  

  // If no type is provided use the existing node type.
  if (empty($node['type'])) {
    $node['type'] = $old_node->type;
  }
  elseif ($node['type'] != $old_node->type) {
    // Node types cannot be changed once they are created.
    return services_error(t('Node type cannot be changed'), 406);
  }

  // Validate the node. If there is validation error Exception will be thrown
  // so code below won't be executed.
  apsjobs_services_vacancy_resource_validate($node);
  //for update
  $node['vacancy_nid'] = $nid;
  $vacancy_nid = apsjobs_services_vacancy_resource_save($node);

  if (!empty($vacancy_nid)) {
    $result = array(
      'nid' => $vacancy_nid,
      'status' => TRUE,
      'message' => 'Update Success'
    );
  }
  else {
    $result = array(
      'status' => FALSE,
      'message' => 'Update Fail'
    );
  }
  return $result;
}

function apsjobs_services_vacancy_resource_delete($nid) {
  node_delete($nid);
  return TRUE;
}

function apsjobs_services_vacancy_resource_validate(&$node) {
  if (!isset($node['type'])) {
    return services_error(t('Missing node type'), 406);
  }
  else if ($node['type'] != 'aps_vacancies') {
    return services_error(t('Node type must be aps_vacancies'), 406);
  }
  // Wanted to return a graceful error instead of a blank nid, this should
  // allow for that.
  $types = node_type_get_types();
  $node_type = $node['type'];
  if (!isset($types[$node_type])) {
    return services_error(t('Node type @type does not exist.', array('@type' => $node_type)), 406);
  }
  $allowed_node_types = variable_get('services_allowed_create_content_types', $types);
  if (!isset($allowed_node_types[$node_type])) {
    return services_error(t("This node type @type can't be processed via services", array('@type' => $node_type)), 406);
  }

  ///
  $values = $node;
  if (empty($values['employer'])) {
    return services_error('Missing employer', 406);
  }
  else{
    $valid_agencies = apsjobs_vacancies_get_employer();
    $valid_agencies_id = array_keys($valid_agencies);
    if (!in_array($values['employer'], $valid_agencies_id)) {
      return services_error('Invalid employer', 406);
    }
  }
  if (empty($values['position_number'])) {
    return services_error('Missing position_number', 406);
  }
  if (empty($values['job_title'])) {
    return services_error('Missing job_title', 406);
  } else if (strlen($values['job_title']) > 128) {
    return services_error('job_title maximum length is 128 letters', 406);
  }

  if (empty($values['moderation'])) {
    return services_error('moderation required', 406);
  }

  $working_hours = array();
  foreach ($values['working_hours'] as $key => $value) {
    if (!empty($value) && $key != 'all')
      $working_hours[] = $key;
  }
  $values['working_hours'] = $working_hours;
  if (empty($working_hours)) {
    return services_error('Missing working_hours', 406);
  }

  $engagement_types = array();
  foreach ($values['engagement_types'] as $key => $value) {
    if (!empty($value) && $key != 'all')
      $engagement_types[] = $key;
  }
  $values['engagement_types'] = $engagement_types;
  if (empty($engagement_types)) {
    return services_error('Missing engagement_types', 406);
  }
  else if (in_array('248', $engagement_types)) {
    if (!isset($values['non_ongoing_duration']) || empty($values['non_ongoing_duration'])) {
      return services_error('non_ongoing_duration required if engagement_types chose non_ongoing', 406);
    }
  }

  $specific_considerations = array();
  foreach ($values['specific_considerations'] as $key => $value) {
    if (!empty($value))
      $specific_considerations[] = $key;
  }
  $values['specific_considerations'] = $specific_considerations;
  if (empty($specific_considerations)) {
    return services_error('Missing specific_considerations', 406);
  }

  if (empty($values['salary_type']) && empty($values['salary_from']) && empty($values['salary_to'])) {
    return services_error('Missing salary', 406);
  }
  elseif (!empty($values['salary_type'])) {
    if ($values['salary_type'] != 'annually' && $values['salary_type'] != 'hourly') {
      return services_error('salary_type is annually or hourly', 406);
    }
    if (empty($values['salary_from'])) {
      return services_error('Missing salary_from', 406);
    }
    elseif (!is_numeric($values['salary_from'])) {
      return services_error('salary_from is numberic', 406);
    }
    if (empty($values['salary_to'])) {
      return services_error('Missing salary_to', 406);
    }
    elseif (!is_numeric($values['salary_to'])) {
      return services_error('salary_to is numberic', 406);
    }

    if (intval($values['salary_from']) > intval($values['salary_to'])) {
      return services_error('salary_to must be equal or greater than salary_from', 406);
    }
  }
  else {    
    return services_error('Missing salary_type', 406);
  }

  if (empty($values['location'])) {
    return services_error('Missing location', 406);
  }

  if (empty($values['publication_date']['date'])) {
    return services_error('Missing publication_date', 406);
  }
  else if (!validateDate($values['publication_date']['date'])) {
    return services_error('publication_date is not valid', 406);
  }
  else if (!isTodayOrFuture($values['publication_date']['date'])) {
    return services_error('publication_date can not be past', 406);
  }

  if (!empty($values['closing_date']['date'])) {
    if (!validateDate($values['closing_date']['date'])) {
      return services_error('closing_date is not valid', 406);
    }    
  }

  if (!empty($values['publication_date']['date']) && !empty($values['closing_date']['date'])) {
    $publication = strtotime($values['publication_date']['date']);
    $closing_date = strtotime($values['closing_date']['date']);
    if ($closing_date < $publication) {
      return services_error('Closing date must be equal or after Publication date', 406);
    }
  }
  if ($values['legal'] == 0 && ($form_state['clicked_button']['#id'] == 'edit-save-publish' || $form_state['clicked_button']['#id'] == 'edit-save-draft')) {
    return services_error('Missing legal', 406);
  }
  $node = $values;
}

function apsjobs_services_vacancy_resource_save($node) {
  $uid = $node['uid'];
  $values = $node;
  switch ($node['moderation']) {
    case 'draft':      
      $vacancy_id = apsjobs_vacancies_action_save($uid, $values, 'draft');
      return $vacancy_id;
    case 'needs_review':
      $vacancy_id = apsjobs_vacancies_action_save($uid, $values, 'needs_review');
      // if ($vacancy_id) {
      //   if (in_array('APS Agency User', $user->roles)) {
      //     apsjobs_vacancies_send_email_to_approver($vacancy_id, $values['approver']);
      //   }
      // }
      return $vacancy_id;
  }
  return NULL;
}

function validateDate($date){
  $time = strtotime($date);
  $d = date('D M d, Y', $time);
  return $d && $d == $date;
}

function isTodayOrFuture($date) {
  $yesterday = time() - 24*60*60;
  $time = strtotime($date);
  
  return $yesterday < $time;
}