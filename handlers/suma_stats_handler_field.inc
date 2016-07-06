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
}