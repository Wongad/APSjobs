<?php
/**
  * @file
  * Provide utils function for form process.
  */

// load section of the act
function apsjobs_outcomes_section_of_the_act() {
  return array('22(2)a', '22(2)b', '22(2)c', '22(5)', '32', '72(1)a', '72(1)b', '72(1)c', '72(1)d');
}
// load employment status
function apsjobs_outcomes_employment_status() {
  $employment_status = taxonomy_vocabulary_machine_name_load('aps_employment_status');
  $vid = $employment_status->vid;
  $engagement = entity_load('taxonomy_term', FALSE, array('vid' => $vid));
  $engagement_option = array();
  foreach ($engagement as $key => $value) {
    $engagement_option[$key] = $value->name;
  }
  return $engagement_option;
}
//load agency
function apsjobs_outcomes_agency_list() {
  $entities = entity_load('node', FALSE, array('type'=>'aps_agencies', 'status' => 1));
  foreach($entities as $entity) {
    $agencies[$entity->nid] = $entity->title;
  }
  return $agencies;
}

//checked
function apsjobs_outcomes_action_candidate_save($uid, $values) {
  if (!empty($values['candidate_nid'])) {
    $node = node_load($values['candidate_nid']);
    $node->title = $values['first_name'] . ' ' . $values['last_name'];
    $node->field_first_name[$node->language][0]['value'] = $values['first_name'];
    $node->field_last_name[$node->language][0]['value'] = $values['last_name'];
    $node->field_ags_number[$node->language][0]['value'] = $values['ags_number'];
    node_save($node);
  }
  else {
    $node = new stdClass();
    $node->title = $values['first_name'] . ' ' . $values['last_name'];
    $node->type = "aps_candidate";
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $uid;
    $node->status = 1;
    $node->promote = 0;

    $node->field_first_name[$node->language][]['value'] = $values['first_name'];
    $node->field_last_name[$node->language][]['value'] = $values['last_name'];
    $node->field_ags_number[$node->language][]['value'] = $values['ags_number'];

    $node = node_submit($node);
    node_save($node);
  }

  return $node->nid;
}

