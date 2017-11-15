<?php
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

function apsjobs_outcomes_action_outcome_save($uid, $values, $moderation) {
  $args = arg();
  $jid = $args[2];
  if (isset($values['outcome_nid']) && $values['outcome_nid'] != NULL) {
    $outcome = node_load($values['outcome_nid']);
    if ($moderation != $outcome->workbench_moderation['current']->state) {
      if (_workbench_moderation_moderate_access($vacancy, $moderation)) {
        $outcome->workbench_moderation_state_new = $moderation;
        $outcome->revision = 1;
        $outcome->log = "Bulk moderation state change.";
        node_save($outcome);
        watchdog('action', 'Change node %nid moderation state to %state.', array('%nid' => $outcome->nid, '%state' => $moderation));
      }
      else {
        drupal_set_message(t('You do not have permission to moderate %node to %moderation state', array('%node' => $outcome->title, '%moderation' => $moderation)), 'warning');
        return NULL;
      }
    }
  }
  else {
    $outcome_type = $args[3];
    $vacancy = node_load($jid);
    $to_job_level = isset($vacancy->field_job_level[LANGUAGE_NONE][0]['tid']) ? $vacancy->field_job_level[LANGUAGE_NONE][0]['tid'] : '';
    //remove old outcome if created
    if (!empty($vacancy->field_outcome)) {
      $ooid = $vacancy->field_outcome[$vacancy->language][0]['target_id'];
      $old_outcome = node_load($ooid);
      if (!empty($old_outcome->field_employee_details)) {
        $candidate_id = $old_outcome->field_employee_details[$vacancy->language][0]['target_id'];
        node_delete($candidate_id);        
      }
      node_delete($ooid);
    }
    //save candidate
    $cid = apsjobs_outcomes_action_candidate_save($uid, $values);

    //ceate outcome
    $node = entity_create('node', array(
      'type' => 'aps_outcomes',
      'uid' => $uid,
    ));

    $node->title = $vacancy->title . ' ' . $outcome_type . ' outcome';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->status = 1;
    $node->promote = 0;
    $node->revision = 1;
    $node->workbench_moderation_state_new = $moderation;
    $outcome_wrapper = entity_metadata_wrapper('node', $node);

    $outcome_wrapper->field_publication_date = strtotime($values['publication_date']['date']);

    $node->field_is_isac[$node->language][]['value'] = $values['isac'];
    $node->field_is_recruitability[$node->language][]['value'] = $values['recruitability'];
    $node->field_notes[$node->language][]['value'] = $values['notes'];
    $node->field_editorial_instructions[$node->language][]['value'] = $values['editorial_instructions'];
    $node->field_employee_details[$node->language][] = array(
      'target_id' => $cid,
      'target_type' => 'node',
    );
    $node->field_vacancy[$node->language][] = array(
      'target_id' => $vacancy->nid,
      'target_type' => 'node',
    );
    $node->field_outcome_type[$node->language][]['tid'] = $values['outcome_type'];
    if ($outcome_type == 'engagement') {
      if (!empty($values['engagement_date']['date'])) {
        $outcome_wrapper->field_date_of_engagement = strtotime($values['engagement_date']['date']);
      }
      $node->field_section_of_ps_act[$node->language][]['value'] = $values['section_of_the_act'];
      $node->field_employ_status[$node->language][]['value'] = $values['employment_status'];
    }
    else if ($outcome_type == 'movement' || $outcome_type == 'promotion') {
      $node->field_from_agency[$node->language][] = array(
        'target_id' => $vacancy->nid,
        'target_type' => 'node',
      );
      $node->field_to_agency[$node->language][] = array(
        'target_id' => $vacancy->nid,
        'target_type' => 'node',
      );
      $node->field_from_agency_job_level[$node->language][]['tid'] = $values['from_job_level'];
      if (isset($to_job_level)) {
        $node->field_to_agency_job_level[$node->language][]['tid'] = $to_job_level;
      }      
    }
    $node = node_submit($node);
    node_save($node);
    //update vacancy
    $vacancy->field_outcome[$node->language][]['target_id'] = $node->nid;
    node_save($vacancy);
    return $node->nid;
  }
}

function apsjobs_outcomes_extract_value($outcome) {
  $values = NULL;
  
  if ($outcome) {
    $language = $outcome->language;    
    
    $candidate_nid = isset($outcome->field_employee_details[$language][0]['target_id']) ? $outcome->field_employee_details[$language][0]['target_id'] : NULL;
    
    $candidate = isset($candidate_nid) ? node_load($candidate_nid) : NULL;
    $values['candidate_nid'] = $candidate_nid;
    $values['first_name'] = isset($candidate->field_first_name[$language][0]['value']) ? $candidate->field_first_name[$language][0]['value'] : '';
    $values['last_name'] = isset($candidate->field_last_name[$language][0]['value']) ? $candidate->field_last_name[$language][0]['value'] : '';
    $values['ags_number'] = isset($candidate->field_ags_number[$language][0]['value']) ? $candidate->field_ags_number[$language][0]['value'] : '';

    $values['publication_date'] = isset($outcome->field_publication_date[$language][0]['value']) ? $outcome->field_publication_date[$language][0]['value'] : '';

    $values['recruitability'] = isset($outcome->field_is_recruitability[$language][0]['value']) ? $outcome->field_is_recruitability[$language][0]['value'] : '';

    $values['editorial_instructions'] = isset($outcome->field_editorial_instructions[$language][0]['value']) ? $outcome->field_editorial_instructions[$language][0]['value'] : '';

    $values['isac'] = isset($outcome->field_is_isac[$language][0]['value']) ? $outcome->field_is_isac[$language][0]['value'] : '';

    $values['notes'] = isset($outcome->field_notes[$language][0]['value']) ? $outcome->field_notes[$language][0]['value'] : '';

    $values['outcome_type'] = isset($outcome->field_outcome_type[$outcome->language][0]['tid']) ? $outcome->field_outcome_type[$outcome->language][0]['tid'] : '';
    
    if ($outcome->field_outcome_type[$outcome->language][0]['tid'] == 280) {      
      $values['engagement_date'] = isset($outcome->field_date_of_engagement[$outcome->language][0]['value']) ? $outcome->field_date_of_engagement[$outcome->language][0]['value'] : '';
      $values['section_of_the_act'] = isset($outcome->field_section_of_ps_act[$outcome->language][0]['value']) ? $outcome->field_section_of_ps_act[$outcome->language][0]['value'] : '';
      $values['employment_status'] = isset($outcome->field_employ_status[$outcome->language][0]['value']) ? $outcome->field_employ_status[$outcome->language][0]['value'] : '';    
    } else {
      $values['from_agency'] = isset($outcome->field_from_agency[$outcome->language][0]['target_id']) ? $outcome->field_from_agency[$outcome->language][0]['target_id'] : '';
      $values['to_agency'] = isset($outcome->field_to_agency[$outcome->language][0]['target_id']) ? $outcome->field_to_agency[$outcome->language][0]['target_id'] : '';

      $values['from_job_level'] = isset($outcome->field_from_agency_job_level[$outcome->language][0]['tid']) ? $outcome->field_from_agency_job_level[$outcome->language][0]['tid'] : '';
      $values['to_job_level'] = isset($outcome->field_to_agency_job_level[$outcome->language][0]['tid']) ? $outcome->field_from_agency_job_level[$outcome->language][0]['tid'] : '';
    }
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
      if ($value->weight < $selected_term->weight) {
        return FALSE;
      }
    }
  }
  return TRUE;
}