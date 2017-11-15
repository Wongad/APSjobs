<?php

function apsjobs_vacancies_new_vacancy_form_submit($form, &$form_state) {
  global $user;
  $uid = $user->uid;
  $values = $form_state['input'];
  $action = ($values['vacancy_nid'] != NULL) ? 'Updated' : 'Created';
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-preview':
      $vacancy = apsjobs_vacancies_action_save($uid, $values, 'draft', TRUE);
      $form_state['vacancy_preview'] = $vacancy;
      $form_state['rebuild'] = TRUE;
      break;

    case 'edit-edit':
      unset($form_state['vacancy_preview']);
      $form_state['rebuild'] = TRUE;
      break;

    case 'edit-save-draft':
      print_r('123');
      $vacancy_id = apsjobs_vacancies_action_save($uid, $values, 'draft');
      if ($vacancy_id) {
        drupal_set_message(t($action . ' "' . $values['job_title'] . '" to Draft!'), 'status', FALSE);
        if (in_array('APS Agency User', $user->roles)) {
          //apsjobs_vacancies_send_email_to_approver($vacancy_id);
          drupal_goto('aps-agency/my-job-vacancies/drafts');
        }
        else if (in_array('APS Agency Approver', $user->roles)) {
          drupal_goto('aps-agency/my-job-vacancies/unapproved-vacancies');
        }
      }
      break;

    case 'edit-save-publish':
      $vacancy_id = apsjobs_vacancies_action_save($uid, $values, 'needs_review');
      if ($vacancy_id) {
        drupal_set_message(t($action . ' "' . $values['job_title'] . '" to Awaiting Approval!'), 'status', FALSE);
        if (in_array('APS Agency User', $user->roles)) {
          //TODO disable send mail on test, enable on production
          apsjobs_vacancies_send_email_to_approver($vacancy_id, $values['approver']);
          drupal_goto('aps-agency/my-job-vacancies/awaiting-approval');
        }
        else if (in_array('APS Agency Approver', $user->roles)) {
          drupal_goto('aps-agency/my-job-vacancies/unapproved-vacancies');
        }
      }
      else {
        drupal_set_message(t('Error occur when trying to save vacancy to Awaiting Approval!'), 'error', FALSE);
      }
      break;
  }
}

function apsjobs_vacancies_new_vacancy_form_validate(&$form, &$form_state) {
  $values = $form_state['input'];

  if ($form_state['clicked_button']['#id'] == 'edit-cancel' || $form_state['clicked_button']['#id'] == 'edit-cancel-preview') {
    drupal_get_messages('error');
    if ($values['vacancy_nid'] != NULL) {
      drupal_goto('node/' . $values['vacancy_nid']);
    }
    else {
      drupal_goto('aps-agency/my-job-vacancies');
    }
    return;
  }

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

  if ($values['legal'] == 0 && ($form_state['clicked_button']['#id'] == 'edit-save-publish' || $form_state['clicked_button']['#id'] == 'edit-save-draft')) {
    form_set_error('legal', 'Legal confirmation field is required.');
  }

  $form_state['input'] = $values;
}

