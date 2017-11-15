<?php
function apsjobs_utils_text_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array();
  $object_field['format'] = 'full_html';
  if ($instance['widget']['type'] != 'text_textfield') {
    $object_field['value'] = isset($field_value['value']) ? $field_value['value'] : $field_value;
    $object_field['format'] = isset($field_value['format']) ? $field_value['format'] : 'full_html';
    if ($instance['widget']['type'] == 'text_textarea_with_summary') {
      $object_field['summary'] = is_array($field_value) && isset($field_value['summary']) ? $field_value['summary'] : "";
    }
  }
  else {
  	$object_field['value'] = $field_value;
  }
  return $object_field;
}

function apsjobs_utils_entityreference_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array('target_id' => $field_value);
  return $object_field;
}

function apsjobs_utils_commerce_product_reference_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array('product_id' => $field_value);
  return $object_field;
}

function apsjobs_utils_field_collection_generate($field_value, $object, $field, $instance, $bundle) {
  if(!empty($field_value) && isset($field_value['field_title']) && isset($field_value['field_duration'])) {
    $node = $object;
    $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_track_listing'));
    $field_collection_item->field_title[LANGUAGE_NONE][] = array('value' => $field_value['field_title']);
    $field_collection_item->field_duration[LANGUAGE_NONE][] = array('value' => $field_value['field_duration']);
    $field_collection_item->setHostEntity('node', $node);
    $field_collection_item->save(TRUE);
    node_save($node);
  }
}

function apsjobs_utils_image_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array();
  if (is_array($field_value)) {
    foreach ($field_value as $url) {
      $rs = apsjobs_utils_save_image($url, $object, $field, $instance, $bundle);
      $object_field[] = $rs;
    }
  }
  else {
    $object_field = apsjobs_utils_save_image($field_value, $object, $field, $instance, $bundle);
  }

  return $object_field;
}

function apsjobs_utils_save_image($url, $object, $field, $instance, $bundle) {
  $info = pathinfo($url);
  $filename = isset($info['filename']) ? $info['filename'] : "";
  $extension = isset($info['extension']) ? $info['extension'] : 'png';

  $object_field = array();
  $path = apsjobs_utils_download_image($url);
  if ($path) {
    $source = new stdClass();
    $source->uri = $path;
    $source->uid = 1;
    $source->filemime = 'image/' . pathinfo($path, PATHINFO_EXTENSION);
    $destination_dir = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];
    file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);
    $counter = 0;
    $destination = $destination_dir . '/' . $filename . "." . $extension;
    $realpath = drupal_realpath($destination);
    while (file_exists($realpath)) {
      $counter ++;
      $destination = $destination_dir . '/' . $filename . ($counter ? "_" . $counter : "") . "." . $extension;
      $realpath = drupal_realpath($destination);
    }
    $source->filename = $filename . ($counter ? "_" . $counter : "") . "." . $extension;
    $file = file_move($source, $destination, FILE_CREATE_DIRECTORY);
  }
  else {
    return NULL;
  }

  //debug($file);
  $object_field['fid'] = $file->fid;
  $object_field['alt'] = '';
  $object_field['title'] = '';
  return $object_field;
}

function apsjobs_utils_download_image($input_path) {
  $info = pathinfo($input_path);
  $extension = isset($info['extension']) ? $info['extension'] : 'png';
  $tmp_file = drupal_tempnam('temporary://', 'imagefield_');
  if ($tmp_file && file_exists($input_path)) {
    $content = file_get_contents($input_path);
    $destination = $tmp_file . '.' . $extension;
    $path = file_unmanaged_move($tmp_file, $destination, FILE_CREATE_DIRECTORY);
    file_put_contents(drupal_realpath($path), $content);
    return $destination;
  }
  return NULL;
}

function apsjobs_utils_file_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array();
  if (is_array($field_value)) {
    foreach ($field_value as $url) {
      $rs = apsjobs_utils_save_file($url, $object, $field, $instance, $bundle);
      $object_field[] = $rs;
    }
  }
  else {
    $object_field = apsjobs_utils_save_file($field_value, $object, $field, $instance, $bundle);
  }
  return $object_field;
}

function apsjobs_utils_save_file($url, $object, $field, $instance, $bundle) {
  $info = pathinfo($url);
  $filename = isset($info['filename']) ? $info['filename'] : "";
  $extension = isset($info['extension']) ? $info['extension'] : 'png';

  $object_field = array();
  $path = apsjobs_utils_download_file($url);
  if ($path) {
    $source = new stdClass();
    $source->uri = $path;
    $source->uid = 1;
    $source->filemime = 'field/file/' . pathinfo($path, PATHINFO_EXTENSION);
    $destination_dir = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];
    file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);
    $counter = 0;
    $destination = $destination_dir . '/' . $filename . "." . $extension;
    $realpath = drupal_realpath($destination);
    while (file_exists($realpath)) {
      $counter ++;
      $destination = $destination_dir . '/' . $filename . ($counter ? "_" . $counter : "") . "." . $extension;
      $realpath = drupal_realpath($destination);
    }
    $source->filename = $filename . ($counter ? "_" . $counter : "") . "." . $extension;
    $file = file_move($source, $destination, FILE_CREATE_DIRECTORY);
  }
  else {
    return NULL;
  }
  $object_field['fid'] = $file->fid;
  $object_field['display'] = 1;
  $object_field['description'] = '';
  $object_field['upload_button'] = 'Upload';
  $object_field['remove_button'] = 'Remove';
  $object_field['upload'] = '';
  return $object_field;
}

