<?php

/**
 * @file
 * Provide views handlers and plugins that allow usage of PHP.
 */

/**
 * Implements hook_views_data().
 */
function views_callback_views_data() {
  $data['views']['callback'] = array(
    'title' => t('Custom Callback'),
    'help' => t('Use custom callback.'),
    'area' => array(
      'help' => t('Use custom callback to construct the output of an area.'),
      'handler' => 'views_callback_handler_area',
    ),
    'field' => array(
      'help' => t('Use custom callback to construct the output of a field.'),
      'handler' => 'views_callback_handler_field',
    ),
    'filter' => array(
      'help' => t('Use custom callback to filter the result of the view.'),
      'handler' => 'views_callback_handler_filter',
    ),
    'sort' => array(
      'help' => t('Use custom callback to sort the result of the view.'),
      'handler' => 'views_callback_handler_sort',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function views_callback_views_plugins() {
  $plugins = array(
    'access' => array(
      'callback' => array(
        'title' => t('Custom Callback'),
        'help' => t('Use custom callback to grant access.'),
        'help topic' => '',
        'handler' => 'views_callback_plugin_access',
        'uses options' => TRUE,
      ),
    ),
    'cache' => array(
      'callback' => array(
        'title' => t('Custom Callback'),
        'help' => t('Use custom callback to determine whether a should be cached.'),
        'help topic' => '',
        'handler' => 'views_callback_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
  return $plugins;
}
