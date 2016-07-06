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
    'help' => t('The id of this report'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['count'] = array(
    'title' => t('Count'),
    'help' => t('The count of this location'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['incNum'] = array(
    'title' => t('Incremental Number'),
    'help' => t('The incremental number in which the span option is increased'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
    ),
  );
  $data['suma_stats']['spanOption'] = array(
    'title' => t('Span Option'),
    'help' => t('The time span from which the start date is set'),
    'field' => array(
      'handler' => 'suma_stats_handler_field',
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