function apsjobs_utils_download_file($input_path) {
  $info = pathinfo($input_path);
  $extension = isset($info['extension']) ? $info['extension'] : 'png';
  $tmp_file = drupal_tempnam('temporary://', 'imagefield_');
  if ($tmp_file && file_exists($input_path)) {
    $content = file_get_contents($input_path);
    $destination = $tmp_file . '.' . $extension;
    $path = file_unmanaged_move($tmp_file, $destination, FILE_CREATE_DIRECTORY);
    file_put_contents(drupal_realpath($path), $content);
    return $destination;
  }
  return NULL;
}

function apsjobs_utils_taxonomy_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array();
  if (!empty($field_value)) {
    $str = $field_value;
    if (!is_array($field_value)) {
      $field_value = trim($field_value);
    }
    $machine_name = $field['settings']['allowed_values'][0]['vocabulary'];
    $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
    $query = db_select('taxonomy_term_data', 't');
    $tid = $query
      ->fields('t', array('tid'))
      ->condition('t.vid', $vocabulary->vid, '=')
      ->condition('t.name', $field_value)
      ->range(0, 1)
      ->execute()
      ->fetchField();
    if ($tid === FALSE) {
      $term_info = array(
        'name' => $field_value,
        'description' => $field_value,
        'vid' => $vocabulary->vid,
      );
      $term_object = (object) $term_info;
      taxonomy_term_save($term_object);
    }
    $query = db_select('taxonomy_term_data', 't');
    $tid = $query
      ->fields('t', array('tid'))
      ->condition('t.vid', $vocabulary->vid, '=')
      ->condition('t.name', $field_value)
      ->range(0, 1)
      ->execute()
      ->fetchField();
    $object_field['tid'] = (int) $tid;
  }
  return $object_field;
}

function apsjobs_utils_boolean_generate($field_value, $object, $field, $instance, $bundle) {
  return array('value' => $field_value);
}

function apsjobs_utils_number_generate($field_value, $object, $field, $instance, $bundle) {
  $value = is_numeric($field_value) ? $field_value : 0;
  return array('value' => $value);
}

function apsjobs_utils_date_generate($field_value, $object, $field, $instance, $bundle) {
  if ($field_value == '')
    return array();
  $field_value = str_replace('/', '-', $field_value);
  $time = strtotime($field_value);
  $str = date('Y-m-d H:i:s', $time);
  return array('value' => $str);
}

function apsjobs_utils_link_generate($field_value, $object, $field, $instance, $bundle){
  $object_field = array();
  $object_field['url'] = isset($field_value['url']) ? $field_value['url'] : "";
  $object_field['title'] = isset($field_value['title']) ? $field_value['title'] : "";
  $object_field['attributes'] = isset($field_value['attributes']) ? $field_value['attributes'] : "";
  return $object_field;
}

function apsjobs_utils_commerce_price_generate($field_value, $object, $field, $instance, $bundle){
  $object_field = array();
  if (empty($field_value))
    return $object_field;
  $object_field['amount'] = $field_value;
  $object_field['currency_code'] = 'EUR';
  return $object_field;
}

function apsjobs_utils_youtube_generate($field_value, $object, $field, $instance, $bundle){
  $object_field = array();
  $object_field['input'] = $field_value;
  $data = explode('=', $field_value);
  $object_field['video_id'] = $data[count($data)-1];
  return $object_field;
}

function apsjobs_utils_addressfield_generate($field_value, $object, $field, $instance, $bundle) {
  if(is_array($field_value)) {
    $object_field = array(
      'country' => $field_value['country'],
      'locality' => $field_value['locality'],
      'thoroughfare' => $field_value['thoroughfare'],
      'postal_code' => $field_value['postal_code']
    );
  }
  else {
    $states = array('ACT', 'NSW', 'NT', 'QLD', 'SA', 'TAS', 'VIC', 'WA');
    $object_field = array(
      'country' => 'AU',
    );
    $t1 = $field_value;
    $parts = explode(" ", $t1);
    $object_field['postal_code'] = trim($parts[count($parts) - 1]);

    $object_field['administrative_area'] = trim($parts[count($parts) - 2]);

    unset($parts[count($parts) - 1]);
    unset($parts[count($parts) - 1]);

    $t3 = implode(" ", $parts);
    $parts = explode(",", $t3);
    if(isset($parts[1])) {
      $object_field['locality'] = trim($parts[1]);
      $object_field['thoroughfare'] = trim($parts[0]);
    }
    else {
      $ps = explode(" ", $t3);
      $object_field['locality'] = trim($ps[count($ps) - 1]);
      unset($ps[count($ps) - 1]);
      $t4 = implode(" ", $ps);
      $object_field['thoroughfare'] = trim($t4);
    }
  }
  return $object_field;
}

function apsjobs_utils_email_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array(
    'email' => $field_value,
  );
  return $object_field;
}

function apsjobs_utils_list_generate($field_value, $object, $field, $instance, $bundle) {
  $number_of_values = $field['cardinality'];
  $list_type = $field['type'];
  $object_field = array();
  switch ($list_type) {
    case 'list_boolean':
      $object_field = apsjobs_utils_list_boolean($field_value);
      break;
    case 'list_text':
      $object_field = apsjobs_utils_list_text($field_value, $number_of_values);
      break;
  }

  return $object_field;
}

function apsjobs_utils_list_boolean($field_value) {
  if ($field_value == 'true' || $field_value == 1) {
    return array('value' => 1);
  }
  else {
    return array('value' => 0);
  }
}

function apsjobs_utils_list_text($field_value, $number_of_values) {
  $result = array();
  if ($number_of_values == 1) {
    $result = array('value' => $field_value);
  }
  else {
    for($i = 0; $i < $number_of_values; $i++) {
      $result[$i] = array('value' => $field_value[$i]);
    }
  }

  return $result;
}