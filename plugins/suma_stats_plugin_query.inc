<?php

/**
 * @file
 *   Views query plugin for suma stats.
 */
class suma_stats_plugin_query extends views_plugin_query
{

  function query($get_count = FALSE) { }

  /**
   * @param view $view
   * place holder function for suma api
   * when finished setting up api this needs to return a row-field data set
   */
  function execute(&$view) {
    $suma_url='https://stats.lib.ou.edu/sumaserver/query/counts?format=%27ac%27';
    $response = drupal_http_request($suma_url);
    $sumadata = json_decode($response->data);
    foreach ($sumadata as $location) {
      $row = new stdClass();
      $row->title = $location->title;
      $row->id = $location->id;
      $view->result[] = $row;
    }
    dpm($sumadata);
//    foreach ($photos['photos']['photo'] as $photo) {
//      $row = new stdClass;
//      $photo_id = $photo['id'];
//      $info = $flickr->photos_getInfo($photo_id);
//      $row->title = $info['photo']['title'];
//      $view->result[] = $row;
//    }
    error_log('later');
  }

  function option_definition()
  {
    $options = parent::option_definition();

    $options['collection'] = array(
      'default' => '20',
    );
    $options['format'] = array(
      'default' => '',
    );

    return $options;
  }

  functio


}