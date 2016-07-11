<?php

/**
 * @file
 *   Views query plugin for suma stats.
 */
class suma_stats_plugin_query extends views_plugin_query {

  function query($get_count = FALSE) {
  }

  /**
   * @param view $view
   * gets number of counts for given parameters from suma
   */
  function execute(&$view) {
    //@todo add better error handling here
    //setting server variable
    $mySuma = variable_get('suma_stats_server', "");
    if ($mySuma == '') {
      return;
    }
    //setting a start date if start date was deleted
    $time = date('Y-m-d H:i:s', strtotime('-1 hour'));
    $newDate = new DateTime($time, new DateTimeZone('UTC'));
    $newDate->setTimezone(new DateTimeZone('America/Chicago'));
//    $defaultDate = $newDate->format('YmdHi');

    //number of days, months, etc
    $incNum = $this->options['incnum'];
    //option for days, months, etc
    $spanOption = $this->options['spanOption'];
//    $ct = $this->options['ct'];
    $collection = $this->options['collection'];
    $format = $this->options['format'];
    $scale = $this->options['suma_date_group_format'];

    //setting the variables to lowercase if they are not numeric
    //the following two variables need to be put in place before this is public
    $entActivity = is_numeric($this->options['activity']) ? $this->options['activity'] : strtolower($this->options['activity']);
    $entActivityGroup = is_numeric($this->options['activityGroup']) ? $this->options['activityGroup'] : strtolower($this->options['activityGroup']);
    $startDate = $this->getStartTime($incNum, $spanOption);

    //passing the parameters to the api..this may need revised for more options or empty parameters
    $suma_url = variable_get('suma_stats_server', "") . '/query/counts?id=' . $collection . '&format=%27' . $format . '%27&sdate=' . $startDate;
    $response = drupal_http_request($suma_url);
    $sumadata = json_decode($response->data);
    $preArrayLocation = $this->options['location'];
    $arrayedLocation = explode(',', $preArrayLocation);
    $entLocation = $this->setArray($arrayedLocation, $sumadata);
    $suma_ag_data = array();

    /**
     * looping through the returned data to set the rows of the view
     */
    foreach ($sumadata as $location) {
      if (isset($location->id)) {
        /**
         * Turn counts in to rows
         */
        foreach ($location->counts as $entry) {
          $temp_time = $this->trunc_date($scale, $entry->time);
          $coord = $location->title . "|" . $location->id . "|" . $temp_time;

          if (isset($suma_ag_data[$coord])) {
            $suma_ag_data[$coord]++;
          }
          else {
            $suma_ag_data[$coord] = 1;
          }
        }
      }
    }
    foreach ($suma_ag_data as $coords => $count) {
      $row = new stdClass();
      $temp_row = explode("|", $coords);
      array_push($temp_row, $count);
      $row->date = strtotime($temp_row[2]);
      $row->title = $temp_row[0];
      $row->id = $temp_row[1];
      $row->count = $temp_row[3];
      $view->result[] = $row;

    }
  }

  /**
   * @param $location
   * @param $loc
   * @param $act
   * @param $actG
   * @return int
   *
   * if the results match the entered parameters this will increase the count by 1
   * this does not currently allow the view to count 'all' locations
   * @todo add functionality to account for 'all' locations
   */
//  function getTally($location, $loc, $act, $actG) {
//    $tally = 0;
//    if (!isset($location->counts)) {
//      return $tally;
//    }
//    foreach ($location->counts as $cc) {
//      if (in_array($cc->location, $loc)) {
//        $tally += 1;
//      }
//    }
//    return $tally;
//  }
//
//  function getCount($location, $loc, $act, $actG) {
//    $count = array();
//    $i = 0;
//    if (!isset($location->counts)) {
//      return NULL;
//    }
//    foreach ($location->counts as $cc) {
//      $count[$i] = array();
//      if (in_array($cc->location, $loc)) {
//        $count[$i]['id'] = $cc->id;
//        $count[$i]['location'] = $cc->location;
//        $count[$i]['activities'] = $cc->activities;
//        $i++;
//      }
//    }
//    return $count;
//  }

  function trunc_date($scale, $time) {
    //evaluates $scale which is the group by function the user chose
    //and sets it to the proper string format for comparison
    switch ($scale) {
      case 'hour':
        $fd = 'Y-m-d-H';
        break;
      case 'day':
        $fd = 'Y-m-d';
        break;
      case 'month':
        $fd = 'Y-m';
        break;
      case 'year':
        $fd = 'Y';
        break;
      default:
        $fd = 'Y-m';
    }

    $date = new DateTime($time);
    $temp_date = $date->format($fd);
    return $temp_date;
  }

