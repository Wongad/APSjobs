<?php

/**
 * Load all agency of user
 */
function apsjobs_vacancies_get_employer() {
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

  //load all if agency_id empty;
  if (!empty($agency_id)) {
    $query = $query->condition('nid', $agency_id, 'IN');
  }

  $result = $query->execute()->fetchAll();

  $return = array();
  foreach ($result as $value) {
    $return[$value->nid] = $value->title;
  }
  if (!empty($agency_id)) {
    $agency = node_load($agency_id['field_agency_register_target_id']);
    if (!empty($agency->field_managed_agencies)) {
      $managed_agencies_arr = $agency->field_managed_agencies[LANGUAGE_NONE];
      foreach ($managed_agencies_arr as $item) {
        $aid = $item['target_id'];
        $m_agency = node_load($aid);
        $return[$m_agency->nid] = $m_agency->title;
      }
    }
  }

  return $return;
}

function apsjobs_vacancies_get_term_parent_child($voc_name) {
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  $tree = taxonomy_get_tree($voc->vid);
  $results = array();
  foreach ($tree as $key => $value) {
    $tree[$value->tid] = $value;
    unset($tree[$key]);
  }
  $terms = array();

  foreach ($tree as $value) {
    $prefix = '';
    $parent = $value->parents[0];
    if ($parent != 0) {
      $terms[$tree[$parent]->name][$value->tid] = $value->name;
    }
  }
  return $terms;
}

/**
 * Load all taxonomy term by vocabulary machine name.
 */
function apsjobs_vacancies_get_taxonomy_terms($voc_name) {
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  $tree = taxonomy_get_tree($voc->vid);
  if ($voc->vid == 9) {
    //print_r($tree);exit;
  }
  foreach ($tree as $key => $value) {
    $tree[$value->tid] = $value;
    unset($tree[$key]);
  }

  $return = array();
  foreach ($tree as $value) {
    $prefix = '';
    $parent = $value->parents[0];
    while ($parent != 0) {
      $prefix .= '---';
      $parent = $tree[$parent]->parents[0];
    }
    $return[$value->tid] = $prefix . $value->name;
  }

  return $return;
}

function apsjobs_vacancies_get_working_hour() {
  $name = 'aps_working_hours';
  return apsjobs_vacancies_get_taxonomy_terms($name);
}

function apsjobs_vacancies_get_engagement_type() {
  $name = 'aps_engagement_types';
  return apsjobs_vacancies_get_taxonomy_terms($name);
}

function apsjobs_vacancies_get_considerations() {
  $name = 'aps_specific_considerations';
  return apsjobs_vacancies_get_taxonomy_terms($name);
}

function apsjobs_vacancies_get_job_category() {
  $result = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('tid', 'name'))
    ->condition('vid', 2, '=')
    ->execute()
    ->fetchAllAssoc('tid');
  foreach ($result as $key => $value) {
    $result[$key] = $value->name;
  }
  return $result;
}

function apsjobs_vacancies_get_job_level() {
  $name = 'aps_job_levels';
  return apsjobs_vacancies_get_term_parent_child($name);
}

function apsjobs_vacancies_get_clearance_level() {
  $name = 'aps_clearance_levels';
  return apsjobs_vacancies_get_taxonomy_terms($name);
}

function apsjobs_vacancies_get_salary_range() {
  $name = 'aps_salary_ranges';
  return apsjobs_vacancies_get_taxonomy_terms($name);
}

function apsjobs_vacancies_action_cancel() {
}

function apsjobs_vacancies_action_save($uid, $values, $moderation, $preview = FALSE) {
  if ($values['vacancy_nid'] != NULL) {
    $vacancy = node_load($values['vacancy_nid']);
    if (!$preview) {
      workbench_moderation_moderate($vacancy, $moderation);
    }
  }
  else {
    $vacancy = entity_create('node', array(
      'type' => 'aps_vacancies',
      'uid' => $uid,
    ));
    $vacancy->revision = 1;
    $vacancy->workbench_moderation_state_new = $moderation;
  }

  if ($vacancy == NULL) {
    return;
  }

  $wrapper = entity_metadata_wrapper('node', $vacancy);

  $wrapper->title = $values['job_title'];

  $wrapper->field_position_number = $values['position_number'];

  $wrapper->field_agency = $values['employer'];

  $wrapper->field_agency_owner = $values['agency_owner'];

  $wrapper->field_location_raw = $values['location'];

  $wrapper->field_publish_date = strtotime($values['publication_date']['date']);

  if (!empty($values['closing_date']['date'])) {
    $wrapper->field_closing_date = strtotime($values['closing_date']['date']);
  }

  $wrapper->field_working_hours = $values['working_hours'];

  $wrapper->field_engagement_types = $values['engagement_types'];

  $non_ongoing_term_id = 248;
  if (in_array($non_ongoing_term_id, $values['engagement_types'])) {
    $wrapper->field_non_ongoing_duration = $values['non_ongoing_duration'];
  }

  $wrapper->field_specific_considerations = $values['specific_considerations'];

  $wrapper->field_job_category = $values['job_category'];

  $wrapper->field_job_level = $values['job_level'];

  $wrapper->field_clearance_level = $values['clearance_level'];

  $wrapper->field_salary_type = empty($values['salary_type']) ? NULL : $values['salary_type'];

  $wrapper->field_salary_from = empty($values['salary_from']) ? NULL : $values['salary_from'];

  $wrapper->field_salary_to = empty($values['salary_to']) ? NULL : $values['salary_to'] ;

  $wrapper->field_job_description = $values['job_description'];

  $vacancy->field_apply_url[LANGUAGE_NONE][0]['url'] = $values['apply_url'];

  $wrapper->field_legal_confirmation = $values['legal'];

  if ($preview) {
    return $vacancy;
  }
  else {
    if (!isset($wrapper->field_notice_number) || $wrapper->field_notice_number->value() == '') {
     $wrapper->field_notice_number = incrementalHash();
    }
    $wrapper->save();
    return $vacancy->nid;
  }
}