function apsjobs_vacancies_new_vacancy_form($form, &$form_state) {
  $args = arg();
  $nid = isset($args[2]) ? $args[2] : NULL;
  $action = isset($args[1]) ? $args[1] : NULL;
  $vacancy = ($action == 'edit-vacancy') ? node_load($nid) : NULL;
  global $user;
  $creator_id = $vacancy ? $vacancy->uid : $user->uid;
  $creator = user_load($creator_id);
  $default_value = $vacancy ? apsjobs_vacancies_extract_value($vacancy) : NULL;
  $form = array(
    '#attributes' => array('class' => array('apsjobs-form')),
  );
  // Some special stuff when previewing a node.
  if (isset($form_state['vacancy_preview'])) {
    $vacancy_preview = $form_state['vacancy_preview'];
    unset($form_state['vacancy_preview']);
    $vacancy_preview->in_preview = TRUE;
    if (!isset($vacancy_preview->nid)) {
      $vacancy_preview->nid = NULL;
      $vacancy_preview->created = 0;
    }
    $output = node_view($vacancy_preview, 'full');
    $form['preview_container'] = array(
      '#type' => 'container',
      'preview_title' => array('#markup' => '<h2>'.t('Preview').'</h2>'),
      'preview_body' => array('#markup' => drupal_render($output)),
    );
  }
  else {
    if ($vacancy) unset($vacancy->in_preview);
    unset($vacancy_preview);
    unset($form['preview_container']);
  }

  $hide_on_preview = isset($vacancy_preview) ? 'style="display:none"' : '';
  $hide_on_edit = !isset($vacancy_preview) ? 'style="display:none"' : '';

  $form['vacancy'] = array(
    '#prefix' => isset($vacancy_preview) ? '<div id="edit-form" style="display:none;">' : '<div id="edit-form">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#title' => ' ',//$vacancy ? 'EDIT JOB' : 'NEW JOB',

    'vacancy_nid' => array(
      '#type' => 'hidden',
      '#value' => $vacancy ? $vacancy->nid : NULL,
    ),

    'agency_owner' => array(
      '#type' => 'hidden',
      '#value' => $creator->field_agency_register[LANGUAGE_NONE][0]['target_id'],
    ),

    'position_number' => array(
      '#type' => 'textfield',
      '#title' => t('Position Number'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#default_value' => $default_value ? $default_value['position_number'] : '',
    ),

    'employer' => array(
      '#type' => 'select',
      '#title' => t('Employer (Agency name, Department or University)'),
      '#required' => TRUE,
      '#options' => apsjobs_vacancies_get_employer(),
      '#default_value' => $default_value ? $default_value['employer'] : array(),
    ),

    'job_title' => array(
      '#type' => 'textfield',
      '#title' => t('Job title (max 128 characters)'),
      '#required' => TRUE,
      '#default_value' => $default_value ? $default_value['job_title'] : '',
    ),

    'location' => array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#required' => TRUE,
      '#attributes' => array('placeholder' => 'Type suburb/ postcode'),
      '#description' => '* System provides options based on what user
      has typed in, or provide dropdown for state, then search
      via suburb/postcode?',
      '#default_value' => $default_value ? $default_value['location'] : '',
      '#autocomplete_path' => 'vacancies-location/autocomplete',
    ),

    'publication_date' => array(
      '#type' => 'date_popup',
      '#title' => t('Publication date'),
      '#required' => TRUE,
      '#date_format' => 'D M d, Y',
      '#date_increment' => 1,
      '#date_type' => DATE_DATETIME,
      '#datepicker_options' => array(
        'minDate' => '0',
        //'maxDate' => '0',
        //'yearRange' => "2001:+5",
      ),
      '#default_value' => $default_value ? $default_value['publication_date'] : date('Y-m-d H:i:s', time()),
      '#date_label_position' => 'none',
      '#attributes' => array('readonly' => 'readonly'),
    ),

    'closing_date' => array(
      '#type' => 'date_popup',
      '#title' => t('Closing date'),
      '#date_format' => 'D M d, Y',
      '#date_increment' => 1,
      '#date_type' => DATE_DATETIME,
      '#datepicker_options' => array(
        'minDate' => '0',
        //'maxDate' => '0',
        //'yearRange' => "2001:+5",
      ),
      '#default_value' => $default_value ? $default_value['closing_date'] : '',
      '#date_label_position' => 'none',
      //'#attributes' => array('readonly' => 'readonly'),
    ),

    'job_type' => array(
      '#type' => 'fieldset',
      '#title' => t('Job type'),

      'working_hours' => array(
        '#type' => 'checkboxes',
        '#title' => t('Working hours'),
        '#required' => TRUE,
        '#options' => array('all' => 'All') + apsjobs_vacancies_get_working_hour(),
        '#default_value' => $default_value ? $default_value['working_hours'] : array(),
      ),

      'engagement_container' => array(
        '#type' => 'container',
        'engagement_types' => array(
          '#type' => 'checkboxes',
          '#title' => t('Engagement type'),
          '#required' => TRUE,
          '#options' => array('all' => 'All') + apsjobs_vacancies_get_engagement_type(),
          '#default_value' => $default_value ? $default_value['engagement_types'] : array(),
        ),

        'non_ongoing_duration' => array(
          '#type' => 'select',
          '#options' => array('3 months' => '3 months', '6 months' => '6 months', '9 months' => '9 months', '12 months' => '12 months'),
          '#default_value' => $default_value ? $default_value['non_ongoing_duration'] : '3 months',
          '#attributes' => array('class' => array('hidden')),
        ),
      ),

      'specific_considerations' => array(
        '#type' => 'checkboxes',
        '#title' => t('Specific considerations'),
        '#required' => TRUE,
        '#options' => apsjobs_vacancies_get_considerations(),
        '#default_value' => $default_value ? $default_value['specific_considerations'] : array(),
      ),
    ),

    'job_category' => array(
      '#type' => 'select',
      '#title' => t('Job Category'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_job_category(),
      '#default_value' => $default_value ? $default_value['job_category'] : array(),
    ),

    'job_level' => array(
      '#type' => 'select',
      '#title' => t('Job Level'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_job_level(),
      '#default_value' => $default_value ? $default_value['job_level'] : array(),
    ),

    'clearance_level' => array(
      '#type' => 'select',
      '#title' => t('Clearance Level'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_clearance_level(),
      '#default_value' => $default_value ? $default_value['clearance_level'] : array(),
    ),

    'group_salary' => array (
      '#type' => 'container',
      '#attributes' => array('class' => array('group-salary')),
      'salary_type' => array(
        '#type' => 'radios',
        '#title' => t('Salary'),
        '#required' => TRUE,
        '#options' => array('annually' => 'Annual', 'hourly' => 'Hourly'),
        '#default_value' => $default_value ? $default_value['salary_type'] : 'annually',
      ),

      'salary_from' => array(
        '#type' => 'textfield',
        //'#title' => t('Or'),
        '#attributes' => array('placeholder' => t('Minimum Salary')),
        '#default_value' => $default_value ? $default_value['salary_from'] : '',
      ),

      'salary_to' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => t('Maximum Salary')),
        '#default_value' => $default_value ? $default_value['salary_to'] : '',
      ),
    ),


    'job_description' => array(
      '#type' => 'text_format',
      '#title' => t('Job Description'),
      '#attributes' => array('placeholder' => 'Enter Job Description'),
      '#default_value' => $default_value ? $default_value['job_description'] : '',
    ),

    'apply_url' => array(
      '#type' => 'textfield',
      '#title' => t('Apply URL'),
      '#attributes' => array('placeholder' => array('Enter URL Link')),
      '#description' => '(if there is no URL to apply, please provide
       instructions to apply within the Job Description field)
       <a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#default_value' => $default_value ? $default_value['apply_url'] : '',
    ),

    'legal' => array(
      '#type' => 'checkbox',
      '#required' => TRUE,
      '#title' => 'Legal text saying that I have done my due diligence
       and met the legal requiremenys to publish this job.',
      '#description' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean euismod bibendum laoreet. Proin gravida dolor sit amet lacus accumsan et viverra justo commodo. Proin sodales pulvinar tempor. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nam fermentum, nulla luctus pharetra vulputate, felis tellus mollis orci, sed rhoncus sapien nunc eget.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean euismod bibendum laoreet. Proin gravida dolor sit amet lacus accumsan et viverra justo commodo. Proin sodales pulvinar tempor. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nam fermentum, nulla luctus pharetra vulputate, felis tellus mollis orci, sed rhoncus sapien nunc eget.',
      '#default_value' => $default_value ? $default_value['legal'] : NULL,
    ),
  );

  $form['select_approver_popup'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="select-approver-popup" class="modal fade" role="dialog"><div class="modal-dialog"><div class="modal-content"><div class="modal-body">',
    'approver' => array(
      '#type' => 'select',
      '#title' => t('Select approver'),
      '#options' => apsjobs_vacancies_get_approvers_mail($creator_id),
    ),
    'save_publish' => array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    ),
    '#suffix' => '</div></div></div></div>',
  );

  $form['group-btn'] = array(
    '#type' => 'container',
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
    'edit' => array(
      // '#markup' => '<span class="btn form-submit" id="edit-show-form" '.$hide_on_edit .'>'.t('Edit').'</span>',
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#attributes' => $hide_on_edit != '' ? array('style' => 'display:none') : array(),
    ),
    'save_draft' => array(
      '#type' => 'submit',
      '#value' => t('Save Draft'),
    ),
    'preview' => array(
      '#type' => 'submit',
      '#value' => t('Preview'),
      '#attributes' => $hide_on_preview != '' ? array('style' => 'display:none') : array(),
    ),
    'select_approver' => array(
      '#markup' => '<button type="button" id="edit-select-approver" class="form-submit" data-toggle="modal" data-target="#select-approver-popup" '.$hide_on_edit .'>' . t('Submit for Approval/Publish (Managers)') . '</button>',
    ),
  );

  return $form;
}
