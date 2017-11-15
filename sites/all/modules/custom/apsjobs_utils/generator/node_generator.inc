<?php

function apsjobs_utils_generate_node_revision($node_info, $active_db = 'default') {
  apsjobs_utils_active_default_db($active_db);
    
  $node = node_load($node_info['nid']);
  $node->nid = $node_info['nid'];
  $node->type = isset($node_info['type']) ? $node_info['type'] : 'article';
  $node->title = isset($node_info['title']) ? $node_info['title'] : '';
  $node->uid = isset($node_info['uid']) ? $node_info['uid'] : 1;
  //$node->revision = 1;
  $node->log = isset($node_info['log']) ? $node_info['log'] : "";
  $node->promote = isset($node_info['promote']) ? $node_info['promote'] : 0;
  $node->comment = isset($node_info['comment']) ? $node_info['comment'] : 0;
  $node->moderate = isset($node_info['moderate']) ? $node_info['moderate'] : 0;
  $node->sticky = isset($node_info['sticky']) ? $node_info['sticky'] : 0;
  $node->translate = isset($node_info['translate']) ? $node_info['translate'] : 0;
  $node->created = isset($node_info['created']) && $node_info['created'] ? $node_info['created'] : time();
  $node->changed = isset($node_info['changed']) && $node_info['changed'] ? $node_info['changed'] : time();
  $node->language = isset($node_info['language']) ? $node_info['language'] :
    (language_default('language') == 'en' ? LANGUAGE_NONE : language_default('language'));
  $node->vid = NULL;
  apsjobs_utils_generate_fields($node_info, $node, 'node', $node->type);
  _node_save_revision($node, $node->uid);
  db_update("node_revision")->condition("nid", $node->nid)->condition("vid", $node->vid)->fields(array('timestamp' => $node_info['timestamp'], 'uid' => $node->uid))->execute();
  apsjobs_utils_deactive_default_db($active_db);
  
  return $node;    
}

function apsjobs_utils_generate_node($node_info, $parent_node = NULL, $node_id = NULL) {
  if ($node_id) {
    $node = node_load($node_id);
  }
  else {
    $node = new stdClass();
    $node->nid = NULL;
    $node->tnid = $parent_node ? $parent_node->nid : NULL;
    $node->type = isset($node_info['type']) ? $node_info['type'] : 'article';
  }
  node_object_prepare($node);
  $type = node_type_get_type($node);
  $node->title = isset($node_info['title']) ? (is_array($node_info['title']) ? $node_info['title'][0] : $node_info['title']) : '';
  $node->uid = isset($node_info['uid']) ? $node_info['uid'] : 1;
  $node->revision = isset($node_info['revision']) ? $node_info['revision'] : 0;
  $node->promote = isset($node_info['promote']) ? $node_info['promote'] : 0;
  $node->comment = isset($node_info['comment']) ? $node_info['comment'] : 0;
  $node->moderate = isset($node_info['moderate']) ? $node_info['moderate'] : 0;
  $node->sticky = isset($node_info['sticky']) ? $node_info['sticky'] : 0;
  $node->translate = isset($node_info['translate']) ? $node_info['translate'] : 0;
  $node->created = isset($node_info['created']) && $node_info['created'] ? $node_info['created'] : time();
  $node->changed = isset($node_info['changed']) && $node_info['changed'] ? $node_info['changed'] : time();
  $node->language = isset($node_info['language']) ? $node_info['language'] :
    (language_default('language') == 'en' ? LANGUAGE_NONE : language_default('language'));

  apsjobs_utils_generate_fields($node_info, $node, 'node', $node->type);
  node_save($node);
  return $node;
}

function apsjobs_utils_generate_fields($node_info, $object, $object_type, $bundle) {
  $field_types = field_info_field_types();
  $instances = field_info_instances($object_type, $bundle);
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    if (isset($node_info[$field_name])) {
      $field = field_info_field($field_name);
      $field_values = $node_info[$field_name];
      $module = $field_types[$field['type']]['module'];
      if($module !== 'field_collection') {
        $object_field = array();
        foreach ($field_values as $field_value) {
          $generator_function = 'apsjobs_utils_' . $module . '_generate';
          if (function_exists($generator_function)) {
            $result = $generator_function($field_value, $object, $field, $instance, $bundle);
            if ($result) {
              $object_field[] = $result;
            }
          }
        }
        $object->{$field['field_name']} = array(
          LANGUAGE_NONE => $object_field,
        );
      }
    }
  }
}

function apsjobs_utils_add_field_collection_fields($node_info, $object, $object_type, $bundle) {
  $field_types = field_info_field_types();
  $instances = field_info_instances($object_type, $bundle);
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    if (isset($node_info[$field_name])) {
      $field = field_info_field($field_name);
      $field_values = $node_info[$field_name];
      $module = $field_types[$field['type']]['module'];
      if($module === 'field_collection') {
        $object_field = array();
        foreach ($field_values as $field_value) {
          $generator_function = 'apsjobs_utils_field_collection_generate';
          if (function_exists($generator_function)) {
            $result = apsjobs_utils_field_collection_generate($field_value, $object, $field, $instance, $bundle);
          }
        }
      }
    }
  }
}