function apsjobs_vacancies_extract_value($vacancy) {
  $values = NULL;
  if ($vacancy) {
    $language = $vacancy->language;

    $values['position_number'] = isset($vacancy->field_position_number[$language][0]['value']) ? $vacancy->field_position_number[$language][0]['value'] : '';

    $values['employer'] = isset($vacancy->field_agency[$language][0]['target_id']) ? $vacancy->field_agency[$language][0]['target_id'] : NULL;

    $values['job_title'] = $vacancy->title;

    $values['location'] = isset($vacancy->field_location_raw[$language][0]['value']) ? $vacancy->field_location_raw[$language][0]['value'] : '';

    $values['publication_date'] = isset($vacancy->field_publish_date[$language][0]['value']) ? $vacancy->field_publish_date[$language][0]['value'] : '';

    $values['closing_date'] = isset($vacancy->field_closing_date[$language][0]['value']) ? $vacancy->field_closing_date[$language][0]['value'] : '';

    $values['working_hours'] = isset($vacancy->field_working_hours[$language]) ? $vacancy->field_working_hours[$language] : array();

    $values['engagement_types'] = isset($vacancy->field_engagement_types[$language]) ? $vacancy->field_engagement_types[$language] : array();

    $values['specific_considerations'] = isset($vacancy->field_specific_considerations[$language]) ? $vacancy->field_specific_considerations[$language] : array();

    $values['job_category'] = isset($vacancy->field_job_category[$language][0]['tid']) ? $vacancy->field_job_category[$language][0]['tid'] : NULL;

    $values['job_level'] = isset($vacancy->field_job_level[$language][0]['tid']) ? $vacancy->field_job_level[$language][0]['tid'] : NULL;

    $values['clearance_level'] = isset($vacancy->field_clearance_level[$language][0]['tid']) ? $vacancy->field_clearance_level[$language][0]['tid'] : NULL;

    $values['salary_type'] = isset($vacancy->field_salary_type[$language][0]['value']) ? $vacancy->field_salary_type[$language][0]['value'] : 'annually';

    $values['salary_from'] = isset($vacancy->field_salary_from[$language][0]['value']) ? $vacancy->field_salary_from[$language][0]['value'] : '';

    $values['salary_to'] = isset($vacancy->field_salary_to[$language][0]['value']) ? $vacancy->field_salary_to[$language][0]['value'] : '';

    $values['job_description'] = isset($vacancy->field_job_description[$language][0]['value']) ? $vacancy->field_job_description[$language][0]['value'] : '';

    $values['apply_url'] = isset($vacancy->field_apply_url[$language][0]['url']) ? $vacancy->field_apply_url[$language][0]['url'] : '';

    $values['legal'] = isset($vacancy->field_legal_confirmation[$language][0]['value']) ? $vacancy->field_legal_confirmation[$language][0]['value'] : NULL;

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
      && isset($vacancy->field_non_ongoing_duration)
      && !empty($vacancy->field_non_ongoing_duration)) {
      $values['non_ongoing_duration'] = $vacancy->field_non_ongoing_duration[$language][0]['value'];
    } else {
      $values['non_ongoing_duration'] = '3 months';
    }
  }
  return $values;
}

function apsjobs_vacancies_get_approvers_mail($uid) {
  $approvers = apsjobs_vacancies_get_approvers($uid);
  $mails = array();
  foreach ($approvers as $uid => $approver) {
    $value = !empty($approver->field_first_name) ? $approver->field_first_name[LANGUAGE_NONE][0]['value'] : '';
    $value .= ' ';
    $value .= !empty($approver->field_first_name) ? $approver->field_last_name[LANGUAGE_NONE][0]['value'] : '';
    $value .= ' (' . $approver->mail . ')';
    $mails[$uid] = $value;
  }
  return $mails;
}

function apsjobs_vacancies_get_approvers($uid) {
  //get approver id;
  $approver_id = db_select('field_data_field_approvers', 'fa')
    ->fields('fa', array('field_approvers_target_id'))
    ->condition('entity_id', $uid, '=')
    ->execute()
    ->fetchAllAssoc('field_approvers_target_id');
  //get approver
  if (empty($approver_id)) {
    return NULL;
  }
  $uids = array_keys($approver_id);
  $approvers = user_load_multiple($uids);
  return $approvers;
}

function apsjobs_vacancies_send_email_to_approver($vacancy_id, $approver_id) {
  $vacancy = node_load($vacancy_id);
  $creator = user_load($vacancy->uid);
  $approver = user_load($approver_id);

  if ($approver == FALSE) {
    drupal_set_message(t("You ({$creator->mail}) don't have approver"), 'warning', FALSE);
    return;
  }

  //get link vacancy
  global $base_url;
  $link_vacancy = $base_url . '/node/'. $vacancy_id;

  $variables = array(
    'user_email' => $creator->mail,
    'link_vacancy' => $link_vacancy,
  );

  $content = theme_render_template(
    drupal_get_path('module', 'apsjobs_vacancies') . '/templates/apsjobs_vacancies_mail.tpl.php',
    $variables
  );

  $params = array('subject' => 'APSJobs Notification', 'body' => $content);

  drupal_mail('apsjobs_vacancies', 'apsjobs_mail', $approver->mail, LANGUAGE_NONE, $params, $from = NULL, $send = TRUE);
}
