<?php
/**
  * @file
  * Handles the form submission of the Outcomes Form
  */

/**
  * Handles what to do with the submitted form depending on what stage has been
  * completed.
  *
  * @see apsjobs_outcomes_create_form()
  * @see apsjobs_outcomes_create_form_validate()
  *
  * @param type $form
  * @param type $form_state
  */
function apsjobs_outcomes_create_form_submit($form, &$form_state) {
  global $user;
  $uid = $user->uid;
  $values = $form_state['values'];
  $args = arg();
  // $action = ($values['outcome_nid'] != NULL) ? 'Updated' : 'Saved';
  switch ($form_state['clicked_button']['#id']) {

    case 'edit-save-draft':
      $outcome_id = apsjobs_outcomes_action_outcome_save($uid, $values, 'draft');
      if ($outcome_id) {
        drupal_set_message(t('Outcome saved to Draft!'), 'status', FALSE);
        if ($args[1] != 'create-free-outcome') {
          drupal_goto('node/' . $args[2]);
        }
        else {
          drupal_goto('aps-agency/dashboard');
        }
      }
      break;

    case 'edit-submit-approval':
      $outcome_id = apsjobs_outcomes_action_outcome_save($uid, $values, 'needs_review');
      if ($outcome_id) {
        //apsjobs_outcomes_send_email_to_approver($outcome_id, $values['approver_id']);
        drupal_set_message(t('Saved to Awaiting Approval!'), 'status', FALSE);
        if ($args[1] != 'create-free-outcome') {
          drupal_goto('node/' . $args[2]);
        }
        else {
          drupal_goto('aps-agency/dashboard');
        }
      }
      break;
  }
}
