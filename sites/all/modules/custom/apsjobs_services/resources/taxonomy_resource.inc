<?php

function apsjobs_taxonomy_resource_definition() {
  $definition['get_taxonomies'] = array(
    'operations' => array(
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'apsjobs_services', 'name' => 'resources/vacancy_resource'),
        'help'   => t('Returns terms of Working hours, Engagement types, Specific Considerations, Job Levels, Job Categories, Clearance Levels taxonomies.'),
        'callback' => 'apsjobs_services_get_taxonomies',
        'args'     => array(
          array(
            'name' => 'machine_name',
            'optional' => TRUE,
            'type' => 'string',
            'description'  => t('The taxonomy machine name to get.'),
            'default value' => '*',
            'source' => array('param' => 'machine_name'),
          ),
        ),
        'access callback' => 'apsjobs_services_taxonomy_resource_access',
        'access arguments' => array('access content'),        
        'access arguments append' => TRUE,
      ),
    ),  
  );

  return $definition;
}

function apsjobs_services_taxonomy_resource_access() {
  if (user_is_logged_in())
    return true;
  return false;
}

function apsjobs_services_get_taxonomies($machine_name) {
  $taxonomies = array();
  if ($machine_name == '*') {
    //return all taxonomies for create vacancy
    $taxonomies_mc = array(
      'aps_working_hours',
      'aps_engagement_types',
      'aps_specific_considerations',
      'aps_job_levels',
      'aps_job_categories',
      'aps_clearance_levels'
    );
    foreach($taxonomies_mc as $tmc) {
      $voc = taxonomy_vocabulary_machine_name_load($tmc);
      $tree = taxonomy_get_tree($voc->vid);
      $taxonomies[$tmc] = $tree;
    }    
  }
  else {
    $voc = taxonomy_vocabulary_machine_name_load($machine_name);
    if (empty($voc)) {
      return services_error(t('No vocabulary with machine name @name found.', array('@name' => $machine_name)), 404);
    }
    else {
      $voc = taxonomy_vocabulary_machine_name_load($machine_name);
      $tree = taxonomy_get_tree($voc->vid);
      $taxonomies[$machine_name] = $tree;
    }
  }
  return $taxonomies;
}