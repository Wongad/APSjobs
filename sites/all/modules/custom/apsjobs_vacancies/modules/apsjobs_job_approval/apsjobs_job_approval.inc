<?php

function test_cron_apsjobs() {
  print 'hello world';
  $query = db_select('workbench_moderation_node_history', 'w');
  $all_approved_vacancy = $query->fields('w', array('nid'))
  ->condition('w.state', 'approved', '=')
  ->condition('w.is_current', 1, '=')
  ->execute()
  ->fetchAll();
  $nids = array();
  foreach ($all_approved_vacancy as $key => $value) {
    $nids[] = $value->nid;
  }
  $vacancies = entity_load('node', $nids);
  foreach ($vacancies as $vacancy) {
    $wrapper = entity_metadata_wrapper('node', $vacancy);
    $current = time();
    if ($wrapper->field_publish_date->value() < $current) {
      workbench_moderation_set_state_action($vacancy, array('state' => 'published'));
    }
  }
}

function apsjobs_job_approval_theme() {
  $result = array();
  $result['apsjobs_vacancy_approval_block'] = array(
    'variables' => array('block' => array()),
    'template' => 'apsjobs_job_approval_block',
    'path' => drupal_get_path('module', 'apsjobs_job_approval') . '/templates',
  );
  return $result;
}

function apsjobs_job_approval_reject_date() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    workbench_moderation_set_state_action($vacancy, array('state' => 'rejected'));
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE);
    if($state) {
      if(apsjobs_job_approval_update_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE, APSJOBS_APPROVAL_VALUE_REJECTED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    else {
      if(apsjobs_job_approval_insert_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE, APSJOBS_APPROVAL_VALUE_REJECTED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    if(apsjobs_job_approval_check_send_email('node', $vacancy->type, $vacancy->nid)) {
      // send email
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_approve_date() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE);

    if($state) {
      if(apsjobs_job_approval_update_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE, APSJOBS_APPROVAL_VALUE_APPROVED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    else {
      if(apsjobs_job_approval_insert_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE, APSJOBS_APPROVAL_VALUE_APPROVED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    if(apsjobs_job_approval_check_send_email('node', $vacancy->type, $vacancy->nid)) {
      // send email
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_reject_detail() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    workbench_moderation_set_state_action($vacancy, array('state' => 'rejected'));
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL);

    if($state) {
      if(apsjobs_job_approval_update_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL, APSJOBS_APPROVAL_VALUE_REJECTED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    else {
      if(apsjobs_job_approval_insert_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL, APSJOBS_APPROVAL_VALUE_REJECTED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    if(apsjobs_job_approval_check_send_email('node', $vacancy->type, $vacancy->nid)) {
      // send email
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_approve_detail() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL);

    if($state) {
      if(apsjobs_job_approval_update_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL, APSJOBS_APPROVAL_VALUE_APPROVED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    else {
      if(apsjobs_job_approval_insert_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL, APSJOBS_APPROVAL_VALUE_APPROVED)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
    if(apsjobs_job_approval_check_send_email('node', $vacancy->type, $vacancy->nid)) {
      // send email
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_reset_date() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE);
    if($state) {
      if(apsjobs_job_approval_delete_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DATE)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_reset_detail() {
  $args = arg();
  $nid = isset($args[1]) ? $args[1] : NULL;
  $failue = '{"result":"failue"}';
  $success = '{"result":"success"}';
  $vacancy = node_load($nid);
  if ($vacancy) {
    $state = apsjobs_job_approval_get_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL);
    if($state) {
      if(apsjobs_job_approval_delete_state('node', $vacancy->type, $vacancy->nid, APSJOBS_APPROVAL_TYPE_DETAIL)) {
        print $success;
      }
      else {
        print $failue;
      }
    }
  }
  else {
    print $failue;
  }
}

function apsjobs_job_approval_get_state($entity_type, $bundle, $entity_id, $type) {
  //return array($entity_type, $bundle, $entity_id, $type);
  return db_select('apsjobs_approval_states', 's')
  	->fields('s')
  	->condition('entity_type', $entity_type)
  	->condition('bundle', $bundle)
  	->condition('entity_id', $entity_id)
  	->condition('approval_type', $type)
  	->execute()
  	->fetchObject();
}

function apsjobs_job_approval_update_state($entity_type, $bundle, $entity_id, $type, $value) {
  return db_update('apsjobs_approval_states')
    ->fields(array(
      'approval_value' => $value,
      'changed' => time()
    ))
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('entity_id', $entity_id)
    ->condition('approval_type', $type)
    ->execute();
}

function apsjobs_job_approval_delete_state($entity_type, $bundle, $entity_id, $type, $value) {
  return db_delete('apsjobs_approval_states')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('entity_id', $entity_id)
    ->condition('approval_type', $type)
    ->execute();
}

function apsjobs_job_approval_insert_state($entity_type, $bundle, $entity_id, $type, $value) {
  //return array($entity_type, $bundle, $entity_id, $type, $value);
  $now = time();
  return db_insert('apsjobs_approval_states')
    ->fields(array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'entity_id' => $entity_id,
      'approval_type' => $type,
      'approval_value' => $value,
      'created' => $now,
      'changed' => $now,
    ))
    ->execute();
}

