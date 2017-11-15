<?php
/**
  * @file
  * Handles the form validation of the Outcomes Form
  */

function apsjobs_outcomes_create_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-cancel') {
    drupal_get_messages('error');
    if ($form_state['values']['outcome-action'] == 'create-free-outcome') {
      drupal_goto("aps-agency/dashboard");
    }
    else {
      drupal_goto("node/".arg()[2]);
    }
  }

  $values = $form_state['values'];
  // validate vacancy
  if (empty($values['position_number'])) {
    form_set_error('position_number', 'Position Number field is required.');
  }
  if (empty($values['job_title'])) {
    form_set_error('job_title', 'Job title field is required');
  }

  if (empty($values['location'])) {
    form_set_error('location', 'Location field is required.');
  }

  $working_hours = array();
  foreach ($values['working_hours'] as $key => $value) {
    if (!empty($value) && $key != 'all')
      $working_hours[] = $key;
  }
  $values['working_hours'] = $working_hours;
  if (empty($working_hours)) {
    form_set_error('working_hours', 'Working hours field is required.');
  }

  $engagement_types = array();
  foreach ($values['engagement_types'] as $key => $value) {
    if (!empty($value) && $key != 'all')
      $engagement_types[] = $key;
  }
  $values['engagement_types'] = $engagement_types;
  if (empty($engagement_types)) {
    form_set_error('engagement_types', 'Engagement type field is required.');
  }

  $specific_considerations = array();
  foreach ($values['specific_considerations'] as $key => $value) {
    if (!empty($value))
      $specific_considerations[] = $key;
  }
  $values['specific_considerations'] = $specific_considerations;
  if (empty($specific_considerations)) {
    form_set_error('specific_considerations', 'Specific considerations field is required.');
  }

  $is_valid_salary_type = TRUE;
  if (empty($values['salary_type'])) {
    form_set_error('salary_type', 'Salary type is required.');
    $is_valid_salary_type = FALSE;
  }

  $is_valid_salary_from = TRUE;
  if (empty($values['salary_from'])) {
    form_set_error('salary_from', 'Minium salary is required.');
    $is_valid_salary_from = FALSE;
  }
  elseif (!is_numeric($values['salary_from'])) {
    form_set_error('salary_from', 'Minimum salary must be a number.');
    $is_valid_salary_from = FALSE;
  }

  $is_valid_salary_to = TRUE;
  if (empty($values['salary_to'])) {
    form_set_error('salary_to', 'Maximum salary is required.');
    $is_valid_salary_to = FALSE;
  }
  elseif (!is_numeric($values['salary_to'])) {
    form_set_error('salary_to', 'Maximum salary must be a number.');
    $is_valid_salary_to = FALSE;
  }

  if ($is_valid_salary_type && $is_valid_salary_from && $is_valid_salary_to) {
    if (intval($values['salary_from']) <= 0) {
      form_set_error('salary_from', 'Minimum salary must be greater than $0.');
      $is_valid_salary_from = FALSE;
    }

    if (intval($values['salary_to']) < intval($values['salary_from'])) {
      form_set_error('salary_to', 'Maximum salary must be greater than or equal to minimum salary.');
      $is_valid_salary_to = FALSE;
    }
  }
  $form_state['values'] = $values;
  // end validate vacancy

  // validate candidate values
  if (empty($values['first_name'])) {
    form_set_error('first_name', t('First Name of candidate is required'));
  }
  if (empty($values['last_name'])) {
    form_set_error('first_name', t('Last name of candidate is required'));
  }
  if (empty($values['ags_number']) || !is_numeric($values['ags_number'])) {
    form_set_error('ags_number', t('AGS Number is invalid'));
  }
  // end validate candidate values

  if ($values['label_outcome_type'] != 'engagement' && empty($values['from_job_level'])) {
    form_set_error('from_job_level', t('From Agency job level is required'));

    $to_job_level = !empty($values['to_job_level']) ? $values['to_job_level'] : '';
    $to_jlevel = taxonomy_term_load($to_job_level);

    if (isset($values['from_job_level'])) {
      $from_job_level = $values['from_job_level'];
      $from_jlevel = taxonomy_term_load($from_job_level);
      if ($values['label_outcome_type'] == 'promotion'
        && !apsjobs_outcomes_validate_level($from_jlevel->tid, $to_jlevel->tid, $values['label_outcome_type'])) {
          form_set_error('from_job_level', t('The TO job level must be higher than the FROM level'));
      }
      elseif ($values['label_outcome_type'] == 'movement'
        && !apsjobs_outcomes_validate_level($from_jlevel->tid, $to_jlevel->tid, $values['label_outcome_type'])) {
          form_set_error('from_job_level', t('The TO job level must not be higher than the FROM level'));
      }
    }
  }
}
