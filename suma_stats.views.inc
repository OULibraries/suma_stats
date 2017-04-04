<?php

/**
 * Implements hook_views_plugins().
 */
function suma_stats_views_plugins() {
  $plugin = array();
  $plugin['query']['suma_stats_plugin_query'] = array(
    'title' => t('Suma Stats Query'),
    'help' => t('Suma Stats query object.'),
    'handler' => 'suma_stats_plugin_query',
  );
  return $plugin;
}

/**
 * Implements hook_views_data().
 * 
 * the actual fields from which the user can choose in the view
 */
function suma_stats_views_data() {
  $data = array();

  //Base data
  $data['suma_stats']['table']['group'] = t('Suma Stats');
  $data['suma_stats']['table']['base'] = array(
    'title' => t('Suma Stats'),
    'help' => t('Query Suma Stats'),
    'query class' => 'suma_stats_plugin_query'
  );

  //Fields
  $data['suma_stats']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this report'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['id'] = array(
    'title' => t('ID'),
    'help' => t('The id of this count'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['activity'] = array(
    'title' => t('Activity'),
    'help' => t('The activity of this count'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['count'] = array(
    'title' => t('Count'),
    'help' => t('The count of this location'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['suma_stats']['date'] = array(
    'title' => t('Date'),
    'help' => t('The date of the count'),
    'field' => array(
      'handler' => 'suma_stats_handler_field_date',
    ),
  );
  $data['suma_stats']['date_format'] = array(
    'title' => t('Date Format'),
    'help' => t('The date format of the count'),
    'field' => array(
      'handler' => 'suma_stats_field_option',
    ),
  );

  return $data;
}