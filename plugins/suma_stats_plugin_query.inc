<?php
/**
 * @file
 *   Views query plugin for suma stats.
 */
class suma_stats_plugin_query extends views_plugin_query
{

  function query($get_count = FALSE)
  {
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
    $defaultDate = $newDate->format('YmdHi');

    //number of days, months, etc
    $incNum = $this->options['incNum'];
    //option for days, months, etc
    $spanOption = $this->options['spanOptions'];
    $ct = $this->options['ct'];
    $collection = $this->options['collection'];
    $format = $this->options['format'];

    //setting the variables to lowercase if they are not numeric
    $entActivity = is_numeric($this->options['activity']) ? $this->options['activity'] : strtolower($this->options['activity']);
    $entActivityGroup = is_numeric($this->options['activityGroup']) ? $this->options['activityGroup'] : strtolower($this->options['activityGroup']);
    $startDate = $this->getStartTime($incNum, $spanOption);

    //passing the parameters to the api..this may need revised for more options or empty parameters
    $suma_url = variable_get('suma_stats_server', "") .  '/query/counts?id=' . $collection . '&format=%27' . $format . '%27&sdate=' . $startDate;
    $response = drupal_http_request($suma_url);
    $sumadata = json_decode($response->data);
    $preArrayLocation = $this->options['location'];
    $arrayedLocation = explode(',',$preArrayLocation);
    $entLocation = $this->setArray($arrayedLocation, $sumadata);


    /**
     * looping through the returned data to set the rows of the view
     */
    foreach ($sumadata as $location) {

      if (isset($location->id)) {

        /**
         * Turn counts in to rows
         */
        foreach ($location->counts as $entry) {
          $row = new stdClass();
          $row->title = $location->title;
          $row->id = $location->id;
          $row->incNum = $incNum;
          $row->spanOptions = $spanOptions;
          /* Views needs a unix timestamp */
          $row->date = strtotime($entry->time);
          $row->count = 1;

          $view->result[] = $row;
        }
      }
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
  function getTally($location, $loc, $act, $actG) {
    $tally = 0;
    if (!isset($location->counts)) {
      return $tally;
    }
    foreach ($location->counts as $cc) {
      if (in_array($cc->location, $loc)) {
        $tally += 1;
      }
    }
    return $tally;
  }

  function getCount($location, $loc, $act, $actG) {
    $count = array();
    $i = 0;
    if (!isset($location->counts)) {
      return null;
    }
    foreach ($location->counts as $cc) {
      $count[$i] = array();
      if (in_array($cc->location, $loc)) {
        $count[$i]['id'] = $cc->id;
        $date = new DateTime($cc->time);
        $count[$i]['date'] = $date->getTimestamp();
        $count[$i]['location'] = $cc->location;
        $count[$i]['activities'] = $cc->activities;
        $i++;
      }
    }
    return $count;
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
    $time = date('Y-m-d H:i:s', strtotime('-'.$inc. ' ' .$span .' '));
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

  function option_definition()
  {
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
    $options['ct'] = array(
      'default' => 'tally',
    );
    $options['incNum'] = array(
      'default' =>'1',
    );
    $options['spanOptions'] = array(
      'default' => 'hours',
    );
    //may use this option in the future
//    $options['datetime'] = array(
//      'default' => $defaultDate,
//    );

    return $options;
  }

  /**
   * @param $form
   * @param $form_state
   *
   * Creating the fields for the view
   */
  function options_form(&$form, &$form_state)
  {
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
    $form['ct'] = array(
      '#type' => 'radios',
      '#title' => t('Count or Tally'),
      '#description' => t('Select Count for all counts and select Tally for a total'),
      '#default_value' => $this->options['ct'],
      '#options' => array(
        'count' => t('Count'),
        'tally' => t('Tally'),
      ),
    );
    $form['incNum'] = array(
      '#type' => 'textfield',
      '#title' => t('Incremental Number'),
      '#description' => t('Enter the incremental number to coincide with the span field below'),
      '#default_value' => $this->options['incNum'],
      '#maxlength' => 10,
      '#size' => 10,
    );
    $form['spanOptions'] = array(
      '#type' => 'radios',
      '#title' => t('Span of time'),
      '#description' => t('Check the radio to go with the number entered in the number field above.'),
      '#default_value' => $this->options['spanOptions'],
      '#options' => array(
        'hours' => t('hours ago'),
        'days' => t('days ago'),
        'months' => t('months ago'),
        'years' => t('years ago'),
      ),
    );
    //This has been commented out because if we use a date picker it will be static
    //in the future we may revisit this option
//    $form['datetime'] = array(
//      '#type' => 'date_select',
//      '#title' => 'Start Date',
//      '#date_format' => 'YmdHi',
//      '#date_timezone' => 'America/Chicago',
//      '#default_value' => $this->options['datetime'],
//      '#date_year_range' => '-100:+0',
//    );
  }

}