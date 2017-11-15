<?php

function apsjobs_signup_form($form, &$form_state) {

  $values = isset($form_state['agency_info']) ? $form_state['agency_info'] : array();
  $gov_agency = isset($form_state['input']['gov_agency']) ? $form_state['input']['gov_agency'] : '';
  $domain = apsjobs_signup_get_agency_domain($gov_agency);

  $form['agency_info']['header'] = array(
    '#type' => 'item',
    '#markup' => '
    <a href="#">' . t('How to know whether your agency already has a login account?') . '</a>' .
    '<p>There are two types of Individual Access, they are: etc. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean euis mod bibendum</p>',
  );

  $agencies = apsjobs_signup_load_agency();

  $form['agency_info']['gov_agency'] = array(
    '#type' => 'select',
    '#title' => t('Agencies'),
    '#options' => $agencies,
    '#default_value' => isset($values['gov_agency']) ? $values['gov_agency'] : '',
    '#required' => TRUE,
  );

  $form['agency_info']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('HR department generic email address ending in ' . $domain),
    '#default_value' => isset($values['email']) ? $values['email'] : '',
    '#required' => TRUE,
  );

  $form['agency_info']['authorise'] = array(
    '#type' => 'radios',
    '#title' => t('To create a buyer account you need to be a government employee or be authorised by a government employee.'),
    '#options' => array('1'=>'I am an employee under the Commonwealth Public Service Act (1999) or under equivalent state or territory legislation and need access to the Digital Marketplace to perform my role.'),
    '#required' => TRUE,
  );

  $form['agency_info']['term'] = array(
    '#type' => 'item',
    '#markup' => t('<p>By creating an account you confirm your acceptance of our <a href="#">Term of Use</a></p>'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Create Agency Account"),
  );

  return $form;
}

function apsjobs_signup_create_password_form($form, &$form_state) {
  
  // $form['agency_pass']['header'] = array(
  //   '#type' => 'item',
  //   '#markup' => t("<p>Congratulations you have validated your Department's address</p>"),
  // );

  $form['agency_pass']['username'] = array(
    '#type' => 'hidden',
    '#value' => isset($_GET['username']) ? $_GET['username'] : '',
  );

  $form['agency_pass']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['agency_pass']['message'] = array(
    '#type' => 'item',
    '#markup' => '',
  );

  $form['agency_pass']['confirm_password'] = array(
    '#type' => 'password',
    '#title' => t('Confirm Password'),
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Continue"),
  );

  return $form;
}

function apsjobs_signup_agency_account_detail_form($form, &$form_state) {
  global $user;
  $current_user = user_load($user->uid);
  if (in_array('APS Agency User', $current_user->roles) && !empty($current_user->field_agency_register)) {
    $agency_id = $current_user->field_agency_register[LANGUAGE_NONE][0]['target_id'];
    $agency_domain = apsjobs_signup_get_agency_domian($agency_id);
  }
  else {
    $agency_domain = '';
  }
  $form['agency_info'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );
  $form['agency_info']['username'] = array(
    '#type' => 'item',
    '#title' => 'Username:',
    '#markup' => isset($current_user) ? $current_user->name : 'email',
  );
  $form['agency_info']['password'] = array(
    '#type' => 'item',
    '#title' => 'Password:',
    '#markup' => 'XXXXXX',
  );

  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t("<p>These login details could be used by any of the staff in your HR team but users using these login details won't be able to access to xxxxxx.</p>
    <p>You need to create at least one HR Manager account linked to your agency to approve new jobs and notices added manually, and access the redeployment register.</p>"),
  );
  $form['approver_amount'] = array(
    '#type' => 'hidden',
    '#value' => '1',
  );
  $form['agency_domain'] = array(
    '#type' => 'hidden',
    '#value' => $agency_domain,
  );
  //
  $form['agency_detail'] = array(
    '#type' => 'fieldset',
    '#title' => t('New HR Approver Account'),
    '#collapsible' => FALSE,
  );
  $form['agency_detail']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => 'First name',
    '#name' => 'first_name[]',
    '#size' => 60,
		'#maxlength' => 128,
		// '#required' => TRUE,
  );
  $form['agency_detail']['surname'] = array(
    '#type' => 'textfield',
    '#title' => 'Surname',
    '#name' => 'surname[]',
    '#size' => 60,
		'#maxlength' => 128,
		// '#required' => TRUE,
  );
  $form['agency_detail']['job_level'] = array(
    '#type' => 'select',
    '#title' => 'Job Level',
    '#name' => 'job_level[]',
    '#options' => apsjobs_signup_get_job_levels(),
		// '#required' => TRUE,
  );
  $form['agency_detail']['employee_number'] = array(
    '#type' => 'textfield',
    '#name' => 'employee_number[]',
    '#title' => 'Employee Number',
    '#size' => 60,
		'#maxlength' => 128,
		// '#required' => TRUE,
  );
  $form['agency_detail']['email'] = array(
    '#type' => 'textfield',
    '#name' => 'email[]',
    '#title' => t('HR department generic email address ending in ' . $agency_domain),
    //'#default_value' => isset($values['email']) ? $values['email'] : '',
    // '#required' => TRUE,
  );
  $nominate_approvers = apsjobs_signup_get_nominate_approvers();
  if (count($nominate_approvers) > 1) {
    $form['agency_detail']['nominate_approver[]'] = array(
      '#type' => 'select',
      '#name' => 'nominate_approver[]',
      '#title' => 'Nominate Approver',
      '#options' => $nominate_approvers,
      // '#required' => TRUE,
    );
  }
  
  //
  $form['btn_group'] = array(
    '#type' => 'container',
  );
  $form['btn_group']['add_another_hr_approver'] = array(
		'#markup' => l(t('+ Add Another HR Approver'), '#', array('attributes' => array('class' => 'link-add'))),
	);
  $form['btn_group']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Submit Request"),
  );
  
  return $form;
}

