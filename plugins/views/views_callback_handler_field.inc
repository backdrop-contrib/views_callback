<?php

/**
 * A handler to provide a field that is constructed by the administrator using PHP.
 *
 * @ingroup views_field_handlers
 */
class views_callback_handler_field extends views_handler_field {
  const CLICK_SORT_DISABLED = 0;
  const CLICK_SORT_NUMERIC = 1;
  const CLICK_SORT_ALPHA = 2;
  const CLICK_SORT_ALPHA_CASE = 3;
  const CLICK_SORT_NAT = 4;
  const CLICK_SORT_NAT_CASE = 5;
  const CLICK_SORT_CALLBACK = 6;

  protected $callback_static_variable = NULL;

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['use_callback_setup'] = array('default' => FALSE);
    $options['callback_setup'] = array('default' => '');
    $options['use_callback_value'] = array('default' => FALSE);
    $options['callback_value'] = array('default' => '');
    $options['callback_output'] = array('default' => '');
    $options['use_callback_click_sortable'] = array('default' => self::CLICK_SORT_DISABLED);
    $options['callback_click_sortable'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Implements views_handler#options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__handler_field_' . $form_state['id'] . '__step_';

    $form += views_callback_form_element($this,
      array('use_callback_setup', t('Use setup callback'), t('If checked, you can provide a callback function to be run once right before execution of the view. This may be useful to define callback functions to be re-used in the value and/or output callback.')),
      array('callback_setup', t('Setup callback'), t('Callback function to run right before execution of the view. Store data in static variable to carry data from one row to another. The query has been fully built, but has not been executed.'), FALSE),
      array('$view', '$handler', '&$static'),
      $callback_name . 'setup',
    );
    $form += views_callback_form_element($this,
      array('use_callback_value', t('Use value callback'), t('If checked, you can provide a callback function to construct the value of this field.')),
      array('callback_value', t('Value callback'), t('Callback function to construct the value of this field. The return from this callback will be passed to <code>$value</code> in the output function. Data can also be passed in to the <code>$static</code> variable to carry data from one row to another. This callback is called right after the query has been executed, but no rendering has happened yet.'), FALSE),
      array('$view', '$handler', '&$static', '$row', '$data'),
      $callback_name . 'value',
    );
    $form += views_callback_form_element($this,
      array('use_callback_click_sortable', t('Enable click sort'), t('If checked, you can use a callback function to enable click sort on the field. This may have a large performance impact as all results will be loaded from the database, and pager limits will only be applied in PHP.')),
      array('callback_click_sortable', t('Click sort callback'), t('The comparison callback function must return an integer less than, equal to, or greater than zero if the first row should respectively appear before, stay where it was compared to, or appear after the second row.'), FALSE),
      array(
        '$view', '$handler', '$static',
        '$row1' => t('Data of row.'),
        '$row2' => t('Data of row to compare against.'),
      ),
      $callback_name . 'click_sortable',
    );
    $form['use_callback_click_sortable']['#type'] = 'select';
    $form['use_callback_click_sortable']['#options'] = array(
      self::CLICK_SORT_DISABLED => t('No'),
      self::CLICK_SORT_NUMERIC => t('Sort numerically'),
      self::CLICK_SORT_ALPHA => t('Sort alphabetically'),
      self::CLICK_SORT_ALPHA_CASE => t('Sort alphabetically (case insensitive)'),
      self::CLICK_SORT_NAT => t('Sort using a "natural order" algorithm'),
      self::CLICK_SORT_NAT_CASE => t('Sort using a "natural order" algorithm (case insensitive)'),
      self::CLICK_SORT_CALLBACK => t('Sort using custom a custom callback'),
    );
    $form['use_callback_click_sortable']['#default_value'] = $this->options['use_callback_click_sortable'];
    $form['callback_click_sortable']['#states'] = array(
      'visible' => array(
        ':input[name="options[use_callback_click_sortable]"]' => array('value' => (string) self::CLICK_SORT_CALLBACK),
      ),
    );
    $form += views_callback_form_element($this,
      FALSE,
      array('callback_output', t('Output callback'), t('Callback function to render the output of this field. Return a string with optional HTML to be displayed.'), TRUE),
      array('$view', '$handler', '$static', '$row', '$data', '$value' => t('Value of this field.')),
      $callback_name . 'output',
    );
  }

  /**
   * Implements views_handler_field#query().
   *
   * @see views_callback_views_pre_execute()
   */
  public function query() {
    // Provide an field alias but don't actually alter the query.
    $this->field_alias = 'views_callback_' . $this->position;
    // Inform views_callback_views_pre_execute() to seize control over the query.
    $this->view->views_callback = TRUE;
    if ($this->click_sortable()) {
      $this->view->views_callback_query_all = TRUE;
    }
  }

  /**
   * Implements views_handler_field#click_sortable().
   */
  public function click_sortable() {
    return $this->options['use_callback_click_sortable'] != self::CLICK_SORT_DISABLED;
  }

  /**
   * Implements views_handler_field#click_sort().
   *
   * @see self::callback_post_execute()
   */
  public function click_sort($order) {
    $this->callback_click_sort_order = $order;
  }

