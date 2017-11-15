<?php

function apsjobs_signup_form_submit($form, &$form_state) {
  // Generate token_name and add values to probody_user_activation table
  $account_info = $form_state['values'];
  $agency_id = apsjobs_signup_get_agency_id(trim($account_info['gov_agency']));
  $create_time = time();
  $code = md5($account_info['email'] . $create_time);
  
  if (db_table_exists('apsjobs_agency_user_validate')) {
    $gauv = get_gauv_by_email($account_info['email']);
    if (empty($gauv)) {
      $pid = db_insert('apsjobs_agency_user_validate')
        ->fields(array(
          'username' => trim($account_info['email']),
          'gov_agency_id' => $agency_id,
          'code' => $code,
          'active_status' => 0,
          'created' => $create_time,
        ))
        ->execute();
      apsjobs_signup_send_validate_mail(trim($account_info['email']), $code);
      drupal_set_message(t("A verification email has been sent to your E-mail address."), 'status');
    }
    elseif($gauv->active_status == 0) {
      $path = '/resend-email-validate-account?email=' . $account_info['email'];
      $markup = t('This email is existed! Please <a href="' . $path . '">Click here</a> to resend a verification email');
      drupal_set_message($markup, 'status');
    }
    elseif(!isset($gauv->uid)) {
      //validated email but havenot created password yet.
      $path = '/resend-email-validate-account?email=' . $account_info['email'];
      $markup = t('Email was validated but password has not created yet. Please <a href="' . $path . '">Click here</a> to resend a verification email again');
      drupal_set_message($markup, 'status');
      // drupal_set_message(t("E-mail address existed."), 'error');
    }
  }
  drupal_goto('<front>');
}

//create agency admin account->create password
function apsjobs_signup_create_password_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $gauv = get_gauv_by_email($values['username']);
  $agency_id = $gauv->gov_agency_id;
  //$agency = node_load($agency_id);

  $fields = array(
    'name' => trim($values['username']),
    'mail' => trim($values['username']),
    'pass' => $values['password'],
    'status' => 1,
    'init' => $values['username'],
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
    ),
  );
  $exist_account = user_load_by_name($values['username']);

  if (empty($exist_account)) {
    $account = user_save('', $fields);
    if ($account) {
      $account_wrapper = entity_metadata_wrapper('user', $account);
      $account_wrapper->roles[] = 9;
      $account_wrapper->field_agency_register = $agency_id;
      $account_wrapper->save();
      //update account created
      $pid = db_update('apsjobs_agency_user_validate')
      ->fields(array(
          'uid' => $account->uid
        ))
      ->condition('username', $account->name, '=')
      ->execute();
      //login user account
      $uid = user_authenticate($account->name, $values['password']);
      if($uid) {
        $info = array('uid' => $uid);
        user_login_submit(array(), $info);
      }
    }
    drupal_set_message(t("Congratulations you have validated your Department's address!"), 'status');
    drupal_goto('agency-account-details');
  }
}

function apsjobs_signup_agency_account_detail_form_submit($form, &$form_state) {
  global $user;
  $current_user = user_load($user->uid);
  $agency_id = $current_user->field_agency_register[LANGUAGE_NONE][0]['target_id'];
  $inputs = $form_state['input'];
  $amount = $inputs['approver_amount'];
  $new_approver_amount = count($inputs['email']);
  $domain = $inputs['agency_domain'];
  $approvers_selected = $inputs['nominate_approver'];
  $approvers = array();
  $accounts = array();
  $field_names = array(
    'first_name',
    'surname',
    'job_level',
    'employee_number',
    'email',
    //'nominate_approver'
  );
  if (!empty($inputs)) {
    for ($i = 0; $i < $new_approver_amount; $i ++) {
      foreach ($field_names as $field_name) {
        $accounts[$i][$field_name] = isset($inputs[$field_name][$i]) ? $inputs[$field_name][$i] : '';
      }
    }
  }
  foreach ($accounts as $account) {
    if (strpos($account['email'], $domain) !== FALSE && valid_email_address($account['email'])) {
      $approver_id = apsjobs_signup_create_approver_user($account, $agency_id);
      if($approver_id) {
        $approvers[] = $approver_id;
        $new_approvers[] = $approver_id;
      }
      else {
        $exist_emails[] = $account['email'];
      }
    }
    else {
      //reject wrong email
      $wrong_emails[] = $account['email'];
    }
  }
  foreach ($inputs['nominate_approver'] as $key => $ap_id) {
    if (isset($ap_id)) {
      $approver = user_load($ap_id);
      apsjobs_signup_send_notice_approver_mail(trim($approver->mail), $user->mail);
      $approvers[] = $ap_id;
    }
  }
  //link approvers to agency user
  $account_wrapper = entity_metadata_wrapper('user', $current_user);

  $current_approvers = empty($account_wrapper->field_approvers->value()) ? array() : $account_wrapper->field_approvers->value();
  foreach ($current_approvers as $approver) {
    $current_approvers_uid[] = $approver->uid;
  }
  $results = array();
  if (isset($current_approvers_uid)) {
    foreach($current_approvers_uid as $key => $value) {
      if (!empty($value))
        $results[$value] = $value;
    }
  }
  foreach($approvers as $key => $value) {
    if (!empty($value))
      $results[$value] = $value;
  }
  $update_approvers = array_keys($results);

  $account_wrapper->field_approvers->set($update_approvers);
  $account_wrapper->save();

  if (isset($exist_emails) && !empty($exist_emails)) {
    drupal_set_message(t("Email " . implode(', ', $exist_emails) . ' existed'), 'warning');
  }
  if(isset($new_approvers) && !empty($new_approvers)) {
    drupal_set_message(t("A verification email has been sent to the address you provided. Your agency HR approver can complete there signup by following the instructions in this email."), 'status');
  }
  
  if (!(count($approvers_selected) == 1 && $approvers_selected[0] == '')) {
    drupal_set_message(t("Your nominated approver has been notified of your selection"), 'status');
  }
  if (!empty($wrong_emails)){
    drupal_set_message(t("Rejected Approver's invalid email address: " . implode(", ", $wrong_emails)), 'error');
  }
  drupal_goto('aps-agency/dashboard');
}