function apsjobs_signup_create_password_approver_form($form, &$form_state) {
 
  $form['approver_pass']['username'] = array(
    '#type' => 'hidden',
    '#value' => isset($_GET['username']) ? $_GET['username'] : '',
  );

  $form['approver_pass']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['approver_pass']['message'] = array(
    '#type' => 'item',
    '#markup' => '',
  );

  $form['approver_pass']['confirm_password'] = array(
    '#type' => 'password',
    '#title' => t('Confirm Password'),
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Continue"),
  );

  return $form;
}

//
function apsjobs_signup_load_agency() {
  $nodes = node_load_multiple(array(), array('type' => 'aps_agencies'));
  $agencies[''] = t('Select Agency');
  foreach ($nodes as $nid => $node) {
    if (!empty($node->field_agency_domain)) {
      $key = $nid . ':' . $node->field_agency_domain[LANGUAGE_NONE][0]['value'];
      $agencies[$key] = $node->title;
    }
  }
  return $agencies;
}

function apsjobs_signup_get_nominate_approvers() {
  global $user;
  $current_user = user_load($user->uid);
  $nominate_approvers = array(
    '' => t('Select a Nominate Approver'),
  );
  if (!empty($current_user->field_agency_register)) {
    $agency_id = $current_user->field_agency_register[LANGUAGE_NONE][0]['target_id'];  
    //load users by role approver
    $query = 'SELECT ur.uid FROM {users_roles} AS ur WHERE ur.rid = :rid';
    $result = db_query($query, array(':rid' => 10));
    $uids = $result->fetchCol();
    //load users by condtions
    if (!empty($uids)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'user', '=')
        ->propertyCondition('uid', $uids, 'IN')
        ->propertyCondition('status', 1, '=')
        ->fieldCondition('field_agency_register', 'target_id', $agency_id);
      $results = $query->execute();      
      
      if (!empty($results['user']) && !empty(array_keys($results['user']))) {
        $uids = array_keys($results['user']);
        $approvers = entity_load('user', $uids);
        foreach($approvers as $uid => $approver) {
          if (empty($approver->field_first_name) || empty($approver->field_last_name)) {
            $nominate_approvers[$uid] = $approver->mail;  
          } else {
            $nominate_approvers[$uid] = $approver->field_first_name[LANGUAGE_NONE][0]['value'] . ' ' . $approver->field_last_name[LANGUAGE_NONE][0]['value'];  
          }
        }
      }
    }
  } 
  return $nominate_approvers;
}

function apsjobs_signup_get_job_levels() {
  $name = 'aps_job_levels';
  $terms = _get_term_parent_child($name);
  return _get_term_parent_child($name);
}

function _get_term_parent_child($voc_name) {
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

function _get_taxonomy_terms($voc_name) {
  $voc = taxonomy_vocabulary_machine_name_load($voc_name);
  $tree = taxonomy_get_tree($voc->vid);
  $results = array();
  foreach ($tree as $key => $value) {
    $tree[$value->tid] = $value;
    unset($tree[$key]);
  }
  foreach ($tree as $value) {
    $prefix = '';
    $parent = $value->parents[0];
    while ($parent != 0) {
      $prefix .= '---';
      $parent = $tree[$parent]->parents[0];
    }
    $results[$value->tid] = $prefix . $value->name;
  } 

  return $results;
}

function apsjobs_signup_get_agency_domain($value) {
  if (!empty($value)) {
    $gov_agency_parts = explode(':', $value);
    $domain = end($gov_agency_parts);
  }
  return isset($domain) ? $domain : '';
}

function apsjobs_signup_get_agency_id($value) {
  if (!empty($value)) {
    $gov_agency_parts = explode(':', $value);
    $aid = reset($gov_agency_parts);
  }
  return isset($aid) ? $aid : '';
}

function apsjobs_signup_get_agency_domian($agency_id) {
  $agency = node_load($agency_id);
  return $agency->field_agency_domain[LANGUAGE_NONE][0]['value'];
}