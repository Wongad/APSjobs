<?php

function apsjobs_signup_form_validate($form, &$form_state) {
  // Valid email
  if (empty($form_state['input']['gov_agency'])) {
    form_set_error('gov_agency', 'Agency was not selected');
  }
  $gov_agency = $form_state['values']['gov_agency'];
  $email = $form_state['values']['email'];
  if (!empty($gov_agency)) {
    $gov_agency_parts = explode(':', $gov_agency);
    $domain = end($gov_agency_parts);
    if (strpos($email, $domain) === FALSE) {
      form_set_error('email', t('Email not ending in ' . $domain));
    }
  }
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', 'Email is invalid!');
  }
  if (user_load_by_mail($form_state['values']['email'])) {
    form_set_error('email', 'This email is already in use!');
  }
}

function apsjobs_signup_create_password_form_validate($form, &$form_state) {
  // Compare password and password confirm
  if ($form_state['values']['password'] !== $form_state['values']['confirm_password']) {
    form_set_error('confirm_password', 'Password Confirm is invalid!');
  }
}

function apsjobs_signup_agency_account_detail_form_validate($form, &$form_state) {
  $inputs = $form_state['input'];
  $domain = $inputs['agency_domain'];
  if (!isset($domain)) {
    form_set_error(t("You are not belong to any Agency. Please join first."));
  }
  if (empty($inputs['email']) && empty($inputs['nominate_approver'])) {
    form_set_error(t("Please add new an approver or select an exist approver"));
  }
  // for ($i = 0; $i < $amount; $i ++) {
  //   $email = $inputs['email'][$i];
  //   if (strpos($email, $domain) === FALSE || !valid_email_address($email)) {
  //     //wrong email
  //   }
  // }
}

function apsjobs_signup_create_password_approver_form_validate($form, &$form_state) {
  // Compare password and password confirm
  if ($form_state['values']['password'] !== $form_state['values']['confirm_password']) {
    form_set_error('confirm_password', 'Password Confirm is invalid!');
  }
}

function apsjobs_signup_validate_account() {
  #check validate in 3 days.
  $params = drupal_get_query_parameters();
  $current_time = time() - 3 * 24 * 3600 ;
  if (isset($params['code']) && isset($params['username'])) {
    $code = $params['code'];
    $username = $params['username'];
    $row = db_select("apsjobs_agency_user_validate", "p")
      ->fields('p')
      ->condition('username', $username, '=')
      ->condition('code', $code, '=')
      ->condition('active_status', 0, '=')
      ->execute()
      ->fetchAll();
    if ($row) {
      $active_status = $row[0]->active_status;
      if (!$active_status) {
        // $user = user_load_by_name($username);
        // $user->status = 1;
        // user_save($user);

        $pid = db_update('apsjobs_agency_user_validate')
          ->fields(array(
              'active_status' => 1
            ))
          ->condition('username', $username, '=')
          ->condition('code', $code, '=')
          ->condition('created', $current_time, '>=')
          ->execute();
        
        drupal_set_message(t("Congratulations you have validated your Department's address!"), 'status');
        drupal_goto('create-password', array('query' => array('username' => $username)));
      }
    }
    else {
      drupal_set_message(t("This link is invalid!"), 'error');
      drupal_goto('user/login');
    }
  }
  drupal_set_message(t("This link is invalid!"), 'error');
  drupal_goto('user/login');
}

function apsjobs_signup_validate_approver_account() {
  #check validate in 3 days.
  $current_time = time() - 3 * 24 * 3600 ;
  $params = drupal_get_query_parameters();
  if (isset($params['code']) && isset($params['username'])) {
    $code = $params['code'];
    $username = $params['username'];
    $row = db_select("apsjobs_agency_user_validate", "p")
      ->fields('p')
      ->condition('username', $username, '=')
      ->condition('code', $code, '=')
      ->condition('active_status', 0, '=')
      ->execute()
      ->fetchAll();
    if ($row) {
      $active_status = $row[0]->active_status;
      if (!$active_status) {

        $user = user_load_by_name($username);
        $user->status = 1;
        user_save($user);

        $pid = db_update('apsjobs_agency_user_validate')
          ->fields(array(
            'active_status' => 1
          ))
          ->condition('username', $username, '=')
          ->condition('code', $code, '=')
          ->condition('created', $current_time, '>=')
          ->execute();
        
        drupal_set_message(t("Your email has been validated!"), 'status');
        drupal_goto('create-password-approver', array('query' => array('username' => $username)));
      }
    }
    else {
      drupal_set_message(t("This link is invalid!"), 'error');
      drupal_goto('user/login');
    }
  }
  drupal_set_message(t("This link is invalid!"), 'error');
  drupal_goto('user/login');
}