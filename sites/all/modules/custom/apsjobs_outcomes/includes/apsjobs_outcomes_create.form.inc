<?php
/**
  * @file
  * Form create free outcome.
  */

function apsjobs_outcomes_create_form($form, &$form_state) {
  $args = arg();
  if (empty($args)) return $form;

  $values = isset($form_state['values']) ? $form_state['values'] : array();
  switch ($args[1]) {
    case 'create-outcome':
      $vacancy = node_load($args[2]);
      if (empty($vacancy)) {
        return $form;
      }
      $form['vacancy_nid'] = array(
        '#type' => 'hidden',
        '#value' => $args[2],
      );
      if (empty($values)) {
        $values = apsjobs_vacancies_extract_value($vacancy);
      }
      $outcome_type = $args[3];
      $values['to_job_level'] = $values['job_level'];
      $values['to_agency_name'] = $values['employer'];
      break;

    case 'edit-outcome':
      $outcome = node_load($args[2]);
      if (empty($outcome)) {
        return $form;
      }
      if (empty($values)) {
        $values = apsjobs_outcomes_extract_value($outcome);
      }
      $outcome_type = $values['outcome_type'];
      break;

    case 'create-free-outcome':
      $outcome_type = $args[2];
      break;

    case 'edit-free-outcome':
      $outcome = node_load($args[2]);
      if (empty($outcome)) {
        return $form;
      }
      if (empty($values)) {
        $values = apsjobs_outcomes_extract_value($outcome);
      }
      $outcome_type = $values['outcome_type'];
      break;

    default:
      return $form;
  }

  $form = array(
    '#attributes' => array('class' => array('form-apsjobs-create-outcome')),
  );
  $form['outcome_action'] = array(
    '#type' => 'hidden',
    '#value' => $args[1],
  );
  $form['candidate_nid'] = array(
    '#type' => 'hidden',
    '#value' => isset($values['candidate_nid']) ? $values['candidate_nid'] : NULL,
  );
  $form['outcome_nid'] = array(
    '#type' => 'hidden',
    '#value' => isset($outcome->nid) ? $outcome->nid : NULL,
  );

  $form['vacancy'] = array(
    '#type' => 'fieldset',
    '#title' => 'Vacancy',

    'position_number' => array(
      '#type' => 'textfield',
      '#title' => t('Position Number'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#default_value' => isset($values['position_number']) ? $values['position_number'] : '',
    ),

    'employer' => array(
      '#type' => 'select',
      '#title' => t('Employer (Agency name, Department or University)'),
      '#required' => TRUE,
      '#options' => apsjobs_vacancies_get_employer(),
      '#default_value' => isset($values['employer']) ? $values['employer'] : '',
      '#empty_value' => '',
    ),

    'job_title' => array(
      '#type' => 'textfield',
      '#title' => t('Job title (max 128 characters)'),
      '#required' => TRUE,
      '#default_value' => isset($values['job_title']) ? $values['job_title'] : '',
    ),

    'location' => array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#required' => TRUE,
      '#attributes' => array('placeholder' => 'Type suburb/ postcode'),
      '#description' => '* System provides options based on what user
      has typed in, or provide dropdown for state, then search
      via suburb/postcode?',
      '#default_value' => isset($values['location']) ? $values['location'] : '',
      '#autocomplete_path' => 'vacancies-location/autocomplete',
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
      '#default_value' => isset($values['closing_date']) ? $values['closing_date'] : '',
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
        '#default_value' => isset($values['working_hours']) ? $values['working_hours'] : array(),
      ),

      'engagement_container' => array(
        '#type' => 'container',
        'engagement_types' => array(
          '#type' => 'checkboxes',
          '#title' => t('Engagement type'),
          '#required' => TRUE,
          '#options' => array('all' => 'All') + apsjobs_vacancies_get_engagement_type(),
          '#default_value' => isset($values['engagement_types']) ? $values['engagement_types'] : array(),
        ),

        'non_ongoing_duration' => array(
          '#type' => 'select',
          '#options' => array('3 months' => '3 months', '6 months' => '6 months', '9 months' => '9 months', '12 months' => '12 months'),
          '#default_value' => isset($values['non_ongoing_duration']) ? $values['non_ongoing_duration'] : '3 months',
          '#attributes' => array('class' => array('hidden')),
        ),
      ),

      'specific_considerations' => array(
        '#type' => 'checkboxes',
        '#title' => t('Specific considerations'),
        '#required' => TRUE,
        '#options' => apsjobs_vacancies_get_considerations(),
        '#default_value' => isset($values['specific_considerations']) ? $values['specific_considerations'] : array(),
      ),
    ),

    'job_category' => array(
      '#type' => 'select',
      '#title' => t('Job Category'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_job_category(),
      '#default_value' => isset($values['job_category']) ? $values['job_category'] : '',
      '#empty_value' => '',
    ),

    'job_level' => array(
      '#type' => 'select',
      '#title' => t('Job Level'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_job_level(),
      '#default_value' => isset($values['job_level']) ? $values['job_level'] : '',
      '#empty_value' => '',
    ),

    'clearance_level' => array(
      '#type' => 'select',
      '#title' => t('Clearance Level'),
      '#required' => TRUE,
      '#description' => '<a href="" title="Nascetur. Modi ipsam tempor mollit quod, convallis inceptos. Modi aliqua voluptas ultricies, laudantium libero, tincidunt dignissim! Ullamcorper tristique, fringilla quidem.">i</a>',
      '#options' => apsjobs_vacancies_get_clearance_level(),
      '#default_value' => isset($values['clearance_level']) ? $values['clearance_level'] : '',
      '#empty_value' => '',
    ),

    'group_salary' => array (
      '#type' => 'container',
      '#attributes' => array('class' => array('group-salary')),
      'salary_type' => array(
        '#type' => 'radios',
        '#title' => t('Salary'),
        '#required' => TRUE,
        '#options' => array('annually' => 'Annual', 'hourly' => 'Hourly'),
        '#default_value' => isset($values['salary_type']) ? $values['salary_type'] : 'annually',
      ),

      'salary_from' => array(
        '#type' => 'textfield',
        //'#title' => t('Or'),
        '#attributes' => array('placeholder' => t('Minimum Salary')),
        '#default_value' => isset($values['salary_from']) ? $values['salary_from'] : '',
      ),

      'salary_to' => array(
        '#type' => 'textfield',
        '#attributes' => array('placeholder' => t('Maximum Salary')),
        '#default_value' => isset($values['salary_to']) ? $values['salary_to'] : '',
      ),
    ),
  );

  $form['publication_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Publication Date'),
    '#date_format' => 'm/d/Y',
    '#datepicker_options' => array(
      'minDate' => '0',
      //'maxDate' => '0',
      //'yearRange' => "2001:+5",
    ),
    '#default_value' => isset($values['publication_date']) ? $values['publication_date'] : date('Y-m-d'),
    //'#required' => TRUE,
  );

  $form['candidate'] = array(
    '#type' => 'fieldset',
    '#title' => 'Candidate',
    'first_name' => array(
      '#type' => 'textfield',
      '#title' => t('First Name'),
      '#default_value' =>isset($values['first_name']) ? $values['first_name'] : '',
      '#required' => TRUE,
    ),
    'last_name' => array(
      '#type' => 'textfield',
      '#title' => t('Last Name'),
      '#default_value' =>isset($values['last_name']) ? $values['last_name'] : '',
      '#required' => TRUE,
    ),
    'ags_number' => array(
      '#type' => 'textfield',
      '#title' => t('AGS Number'),
      '#default_value' => isset($values['ags_number']) ? $values['ags_number'] : '',
      '#required' => TRUE,
    ),
    'notes' => array(
      '#type' => 'textarea',
      '#title' => t('Notes'),
      '#default_value' => isset($values['notes']) ? $values['notes'] : '',
      // '#required' => TRUE,
    ),
  );

  $form['compliance'] = array(
    '#type' => 'fieldset',
    '#title' => 'Compliance',
    'isac' => array(
      '#type' => 'checkbox',
      '#title' => t('ISAC'),
      '#default_value' => isset($values['isac']) ? $values['isac'] : 0,
      //'#required' => TRUE,
    ),

    'recruitability' => array(
      '#type' => 'checkbox',
      '#title' => t('Recruitability Guidelines'),
      '#default_value' => isset($values['recruitability']) ? $values['recruitability'] : 0,
      //'#required' => TRUE,
    ),
    'editorial_instructions' => array(
      '#type' => 'textarea',
      '#title' => t('Editorial Instructions'),
      '#default_value' => isset($values['editorial_instructions']) ? $values['editorial_instructions'] : '',
      //'#required' => TRUE,
    ),
  );

  // form follow outcome type
  switch($outcome_type) {
    case 280:
    case 'engagement':
      $date = date('Y-m-d');
      $form['engagement_details'] = array(
        '#type' => 'fieldset',
        '#title' => 'Engagement Details',
        'engagement_date' => array(
          '#type' => 'date_popup',
          '#title' => t('Engagement Date'),
          '#date_format' => 'm/d/Y',
          '#datepicker_options' => array(
            'minDate' => '0',
            //'maxDate' => '0',
            //'yearRange' => "2001:+5",
          ),
          '#default_value' =>isset($values['engagement_date']) ? $values['engagement_date'] : $date,
          //'#required' => TRUE,
        ),
        'section_of_the_act' => array(
          '#type' => 'select',
          '#title' => t('Section of the Act'),
          '#options' => apsjobs_outcomes_section_of_the_act(),
          '#default_value' => isset($values['section_of_the_act']) ? $values['section_of_the_act'] : '',
          //'#required' => TRUE,
          '#empty_value' => '',
        ),
        'employment_status' => array(
          '#type' => 'radios',
          '#title' => t('Employment status'),
          '#options' => apsjobs_outcomes_employment_status(),
          '#default_value' => isset($values['employment_status']) ? $values['employment_status'] : '',
        ),
      );
      $form['outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 280
      );
      $form['label_outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 'engagement',
      );
    break;

    case 282:
    case 'movement':
      $form['to_agency'] = array(
        '#type' => 'fieldset',
        '#title' => 'To Agency',
        'to_agency_name' => array(
          '#type' => 'select',
          '#title' => t('Agency Name'),
          '#options' => apsjobs_outcomes_agency_list(),
          '#empty_value' => '',
          '#default_value' => isset($values['to_agency_name']) ? $values['to_agency_name'] : '',
        ),
        'to_job_level' => array(
          '#type' => 'select',
          '#title' => t('Job Level'),
          '#options' => _get_term_parent_child('aps_job_levels'),
          '#empty_value' => '',
          '#default_value' => isset($values['to_job_level']) ? $values['to_job_level'] : '',
        ),
      );

      $form['from_agency'] = array(
        '#type' => 'fieldset',
        '#title' => 'From Agency',
        'from_agency_name' => array(
          '#type' => 'select',
          '#title' => t('Agency Name'),
          '#options' => apsjobs_outcomes_agency_list(),
          '#empty_value' => '',
          '#default_value' => isset($values['from_agency_name']) ? $values['from_agency_name'] : '',
        ),
        'from_job_level' => array(
          '#type' => 'select',
          '#title' => t('Job Level'),
          '#options' => _get_term_parent_child('aps_job_levels'),
          '#empty_value' => '',
          '#default_value' => isset($values['from_job_level']) ? $values['from_job_level'] : '',
        ),
      );
      $form['outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 282
      );
      $form['label_outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 'movement',
      );

      if (!empty($values['to_job_level'])) {
        $invalid_level_tids = apsjobs_outcomes_invalid_level($values['to_job_level'], 'movement');
        $form['#attached']['js'][] = array(
          'data' => array('apsjobsOutcome' => array('invalidLevelTids' => $invalid_level_tids)),
          'type' =>'setting'
        );
      }
    break;

    case 1237:
    case 'promotion':
      $form['to_agency'] = array(
        '#type' => 'fieldset',
        '#title' => 'To Agency',
        'to_agency_name' => array(
          '#type' => 'select',
          '#title' => t('Agency Name'),
          '#options' => apsjobs_outcomes_agency_list(),
          '#empty_value' => '',
          '#default_value' => isset($values['to_agency_name']) ? $values['to_agency_name'] : '',
        ),
        'to_job_level' => array(
          '#type' => 'select',
          '#title' => t('Job Level'),
          '#options' => _get_term_parent_child('aps_job_levels'),
          '#empty_value' => '',
          '#default_value' => isset($values['to_job_level']) ? $values['to_job_level'] : '',
        ),
      );

      $form['from_agency'] = array(
        '#type' => 'fieldset',
        '#title' => 'From Agency',
        'from_agency_name' => array(
          '#type' => 'select',
          '#title' => t('Agency Name'),
          '#options' => apsjobs_outcomes_agency_list(),
          '#empty_value' => '',
          '#default_value' => isset($values['from_agency_name']) ? $values['from_agency_name'] : '',
          '#required' => TRUE,
        ),
        'from_job_level' => array(
          '#type' => 'select',
          '#title' => t('Job Level'),
          '#options' => _get_term_parent_child('aps_job_levels'),//array('' => 'Select job level') + array(1,2,3),
          '#empty_value' => '',
          '#default_value' => isset($values['from_job_level']) ? $values['from_job_level'] : '',
          '#required' => TRUE,
        ),
      );
      $form['outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 1237,
      );
      $form['label_outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 'promotion',
      );

      if (!empty($values['to_job_level'])) {
        $invalid_level_tids = apsjobs_outcomes_invalid_level($values['to_job_level'],'promotion');
        $form['#attached']['js'][] = array(
          'data' => array('apsjobsOutcome' => array('invalidLevelTids' => $invalid_level_tids)),
          'type' =>'setting'
        );
      }
    break;

    case 281:
    case 'cancellation':
      $form['cancellation'] = array (
        '#type' => 'fieldset',
        '#title' => t('Cancellation'),
        'reason_cancel' => array(
          '#type' => 'textarea',
          '#title' => t('Reason for cancellation'),
          '#default_value' => isset($values['reason_cancel']) ? $values['reason_cancel'] : '',
        ),
      );
      $form['outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 281
      );
      $form['label_outcome_type'] = array(
        '#type' => 'hidden',
        '#value' => 'cancellation',
      );
    break;
  }

  if (isset($args[4]) && $args[4] == 'change') {
    $form['change'] = array (
      '#type' => 'fieldset',
      '#title' => t('Change'),
      'reason_change' => array(
        '#type' => 'textarea',
        '#title' => t('Reason for change'),
        '#default_value' => isset($values['reason_change']) ? $values['reason_change'] : '',
      ),
    );
  }

  //end movement | promotion

  $form['actions'] = array(
    '#type' => 'container',
    'cancel' => array(
      '#type' => 'button',
      '#value' => t('Cancel'),
    ),
    'save_draft' => array(
      '#type' => 'submit',
      '#value' => t('Save Draft'),
    ),
    'submit_approval' => array(
      '#type' => 'submit',
      '#value' => t('Submit for Approval'),
    ),
  );
  return $form;
}
