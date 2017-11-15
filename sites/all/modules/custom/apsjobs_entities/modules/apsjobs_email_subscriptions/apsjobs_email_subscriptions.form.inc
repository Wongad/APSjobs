<?php
/**
 * Form for add/edit entity
 */
function aps_email_subscriptions_form($form, &$form_state, $project = NULL) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#suffix' => '<br>This actions cannot undone or <a href="/admin/structure/apsjobs_rest_view/configs">Back</a>'
  );
  return $form;
}

/**
 * Validate handler for add/edit entity form.
 */
function aps_email_subscriptions_form_validate($form, &$form_state) {
  #Code...
}

/**
 * Submit handler for add/edit entity form.
 */
function aps_email_subscriptions_form_submit($form, &$form_state) {
  drupal_set_message('Cron Completed');
}
