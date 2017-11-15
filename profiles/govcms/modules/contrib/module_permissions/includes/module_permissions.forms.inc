<?php

/**
 * @file
 * Form handlers for the module.
 */

/**
 * Managed modules list form.
 */
function module_permissions_form_system_modules_handler(&$form) {
  if (isset($form['modules'])) {
    // Remove module permissions from form.
    if (isset($form['modules']['Administration']['module_permissions'])) {
      unset($form['modules']['Administration']['module_permissions']);
    }
    // Move locked modules into different part of the form, this ensures
    // the values are still submitted.
    $form['locked'] = array('#tree' => TRUE, '#access' => FALSE);
    // Remove modules from form that are not in the managed list.
    $managed_modules = module_permissions_load_managed_modules();
    if (empty($managed_modules)) {
      drupal_set_message(t('Managed module list is empty. Please contact your site administrator.'), 'status');
    }
    foreach (element_children($form['modules']) as $package) {
      foreach (element_children($form['modules'][$package]) as $module) {
        if (!isset($managed_modules[$module])) {
          // Copy row into locked area.
          $row = $form['modules'][$package][$module];
          // Exclude other modules depend on module permissions.
          if (empty($row['#requires']['module_permissions'])) {
            $row['#parents'] = array('modules', $package, $module);
            $form['locked'][$package][$module] = $row;
          }
          // Remove from display.
          unset($form['modules'][$package][$module]);
        }
      }
      $modules = element_children($form['modules'][$package]);
      if (empty($modules)) {
        unset($form['modules'][$package]);
      }
    }

    // Module filter support.
    if (module_exists('module_filter')) {
      $submit_key = array_search('module_filter_system_modules_submit_recent', $form['#submit']);
      if ($submit_key) {
        $form['#submit'][$submit_key] = 'module_permissions_module_filter_system_modules_submit_recent';
      }
    }
  }
}

/**
 * Override module filters recent modules submit handler.
 */
function module_permissions_module_filter_system_modules_submit_recent($form, &$form_state) {
  $recent_modules = variable_get('module_filter_recent_modules', array());

  foreach ($form_state['values']['modules'] as $package => $modules) {
    foreach ($modules as $key => $module) {
      if (isset($form['modules'][$package][$key]['enable']['#default_value'])) {
        $default_value = $form['modules'][$package][$key]['enable']['#default_value'];
      }
      else {
        $default_value = $form['locked'][$package][$key]['enable']['#default_value'];
      }
      if ($default_value != $module['enable']) {
        $recent_modules[$key] = REQUEST_TIME;
      }
    }
  }

  variable_set('module_filter_recent_modules', $recent_modules);
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Only show managed modules permissions.
 */
function module_permissions_form_user_admin_permissions_handler(&$form) {
  // Retrieve role names for columns.
  $role_names = user_roles();
  // Fetch permissions for all roles or the one selected role.
  $role_permissions = user_role_permissions($role_names);

  // Reset form permission.
  if (isset($form['permission'])) {
    unset($form['permission']);
  }
  $form['permission'] = array();

  // Render role/permission overview.
  $options = array();
  $module_info = system_get_info('module');
  $hide_descriptions = system_admin_compact_mode();
  $managed_modules = module_permissions_load_managed_modules();
  if (empty($managed_modules)) {
    drupal_set_message(t('Managed module permission list is empty. Please contact your site administrator.'), 'status');
  }
  $modules = array();
  foreach (module_implements('permission') as $module) {
    if (isset($managed_modules[$module])) {
      $modules[$module] = $module_info[$module]['name'];
    }
  }
  // Sort the list.
  asort($modules);

  foreach ($modules as $module => $display_name) {
    if ($permissions = module_invoke($module, 'permission')) {
      $form['permission'][] = array(
        '#markup' => $module_info[$module]['name'],
        '#id' => $module,
      );
      foreach ($permissions as $perm => $perm_item) {
        // Fill in default values for the permission.
        $perm_item += array(
          'description' => '',
          'restrict access' => FALSE,
          'warning' => !empty($perm_item['restrict access']) ? t('Warning: Give to trusted roles only; this permission has security implications.') : '',
        );
        $options[$perm] = '';
        $perm_description = theme('user_permission_description',
          array(
            'permission_item' => $perm_item,
            'hide' => $hide_descriptions,
          )
        );
        $form['permission'][$perm] = array(
          '#type' => 'item',
          '#markup' => $perm_item['title'],
          '#description' => $perm_description,
        );
        // Build arrays for roles.
        foreach ($role_names as $rid => $name) {
          // Builds arrays for checked boxes for each role.
          if (isset($role_permissions[$rid][$perm])) {
            $status[$rid][] = $perm;
          }
        }
      }
    }
  }

  // Have to build checkboxes here after checkbox arrays are built.
  foreach ($role_names as $rid => $name) {
    $form['checkboxes'][$rid] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => isset($status[$rid]) ? $status[$rid] : array(),
      '#attributes' => array('class' => array('rid-' . $rid)),
    );
    $form['role_names'][$rid] = array(
      '#markup' => check_plain($name),
      '#tree' => TRUE,
    );
  }
}