function apsjobs_signup_create_password_approver_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = array(
    'pass' => $values['password'],
    'status' => 1,
  );
  $user = user_load_by_name($values['username']);
  $account = user_save($user, $fields);

  $uid = user_authenticate($account->name, $values['password']);
  if($uid) {
    $info = array('uid' => $uid);
    user_login_submit(array(), $info);
  }
  drupal_set_message(t("Congratulations you have validated your Department's address!"), 'status');
  drupal_goto('user');
}

function apsjobs_signup_create_approver_user($account_info, $agency_id) {
  global $user;
  $agency_user_email = $user->mail;
  //create 
  $exist_account = user_load_by_name($account_info['email']);
  if ($exist_account) {
    if ($exist_account->status == 1) {
      apsjobs_signup_send_notice_approver_mail(trim($account_info['email']), $agency_user_email);
    } else {
      //account existed but was not activated
    }
    return false;
  } else {
    $fields = array(
      'name' => trim($account_info['email']),
      'mail' => trim($account_info['email']),
      //'pass' => $values['password'],
      'status' => 0,
      'init' => $account_info['email'],
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      ),
    );
    $account = user_save('', $fields);
    if ($account) {
      $account_wrapper = entity_metadata_wrapper('user', $account);
      $account_wrapper->roles[] = 10;
      $account_wrapper->field_first_name = $account_info['first_name'];
      $account_wrapper->field_last_name = $account_info['surname'];
      $account_wrapper->field_employee_number = $account_info['employee_number'];
      $account_wrapper->field_agency_register = $agency_id;
      $account_wrapper->field_job_level->set(array($account_info['job_level']));
      $account_wrapper->save();
    }
    apsjobs_signup_send_validate_email_to_approver($account_info['email'], $agency_id);
    return $account->uid;
  }
}

function apsjobs_signup_send_validate_email_to_approver($email, $agency_id) {
  //send mail validate account
  $create_time = time();
  $code = md5($email . $create_time);
  if (db_table_exists('apsjobs_agency_user_validate')) {
    $gauv = get_gauv_by_email($email);
    if (empty($gauv)) {
      $pid = db_insert('apsjobs_agency_user_validate')
        ->fields(array(
          'username' => trim($email),
          'gov_agency_id' => $agency_id,
          'code' => $code,
          'active_status' => 0,
          'created' => $create_time,
        ))
        ->execute();
      apsjobs_signup_send_validate_approver_mail(trim($email), $code);
    }
    elseif($gauv->active_status == 0) {
      $num_updated = db_update('apsjobs_agency_user_validate')
        ->fields(array(
          'code' => $code,
          'created' => $create_time,
        ))
        ->condition('username', $email)
        ->execute();
      apsjobs_signup_send_validate_approver_mail(trim($email), $code);
    }
    else {
      drupal_set_message(t($email . " existed."), 'error');
    }
  }
}

function get_gauv_by_email($email) {
  $result = db_select('apsjobs_agency_user_validate', 'gauv')
    ->fields('gauv')
    ->condition('username', $email,'=')
    ->execute()
    ->fetchObject();
  return $result;
}