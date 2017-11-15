<?php

function apsjobs_assignments_movements_form($form, &$form_state, $project = NULL) {

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

  field_attach_form('apsjobs_assignments_movements', $project, $form, $form_state);

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
function apsjobs_assignments_movements_form_submit($form, &$form_state) {
  $project = entity_ui_form_submit_build_entity($form, $form_state);
  $project->save();

  drupal_set_message(t('Completed'));
  $form_state['redirect'] = 'admin/structure/apsjobs-entities/apsjobs-assignments-movements';
}



/**
 * Callback function for displaying the individual project page
 */
function apsjobs_assignments_movements_form_view($id) {
  $projects = entity_load('apsjobs_assignments_movements', array($id));
  $apsjobs_assignments_movements = $projects[$id];

  drupal_set_title($apsjobs_assignments_movements->create_time);
  $output = entity_view('apsjobs_assignments_movements', array($apsjobs_assignments_movements));

  return $output;

}