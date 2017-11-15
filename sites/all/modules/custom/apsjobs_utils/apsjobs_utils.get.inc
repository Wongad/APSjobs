<?php

function apsjobs_utils_get_field_from_entity($entity, $field, $key = "value", $multiple = FALSE, $lang = LANGUAGE_NONE) {
  $results = array();
  if($entity) {
    if (!isset($entity->{$field}[$lang])) {
      if ($lang == LANGUAGE_NONE) {
        global $language;
        $lang = $language->language;
      } else {
        $lang = LANGUAGE_NONE;
      }
      if (!isset($entity->{$field}[$lang])) {
        return NULL;
      } 
    }
    foreach($entity->{$field}[$lang] as $i => $value) {
      $results[] = $value[$key];
    }
  }

  if($multiple) {
    return $results;
  }
  else {
    return count($results) ? $results[0] : NULL;
  }

}

function apsjobs_utils_get_field_title_from_entity($entity, $field, $key = "value", $multiple = FALSE, $lang = LANGUAGE_NONE) {
  $results = array();

  if($entity) {
    if (!isset($entity->{$field}[$lang])) {
      if ($lang == LANGUAGE_NONE) {
        global $language;
        $lang = $language->language;
      } else {
        $lang = LANGUAGE_NONE;
      }
      if (!isset($entity->{$field}[$lang])) {
        return NULL;
      } 
    }
    foreach($entity->{$field}[$lang] as $i => $value) {
      $results[] = $value[$key];
    }
  }

  $rs = array();
  if ($key == 'target_id') {
    if (sizeof($results)) {
      foreach ($results as $k => $v) {
        $e = node_load($v);
        $rs[] = $e->title;
      }
    }
  }

  if($multiple) {
    return $rs;
  }
  else {
    return count($rs) ? $rs[0] : NULL;
  }
}

function apsjobs_utils_get_taxonomy_term_with_parent_name($term_name, $vocabulary = NULL, $parent_name = NULL) {
  if (!$term_name) {
    return NULL;
  }
  if (!$parent_name) {
    $term = taxonomy_get_term_by_name($term_name, $vocabulary);
    if(empty($term)) {
      return NULL;
    }
    $term = reset($term);
    return $term;
  }

  $parent_term = apsjobs_utils_get_taxonomy_term_with_parent_name($parent_name, $vocabulary);
  if ($parent_term) {
    $childs = taxonomy_get_children($parent_term->tid);
    foreach ($childs as $child) {
      if($child->name == $term_name) {
        return $child;
      }
    }
  }
  return NULL;
}

function apsjobs_utils_get_taxonomy_name_with_tid($tid = null) {
  if ($tid) {
    $term = taxonomy_term_load($tid);
    return $term->name;
  }
  return null;
}
