<?php
/**
 *
 *
 * @return array
 */

module_load_include('inc', 'apsjobs_services', 'resources/vacancy_resource');
module_load_include('inc', 'apsjobs_services', 'resources/taxonomy_resource');
module_load_include('inc', 'apsjobs_services', 'resources/agency_resource');

function _services_apsjobs_resources() {

  $resources = array(
    '#api_version' => 3002,
  );

  $resources += apsjobs_vacancy_resource_definition();
  $resources += apsjobs_taxonomy_resource_definition();
  $resources += apsjobs_agency_resource_definition();

  return $resources;
}