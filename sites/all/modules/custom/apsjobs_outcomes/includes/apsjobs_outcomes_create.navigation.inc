<?php 

/**
 * Given the current stage the user is on
 * @param type $form_state
 * @return string
 */
function apsjobs_outcomes_create_move_to_next_stage($form, &$form_state) {
  switch ($form_state['stage']) {
    case 'action_an_existing_notice':
      return 'original_notice';
      break;
    case 'original_notice':
      switch ($form_state['values']['outcome_type']) {
        case 'engagement':
          return 'engagement';
          break;
        case 'movement':
          return 'movement';
          break;
        case 'promotion':
          return 'promotion';
          break;
      }
      break;
  }
}

/**
 * Given the previous stage the user is on
 * @param type $form_state
 * @return string
 */
function apsjobs_outcomes_create_move_to_previous_stage($form, &$form_state) {
  switch ($form_state['stage']) {
    case 'original_notice':
      return 'action_an_existing_notice';
      break;
    case 'engagement':
      return 'original_notice';
      break;
    case 'movement':
      return 'original_notice';
      break;
    case 'promotion':
      return 'original_notice';
      break;
  }
}

/**
 * Generate a header which informs the user of which stage they're on.
 *
 * @param type $form
 * @param type $form_state
 * @return type
 */
function apsjobs_outcomes_create_get_header($form, &$form_state) {

  $form_state['stage'] = isset($form_state['stage']) ? $form_state['stage'] : 1;

  $form_stages = array(
    'action_an_existing_notice' => 1,
    'original_notice' => 2,
  );

  if (isset($form_stages[$form_state['stage']])) {
   $current_step = $form_stages[$form_state['stage']];
  }
  else {
   $current_step = 1;
  }
  print_r($form_state); exit;
  $stages = array(
    1 => array('data' => '1. Info'),
    2 => array('data' => '2. Goal'),
    3 => array('data' => '3. Plan'),
    4 => array('data' => '4. Membership'),
  );

 for($i = 1; $i<$current_step; $i++){
    $stages[$i]['class'] = array('prestep');
 }

  $stages[$current_step]['class'] = array('active');

  $stages_list = theme('item_list', array('items' => $stages));

  $form['header'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#value' => $stages_list,
  );

  return $form;

}

/**
 * Get list header item.
 *
 * @param type $outcome_type
 * @return type
 */
function apsjobs_outcomes_get_outcome_type_header($outcome_type) {
  $states = array();

  return $states;
}