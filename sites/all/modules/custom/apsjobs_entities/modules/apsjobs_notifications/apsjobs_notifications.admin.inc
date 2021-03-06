<?php

function apsjobs_notifications_form($form, &$form_state, $project = NULL) {

  $form['summary'] = array(
    '#title' => t('Summary'),
    '#type' => 'textfield',
    '#default_value' => isset($project->summary) ? $project->summary : '',
    '#required' => TRUE,
    '#weight' => 10,
  );
  
  $form['body'] = array(
    '#title' => t('Body'),
    '#type' => 'textarea',
    '#default_value' => isset($project->body) ? $project->body : '',
    '#required' => TRUE,
    '#weight' => 20,
  );

  field_attach_form('apsjobs_notifications', $project, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => isset($project->id) ? t('Update') : t('Save'),
    '#weight' => 30,
  );

  return $form;
}

/**
 * Submit handler for the project add/edit form.
 */
function apsjobs_notifications_form_submit($form, &$form_state) {
  $project = entity_ui_form_submit_build_entity($form, $form_state);
  $project->save();

  drupal_set_message(t('Completed'));
  $form_state['redirect'] = 'admin/structure/apsjobs-entities/apsjobs-notifications';
}



/**
 * Callback function for displaying the individual project page
 */
function apsjobs_notifications_form_view($id) {
  $id = arg(1);
  $projects = entity_load('apsjobs_notifications', array($id));
  $apsjobs_notifications = $projects[$id];
  drupal_set_title($apsjobs_notifications->summary); 
  //drupal_set_title($apsjobs_notifications->create_time);

  //$uri = entity_uri('apsjobs_notifications', $apsjobs_notifications);
  $output = entity_view('apsjobs_notifications', array($apsjobs_notifications));
  return $output;

}