/**/
function apsjobs_outcomes_action_outcome_save($uid, $values, $moderation) {
  $args = arg();

  switch ($args[1]) {
    case 'create-outcome':
      $outcome = entity_create('node', array(
        'type' => 'aps_outcomes',
        'uid' => $uid,
        'revision' => 1,
        'workbench_moderation_state_new' => $moderation,
      ));
      $vacancy = node_load($args[2]);
      break;

    case 'edit-outcome':
      $outcome = node_load($values['outcome_nid']);
      workbench_moderation_moderate($outcome, $moderation);
      break;

    case 'create-free-outcome':
      $outcome = entity_create('node', array(
        'type' => 'aps_outcomes',
        'uid' => $uid,
        'revision' => 1,
        'workbench_moderation_state_new' => $moderation,
      ));
      break;

    case 'edit-free-outcome':
      $outcome = node_load($values['outcome_nid']);
      workbench_moderation_moderate($outcome, $moderation);
      break;
  }

  $outcome_wrapper = entity_metadata_wrapper('node', $outcome);

  $candidate_nid = apsjobs_outcomes_action_candidate_save($uid, $values);

  $outcome_wrapper->title->set($values['job_title'] . ' ' . $values['label_outcome_type'] . ' outcome');

  $outcome_wrapper->field_is_isac->set($values['isac']);

  $outcome_wrapper->field_is_recruitability->set($values['recruitability']);

  $outcome_wrapper->field_notes->set($values['notes']);

  $outcome_wrapper->field_editorial_instructions->set($values['editorial_instructions']);

  $outcome_wrapper->field_employee_details->set($candidate_nid);

  $outcome_wrapper->field_vacancy->set(isset($vacancy->nid) ? $vacancy->nid : NULL);

  //set vacancy values
  $outcome_wrapper->field_position_number->set($values['position_number']);

  $outcome_wrapper->field_job_title->set($values['job_title']);

  $outcome_wrapper->field_agency->set($values['employer']);

  $outcome_wrapper->field_location_raw->set($values['location']);

  if (!empty($values['publication_date']['date'])) {
    $outcome_wrapper->field_position_number->set(strtotime($values['publication_date']['date']));
  }

  foreach ($values['working_hours'] as $key => $tid) {
    $values['working_hours'][$key] = array('tid' => $tid);
  }
  $outcome_wrapper->field_working_hours->set($values['working_hours']);

  foreach ($values['engagement_types'] as $key => $tid) {
    $values['engagement_types'][$key] = array('tid' => $tid);
  }
  $outcome_wrapper->field_engagement_types->set($values['engagement_types']);

  $non_ongoing_term_id = 248;
  if (in_array($non_ongoing_term_id, $values['engagement_types'])) {
    $outcome_wrapper->field_non_ongoing_duration->set(array($values['non_ongoing_duration']));
  }

  foreach ($values['specific_considerations'] as $key => $tid) {
    $values['specific_considerations'][$key] = array('tid' => $tid);
  }
  $outcome_wrapper->field_specific_considerations->set($values['specific_considerations']);

  $outcome_wrapper->field_job_category->set($values['job_category']);

  $outcome_wrapper->field_job_level->set($values['job_level']);

  $outcome_wrapper->field_clearance_level->set($values['clearance_level']);

  $outcome_wrapper->field_salary_type->set($values['salary_type']);

  $outcome_wrapper->field_salary_from->set($values['salary_from']);

  $outcome_wrapper->field_salary_to->set($values['salary_to']);
  //endset vacancy values

  //$outcome->field_outcome_type[LANGUAGE_NONE][]['tid'] = $values['outcome_type'];
  $outcome_wrapper->field_outcome_type->set($values['outcome_type']);

  switch ($values['outcome_type']) {
    case 'engagement':
      if (!empty($values['engagement_date']['date'])) {
        $outcome_wrapper->field_date_of_engagement = strtotime($values['engagement_date']['date']);
      }
      $outcome_wrapper->field_section_of_ps_act->set($values['section_of_the_act']);
      $outcome_wrapper->field_employ_status->set($values['employment_status']);
      break;

    case 'promotion':
    case 'movement':
      $outcome_wrapper->field_from_agency->set($values['from_agency']);
      $outcome_wrapper->field_from_agency_job_level->set($values['from_agency']);
      $outcome_wrapper->field_to_agency->set($values['from_agency']);
      $outcome_wrapper->field_to_agency_job_level->set($values['from_agency']);
      break;

    case 'cancellation':
      if (!empty($values['reason_cancel'])) {
        $outcome_wrapper->field_reason_cancel->set($values['reason_cancel']);
      }
      break;
  }

  if (!empty($values['reason_change'])) {
    $outcome_wrapper->field_reason_change->set($values['reason_change']);
  }

  $outcome_wrapper->field_notice_number->set(incrementalHash());

  $outcome_wrapper->save();

  //update vacancy
  if (!empty($vacancy->field_outcome)) {
    $ooid = $vacancy->field_outcome[LANGUAGE_NONE][0]['target_id'];
    $old_outcome = node_load($ooid);
    if (!empty($old_outcome->field_employee_details)) {
      $candidate_id = $old_outcome->field_employee_details[LANGUAGE_NONE][0]['target_id'];
      node_delete($candidate_id);
    }
    node_delete($ooid);

    $vacancy->field_outcome[LANGUAGE_NONE][0]['target_id'] = $outcome_wrapper->getIdentifier();
    node_save($vacancy);
  }

  return $outcome_wrapper->getIdentifier();
}

