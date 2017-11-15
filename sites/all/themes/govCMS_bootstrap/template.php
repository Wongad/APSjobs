<?php

function govCMS_bootstrap_preprocess_html(&$vars) {
}


function govCMS_bootstrap_css_alter(&$css) {

  // Sort CSS items, so that they appear in the correct order.
  // This is taken from drupal_get_css().
  uasort($css, 'drupal_sort_css_js');

  // The Print style sheets
  // I will populate this array with the print css (usually I have only one but just in case…)
  $print = array();

  // I will add some weight to the new $css array so every element keeps its position
  $weight = 0;

  foreach ($css as $name => $style) {

    // I leave untouched the conditional stylesheets
    // and put all the rest inside a 0 group
    if ($css[$name]['browsers']['!IE']) {
      $css[$name]['group'] = 0;
      $css[$name]['weight'] = ++$weight;
      $css[$name]['every_page'] = TRUE;
    }

    if($css[$name]['media'] == 'screen') {
      $css[$name]['media'] = 'all';
    }

    // I move all the print style sheets to a new array
    if ($css[$name]['media'] == 'print') {
      // remove and add to a new array
      $print[$name] = $css[$name];
      unset($css[$name]);
    }

  }

  // I merge the regular array and the print array
  $css = array_merge($css, $print);
}

function govCMS_bootstrap_preprocess_node(&$vars) {
  $node = $vars['node'];
  if($vars['elements']['#view_mode'] == 'full'){
    if ($node->type == 'aps_vacancies') {
      $vars['theme_hook_suggestions'][]= 'node__full';
      $vars['theme_hook_suggestions'][]= 'node__full__' . $node->type;
    }
  }
  // if($vars['elements']['#view_mode'] == 'preview'){
  //   if ($node->type == 'aps_vacancies') {
  //     $vars['theme_hook_suggestions'][]= 'node__preview';
  //     $vars['theme_hook_suggestions'][]= 'node__preview__' . $node->type;
  //   }
  // }
}

function govCMS_bootstrap_preprocess_block(&$vars) {
  $block = $vars['block'];
  if ($block->bid == 951) {
    $vars['theme_hook_suggestions'][] = 'block__custom';
  }
}