  /**
   * @param int $inc
   * @param int $span
   * @return string
   *
   * set the default date to the proper format
   */
  function getStartTime($inc = 0, $span = 0) {
    //setting a start date if start date was deleted
    $time = date('Y-m-d H:i:s', strtotime('-' . $inc . ' ' . $span . ' ')); dpm($time);
    $newDate = new DateTime($time, new DateTimeZone('UTC'));
    $newDate->setTimezone(new DateTimeZone('America/Chicago'));
    $defaultDate = $newDate->format('YmdHi');
    return $defaultDate;
  }

  /**
   * @param $location
   * @return array
   *
   * takes array of string and numeric locations and compares
   * that to the title and ids of the available locations and
   * then converts the strings(titles) to the numeric value(id)
   */
  function setArray($tempLoc, $location) {
    foreach ($tempLoc as $key => $value) {
      $tempLoc[$key] = is_numeric($value) ? $value : strtolower($value);
      if (!is_numeric($tempLoc[$key])) {
        $tempLoc[$key] = strtolower($value);
        foreach ($location->initiative->dictionary->locations as $locs) {
          $tempLoc[$key] = ($tempLoc[$key] == strtolower($locs->title)) ? $locs->id : $tempLoc[$key];
        }
      }
    }
    return $tempLoc;
  }

  /**
   * @return array
   *
   * option defaults for query settings
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['collection'] = array(
      'default' => '1',
    );
    $options['format'] = array(
      'default' => 'cal',
    );
    $options['location'] = array(
      'default' => '1',
    );
    $options['activity'] = array(
      'default' => '',
    );
    $options['activityGroup'] = array(
      'default' => '',
    );
//    $options['ct'] = array(
//      'default' => 'tally',
//    );
    $options['suma_date_group_format'] = array(
      'default' => 'day',
    );
    $options['spanOption'] = array(
      'default' => 'years',
    );
    $options['incnum'] = array(
      'default' => '1',
    );

    return $options;
  }

  /**
   * @param $form
   * @param $form_state
   *
   * Creating the fields for the query settings view
   */
  function options_form(&$form, &$form_state) {
    $form = parent::options_form($form, $form_state);

    $form['collection'] = array(
      '#type' => 'textfield',
      '#title' => t('ID of Collection'),
      '#description' => t('The ID of the collection that should return stats'),
      '#default_value' => $this->options['collection'],
      '#maxlength' => 10,
      '#size' => 10,
    );
    $form['format'] = array(
      '#type' => 'textfield',
      '#title' => t('Format'),
      '#description' => t('Choose a combination of C A L '),
      '#default_value' => $this->options['format'],
      '#maxlength' => 10,
      '#size' => 10,
    );
    $form['location'] = array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#description' => t('Enter a comma (,) separated list of locations. Do not put a space after the commas'),
      '#default_value' => $this->options['location'],
    );
    $form['activity'] = array(
      '#type' => 'textfield',
      '#title' => t('Activity'),
      '#description' => t('Type the name or ID of the activity'),
      '#default_value' => $this->options['activity'],
    );
    $form['activityGroup'] = array(
      '#type' => 'textfield',
      '#title' => t('Activity Group'),
      '#description' => t('Type the name or ID of the activity group'),
      '#default_value' => $this->options['activityGroup'],
    );
//    $form['ct'] = array(
//      '#type' => 'radios',
//      '#title' => t('Count or Tally'),
//      '#description' => t('Select Count for all counts and select Tally for a total'),
//      '#default_value' => $this->options['ct'],
//      '#options' => array(
//        'count' => t('Count'),
//        'tally' => t('Tally'),
//      ),
//    );
    $form['suma_date_group_format'] = array(
      '#title' => t('Date Group By Options'),
      '#type' => 'select',
      '#description' => 'Enter format in which you would like the counts to be grouped',
      '#options' => array(
        'day' => 'Day',
        'month' => 'Month',
        'year' => 'Year',
        'hour' => 'Hour',
      ),
      '#default_value' => (isset($this->options['suma_date_group_format'])) ? $this->options['suma_date_group_format'] : 'day',
    );
    $form['incnum'] = array(
      '#type' => 'textfield',
      '#title' => t('Incremental Number'),
      '#description' => t('Enter the incremental number to coincide with the span field below'),
      '#default_value' => $this->options['incnum'],
      '#maxlength' => 10,
      '#size' => 10,
    );
    $form['spanOption'] = array(
      '#type' => 'select',
      '#title' => t('Span of time'),
      '#description' => t('Select the option to go with the number entered in the number field above.'),
      '#options' => array(
        'hours' => t('hours ago'),
        'days' => t('days ago'),
        'months' => t('months ago'),
        'years' => t('years ago'),
      ),
      '#default_value' => (isset($this->options['spanOption'])) ? $this->options['spanOption'] : 'days',
    );
  }
}