function apsjobs_outcomes_extract_value($outcome) {
  $values = NULL;
  if ($outcome) {
    $outcome_wrapper = entity_metadata_wrapper('node', $outcome);
    $candidate_nid = $outcome_wrapper->field_employee_details->value();
    $candidate = node_load($candidate_nid);
    if (!empty($candidate)) {
      $candidate_wrapper = entity_metadata_wrapper('node', $candidate);
      // candidate values
      $values['candidate_nid'] = $candidate_wrapper->getIndentifier();
      $values['first_name'] = $candidate_wrapper->field_first_name->value();
      $values['last_name'] = $candidate_wrapper->field_last_name->value();
      $values['ags_number'] = $candidate_wrapper->field_ags_number->value();
    }
    // outcome values
    $values['notes'] = $outcome_wrapper->field_notes->value();
    $values['publication_date'] = $outcome_wrapper->field_publication_date->value();
    $values['recruitability'] = $outcome_wrapper->field_is_recruitability->value();
    $values['editorial_instructions'] = $outcome_wrapper->field_editorial_instructions->value();
    $values['isac'] = $outcome_wrapper->field_is_isac->value();
    $values['outcome_type'] = $outcome_wrapper->field_outcome_type->value()->tid;
    switch ($values['outcome_type']) {
      case 280:
        $values['engagement_date'] = $outcome_wrapper->field_date_of_engagement->value();
        $values['section_of_the_act'] = $outcome_wrapper->field_section_of_ps_act->value();
        $values['employment_status'] = $outcome_wrapper->field_employ_status->value()->tid;
        break;

      case 281:
        $values['reason_cancel'] = $outcome_wrapper->field_reason_cancel->value();
        break;

      case 282:
      case 1237:
        $values['from_agency'] = $outcome_wrapper->field_from_agency->value();
        $values['from_job_level'] = $outcome_wrapper->field_from_agency_job_level->value();
        $values['to_agency'] = $outcome_wrapper->field_to_agency->value();
        $values['to_job_level'] = $outcome_wrapper->field_to_agency_job_level->value();
        break;

      default:
        break;
    }

    // vacancy values
    $values['position_number'] = $outcome_wrapper->field_position_number->value();
    $values['employer'] = $outcome_wrapper->field_agency->value()->nid;
    $values['job_title'] = $outcome_wrapper->field_job_title->value();
    $values['location'] = $outcome_wrapper->field_location_raw->value();
    $values['closing_date'] = $outcome_wrapper->field_closing_date->value();
    $values['working_hours'] = isset($outcome->field_working_hours[LANGUAGE_NONE]) ? $outcome->field_working_hours[LANGUAGE_NONE] : array();
    $values['engagement_types'] = isset($outcome->field_engagement_types[LANGUAGE_NONE]) ? $outcome->field_engagement_types[LANGUAGE_NONE] : array();
    $values['specific_considerations'] = isset($outcome->field_specific_considerations[LANGUAGE_NONE]) ? $outcome->field_specific_considerations[LANGUAGE_NONE] : array();
    foreach ($values as $key => $value) {
      if (is_array($value)) {
        foreach ($value as $k => $v) {
          $value[$k] = $v['tid'];
        }
        $values[$key] = $value;
      }
    }
    $non_ongoing_term_id = 248;
    if (in_array($non_ongoing_term_id, $values['engagement_types'])
      && !empty($outcome->field_non_ongoing_duration)) {
      $values['non_ongoing_duration'] = $vacancy->field_non_ongoing_duration[$language][0]['value'];
    } else {
      $values['non_ongoing_duration'] = '3 months';
    }
    $values['job_category'] = $outcome_wrapper->field_job_category->value()->tid;
    $values['job_level'] = $outcome_wrapper->field_job_level->value()->tid;
    $values['clearance_level'] = $outcome_wrapper->field_clearance_level->value()->tid;
    $values['salary_type'] = $outcome_wrapper->field_salary_type->value();
    $values['salary_from'] = $outcome_wrapper->field_salary_from->value();
    $values['salary_to'] = $outcome_wrapper->field_salary_to->value();
  }

  return $values;
}

function apsjobs_outcomes_change_state($moderation, $oid) {
  if (isset($oid)) {
    $outcome = node_load($oid);
    $vacancy_id = $outcome->field_vacancy[$outcome->language][0]['target_id'];
    // check current moderation state, update if not equal
    if ($moderation != $outcome->workbench_moderation['current']->state) {
      if (_workbench_moderation_moderate_access($outcome, $moderation)) {
        workbench_moderation_moderate($outcome, $moderation);
        // $outcome->workbench_moderation_state_new = $moderation;
        // $outcome->revision = 1;
        // $outcome->log = "Bulk moderation state change.";
        // node_save($outcome);
        watchdog('action', 'Change node %nid moderation state to %state.', array('%nid' => $outcome->nid, '%state' => $moderation));
        drupal_set_message(t('%mod %title outcome', array('%mod' => $moderation, '%title' => $outcome->title)), 'status');
        drupal_goto('node/' . $vacancy_id);
      }
      else {
        drupal_set_message(t('You do not have permission to moderate %node to %moderation state', array('%node' => $outcome->title, '%moderation' => $moderation)), 'warning');
        return NULL;
      }
    }
    else {
      drupal_set_message(t('%title outcome %mod', array('%mod' => $moderation, '%title' => $outcome->title)), 'status');
    }
  }
}

