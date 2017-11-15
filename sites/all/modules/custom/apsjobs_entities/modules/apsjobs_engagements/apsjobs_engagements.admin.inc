<?php

function apsjobs_engagements_form($form, &$form_state, $project = NULL) {

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

  field_attach_form('apsjobs_engagements', $project, $form, $form_state);

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
function apsjobs_engagements_form_submit($form, &$form_state) {
  $project = entity_ui_form_submit_build_entity($form, $form_state);
  $project->save();

  drupal_set_message(t('Completed'));
  $form_state['redirect'] = 'admin/structure/apsjobs-entities/apsjobs-engagements';
}



/**
 * Callback function for displaying the individual project page
 */
function apsjobs_engagements_form_view($id) {
  $projects = entity_load('apsjobs_engagements', array($id));
  $apsjobs_engagements = $projects[$id];

  drupal_set_title($apsjobs_engagements->create_time);
  $output = entity_view('apsjobs_engagements', array($apsjobs_engagements));

  return $output;

}