<?php

function apsjobs_home_statistics() {
  $statistics = array();
  $statistics['locations'] = apsjobs_statistic_locations();
  $statistics['agencies'] = apsjobs_statistic_agencies();
  $statistics['vacancies'] = apsjobs_statistic_vacancies();
  return $statistics;
}

function apsjobs_statistic_locations() {
  return 142;
}

function apsjobs_statistic_agencies() {
  //return 250;
  $result = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', 'aps_agencies','=')
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result;
}

function apsjobs_statistic_vacancies() {
  //return 10056;
  $result = db_select('workbench_moderation_node_history', 'wmnh')
    ->fields('wmnh', array('nid'))
    ->condition('is_current', 1,'=')
    ->condition('state', 'published', '=')
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result;
}