  /**
   *
   * @see views_callback_views_pre_execute()
   * @see self::callback_post_execute()
   */
  public function callback_pre_execute() {
    // Execute static PHP code.
    if (!empty($this->options['use_callback_setup']) && !empty($this->options['callback_setup'])) {
      $function = $this->options['callback_setup'];
      if (function_exists($function)) {
        ob_start();
        $function($this->view, $this, $this->callback_static_variable);
        ob_end_clean();
      }
      else {
        watchdog('views_callback', 'View: %view is calling undefined Field Setup function %callback.', array(
          '%callback' => $function,
          '%view' => $this->view->name,
        ), WATCHDOG_CRITICAL);
      }
    }
  }

  /**
   *
   * @see views_callback_views_post_execute()
   */
  public function callback_post_execute() {
    // Execute value custom callback.
    if (!empty($this->options['use_callback_value']) && !empty($this->options['callback_value'])) {
      $function = $this->options['callback_value'];
      if (function_exists($function)) {
        ob_start();
        foreach ($this->view->result as $i => &$row) {
          $normalized_row = new stdClass();
          foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
            $normalized_row->$field = isset($row->{$handler->field_alias}) ? $row->{$handler->field_alias} : NULL;
          }
          // Store the result, passed by reference to $this->view->result.
          $row->{$this->field_alias} = $function($this->view, $this, $this->callback_static_variable, $normalized_row, $row);
        }
        ob_end_clean();
      }
      else {
        watchdog('views_callback', 'View: %view is calling undefined Field Value function %callback.', array(
          '%callback' => $function,
          '%view' => $this->view->name,
        ), WATCHDOG_CRITICAL);
      }
    }

    // If we're sorting, do the actual sorting then fix the results as per the pager info.
    if (!empty($this->options['use_callback_click_sortable']) && !empty($this->callback_click_sort_order)) {
      if ($this->options['use_callback_click_sortable'] == self::CLICK_SORT_CALLBACK) {
        if (!empty($this->options['callback_click_sortable'])) {
          $this->callback_click_sort_function = TRUE;
        }
      }
      else {
        $predefined = array(
          self::CLICK_SORT_NUMERIC => array($this, 'callback_click_sort_numeric'),
          self::CLICK_SORT_ALPHA => 'strcasecmp',
          self::CLICK_SORT_ALPHA_CASE => 'strcmp',
          self::CLICK_SORT_NAT => 'strnatcasecmp',
          self::CLICK_SORT_NAT_CASE => 'strnatcmp',
        );
        $this->callback_click_sort_function = $predefined[$this->options['use_callback_click_sortable']];
      }

      if (isset($this->callback_click_sort_function)) {
        usort($this->view->result, array($this, 'callback_click_sort'));
      }
    }
  }

  /**
   * Helper function; usort() callback for click sort support.
   */
  public function callback_click_sort($row1, $row2) {
    $factor = strtoupper($this->callback_click_sort_order) == 'ASC' ? 1 : -1;
    $result = 0;
    $function = $this->callback_click_sort_function;
    if (is_bool($this->callback_click_sort_function)) {
      $function = $this->options['callback_click_sortable'];
    }
    if (!function_exists($function)) {
      watchdog('views_callback', 'View: %view is calling undefined Field Sort function %callback.', array(
        '%callback' => $function,
        '%view' => $this->view->name,
      ), WATCHDOG_CRITICAL);
      return $factor * $result;
    }

    if ($this->options['use_callback_click_sortable'] == self::CLICK_SORT_CALLBACK) {
      foreach (array('row1' => 'normalized_row1', 'row2' => 'normalized_row2') as $name => $normalized_name) {
        $$normalized_name = new stdClass();
        foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
          $$normalized_name->$field = isset($$name->{$handler->field_alias}) ? $$name->{$handler->field_alias} : NULL;
        }
      }
      ob_start();
      $result = (int) $function($this->view, $this, $this->callback_static_variable, $normalized_row1, $normalized_row2);
      ob_end_clean();
    }
    else {
      $result = $function($row1->{$this->field_alias}, $row2->{$this->field_alias});
    }
    return $factor * $result;
  }

  /**
   * Helper function; usort callback for numeric sort.
   */
  public function callback_click_sort_numeric($a, $b) {
    return $a - $b;
  }

  /**
   * Implements views_handler_field#render().
   */
  public function render($values) {
    // Execute output callback function.
    if (!empty($this->options['callback_output'])) {
      $normalized_row = new stdClass();
      foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
        $normalized_row->$field = isset($values->{$handler->field_alias}) ? $values->{$handler->field_alias} : NULL;
      }

      $function = $this->options['callback_output'];
      if (function_exists($function)) {
        ob_start();
        echo $function($this->view, $this, $this->callback_static_variable, $normalized_row, $values, isset($values->{$this->field_alias}) ? $values->{$this->field_alias} : NULL);
        $value = ob_get_clean();
      }
      else {
        watchdog('views_callback', 'View: %view is calling undefined Field Output function %callback.', array(
          '%callback' => $function,
          '%view' => $this->view->name,
          ), WATCHDOG_CRITICAL);
      }
    }
    else {
      if (isset($values->{$this->field_alias})) {
        $value = check_plain($values->{$this->field_alias});
      }
    }

    if (isset($value)) {
      return $value;
    }
  }

}
