<?php

/**
 * @file
 *   Views field handler for basic Suma group fields.
 */

/**
 * Class suma_stats_handler_field
 *
 * Making sure the field_alias gets set properly, and that
 * none of the sql-specific query functionality gets called.
 */
class suma_stats_handler_field extends views_handler_field {
  function query() {
    $this->field_alias = $this->real_field;
  }
}

class suma_stats_handler_field_date extends views_handler_field_date {
  function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * @return array
   * setting the default option and definition of the field options
   * default is set to months
   */
  function option_definition() {
    $options = parent::option_definition();
//    $options['suma_date_group_format'] = array(
//      'default' => 'day',
//    );
//    $options['suma_date_range_span1'] = array(
//      'default' => 'years',
//    );
//    $options['suma_date_range_increment'] = array(
//      'default' => '2',
//    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    //offer a list of options of date formats for the user
    //to choose from
    parent::options_form($form, $form_state);

//    $form['suma_date_group_format'] = array(
//      '#title' => t('Date Format Options'),
//      '#type' => 'select',
//      '#description' => 'Enter format in which you would like the counts to be grouped',
//      '#options' => array(
////        'hour' => 'Hour',
//        'day' => 'Day',
//        'month' => 'Month',
//        'year' => 'Year',
//        'hour' =>'Hour',
//      ),
//      '#default_value' => (isset($this->options['suma_date_group_format'])) ? $this->options['suma_date_group_format'] : 'day',
//    );
//    $form['suma_date_range_increment'] = array(
//      '#type' => 'textfield',
//      '#title' => t('Incremental Number'),
//      '#description' => t('Enter the incremental number to coincide with the span field below'),
//      '#default_value' => $this->options['suma_date_range_increment'],
//      '#maxlength' => 10,
//      '#size' => 10,
//    );
//    $form['suma_date_range_span1'] = array(
//      '#type' => 'select',
//      '#title' => t('Span of time'),
//      '#description' => t('Select the option to go with the number entered in the number field above.'),
//      '#options' => array(
//        'hours' => t('hours ago'),
//        'days' => t('days ago'),
//        'months' => t('months ago'),
//        'years' => t('years ago'),
//      ),
//      '#default_value' => (isset($this->options['suma_date_range_span1'])) ? $this->options['suma_date_range_span1'] : 'days',
//    );
    return $form;
  }
}