<?php

function apsjobs_outcomes_create_free_outcomes_form_submit($form, &$form_state) {
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
        drupal_goto('node/' . $args[2]);
      }
      break;

    case 'edit-submit-approval':
      $agency_id = apsjobs_outcomes_agency_save($uid, $values);
      if ($outcome_id) {
        //apsjobs_outcomes_send_email_to_approver($outcome_id, $values['approver_id']);
        drupal_set_message(t('Saved to Awaiting Approval!'), 'status', FALSE);
        drupal_goto('node/' . $args[2]);
      }
      break;
  }
}
