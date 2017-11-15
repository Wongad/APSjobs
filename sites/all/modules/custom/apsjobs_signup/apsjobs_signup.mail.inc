<?php

function apsjobs_signup_send_validate_mail($email, $code) {
  // $user = user_load_by_mail($email);
  // $code = $code;
  $variables = array(
    'username' => $email,
    'code' => $code,
  );
  $content = theme_render_template(
    drupal_get_path('module', 'apsjobs_signup') . '/templates/user-register-validate.tpl.php',
    $variables
  );
  $params = array('subject' => 'Welcome to APSJobs', 'body' => $content);
  drupal_mail('apsjobs_signup', 'apsjobs_mail', $email, LANGUAGE_NONE, $params, $from = NULL, $send = TRUE);
  drupal_get_messages('error');
}

function apsjobs_signup_send_validate_approver_mail($email, $code) {
  // $user = user_load_by_mail($email);
  $code = $code;
  $variables = array(
    'username' => $email,
    'code' => $code,
  );
  $content = theme_render_template(
    drupal_get_path('module', 'apsjobs_signup') . '/templates/user-approver-register-validate.tpl.php',
    $variables
  );
  $params = array('subject' => 'Welcome to APSJobs', 'body' => $content);
  drupal_mail('apsjobs_signup', 'apsjobs_mail', $email, LANGUAGE_NONE, $params, $from = NULL, $send = TRUE);
  drupal_get_messages('error');
}

function apsjobs_signup_send_notice_approver_mail($approver_email, $agency_email) {
  $variables = array(
    'approver_name' => $approver_email,
    'agency_name' => $agency_email,
  );
  $content = theme_render_template(
    drupal_get_path('module', 'apsjobs_signup') . '/templates/user-approver-notification.tpl.php',
    $variables
  );
  $params = array('subject' => 'APSJobs Notification', 'body' => $content);
  drupal_mail('apsjobs_signup', 'apsjobs_mail', $approver_email, LANGUAGE_NONE, $params, $from = NULL, $send = TRUE);
  drupal_get_messages('error');
}

function apsjobs_signup_resend_email_validate_account() {  
  $user_mail = $_GET['email'];
  if(isset($user_mail)) {
    $create_time = time();
    $code = md5(($user_mail) . $create_time);

    $num_updated = db_update('apsjobs_agency_user_validate')
      ->fields(array(
        'code' => $code,
        'created' => $create_time,
        'active_status' => 0,
      ))
      ->condition('username', $user_mail)
      ->execute();
    apsjobs_signup_send_validate_mail(trim($user_mail), $code);
    drupal_set_message(t("A validate mail has been resent to your E-mail address."), 'status');
    drupal_goto('/user/login');
  }
}

function apsjobs_forms_mail_test() {
  $to = 'abc03.tester@gmail.com';
  $language = LANGUAGE_NONE;
  
  $params = array(
    'subject' => 'My Email',
    'body' => "<p>Hello World</p>",
  );

  // Send out the e-mail.
  drupal_mail('apsjobs_signup', 'apsjobs_mail', $to, language_default(), $params);
  echo "string"; exit;
}