function apsjobs_node_workbench_check_state($current_nid, $state) {
  $results = db_select('workbench_moderation_node_history', 'wmnh')
    ->fields('wmnh')
    ->condition('nid', $current_nid)
    ->condition('is_current', 1)
    ->condition('state', $state, 'IN')
    ->execute()
    ->fetchAll();
  return (count($results)) ? TRUE : FALSE;
}

function apsjobs_outcomes_send_email_to_approver($outcome_id, $approver_id) {
  $outcome = node_load($outcome_id);
  $creator = user_load($outcome->uid);
  $approver = user_load($approver_id);

  if ($approver == FALSE) {
    drupal_set_message(t("You ({$creator->mail}) don't have approver"), 'warning', FALSE);
    return;
  }

  global $base_url;
  $link_outcome = $base_url . '/node/'. $outcome_id;

  $variables = array(
    'user_email' => $creator->mail,
    'link_vacancy' => $link_outcome,
  );

  $content = theme_render_template(
    drupal_get_path('module', 'apsjobs_outcomes') . '/templates/apsjobs_outcomes_mail.tpl.php',
    $variables
  );

  $params = array('subject' => 'APSJobs Notification', 'body' => $content);

  drupal_mail('apsjobs_outcomes', 'apsjobs_mail', $approver->mail, LANGUAGE_NONE, $params, $from = NULL, $send = TRUE);
}

//return id terms will be disabled
function apsjobs_outcomes_invalid_level($selected_tid, $outcome_type) {
  $voc_name = 'aps_job_levels';
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  $tree = taxonomy_get_tree($voc->vid);
  $disabled_tids = array();

  foreach ($tree as $key => $value) {
    $tree[$value->tid] = $value;
    unset($tree[$key]);
  }

  $selected_term = $tree[$selected_tid];
  $selected_parent_id = $selected_term->parents[0];
  $selected_parent = $tree[$selected_parent_id];

  foreach ($tree as $value) {
    if ($value->parents[0] != 0) {
      $current_parent = $tree[$value->parents[0]];
      if ($outcome_type == 'promotion') {
        if ($current_parent->weight > $selected_parent->weight) {
          $disabled_tids[] = $value->tid;
        }
        else if ($current_parent->weight == $selected_parent->weight) {
          if ($value->weight >= $selected_term->weight) {
            $disabled_tids[] = $value->tid;
          }
        }
      }
      else if ($outcome_type == 'movement') {
        if ($current_parent->weight < $selected_parent->weight) {
          $disabled_tids[] = $value->tid;
        }
        else if ($current_parent->weight == $selected_parent->weight) {
          if ($value->weight < $selected_term->weight) {
            $disabled_tids[] = $value->tid;
          }
        }
      }
    }
  }
  return $disabled_tids;
}

function apsjobs_outcomes_validate_level($tid_from, $tid_to, $outcome_type) {
  $voc_name = 'aps_job_levels';
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  $tree = taxonomy_get_tree($voc->vid);
  $disabled_tids = array();

  foreach ($tree as $key => $value) {
    $tree[$value->tid] = $value;
    unset($tree[$key]);
  }

  $term_from = $tree[$tid_from];
  $term_from_parent_id = $term_from->parents[0];
  $term_from_parent = $tree[$term_from_parent_id];

  $term_to = $tree[$tid_to];
  $term_to_parent_id = $term_to->parents[0];
  $term_to_parent = $tree[$term_to_parent_id];

  if ($outcome_type == 'promotion') {
    if ($term_from_parent->weight > $term_to_parent->weight) {
      return FALSE;
    }
    else if ($term_from_parent->weight == $term_to_parent->weight) {
      if ($term_from->weight >= $term_to->weight) {
        return FALSE;
      }
    }
  }
  else if ($outcome_type == 'movement') {
    if ($term_from_parent->weight < $term_to_parent->weight) {
      return FALSE;
    }
    else if ($term_from_parent->weight == $term_to_parent->weight) {
      if ($value->weight < $term_to->weight) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

function incrementalHash($len = 5){
  $charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  $base = strlen($charset);
  $result = '';

 $now = explode(' ', microtime())[1];
  while ($now >= $base){
    $i = $now % $base;
    $result = $charset[$i] . $result;
    $now /= $base;
  }
  return substr($result, -5);
}
