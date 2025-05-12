<?php

/**
 * A handler to filter a view using PHP defined by the administrator.
 *
 * @ingroup views_filter_handlers
 */
class views_callback_handler_filter extends views_handler_filter {

  protected $callback_static_variable = NULL;

  /**
   * Implements views_handler#can_expose().
   */
  public function can_expose() {
    return FALSE;
  }

  /**
   * Implements views_object#admin_summary().
   */
  public function admin_summary() {
    return t('Custom callback');
  }

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['use_callback_setup'] = array('default' => FALSE);
    $options['callback_setup'] = array('default' => '');
    $options['callback_filter'] = array('default' => '');
    return $options;
  }

  /**
   * Implements views_handler#option_definition().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__handler_filter_' . $form_state['id'] . '__step_';

    $form += views_callback_form_element($this,
      array('use_callback_setup', t('Use setup callback'), t('If checked, you can provide a custom callback to be run once right before execution of the view. This may be useful to define a function to be re-used in the value and/or output callback.')),
      array('callback_setup', t('Setup callback'), t('Code to run right before execution of the view. Store output to the static variable.'), FALSE),
      array('$view', '$handler', '&$static'),
      $callback_name . 'setup',
    );
    $form += views_callback_form_element($this,
      FALSE,
      array('callback_filter', t('Filter callback'), t('If the callback function returns TRUE the current row is <em>filtered out</em>, that is, removed from, the results. Return FALSE to <em>keep</em> the current row.'), FALSE),
      array('$view', '$handler', '$static', '$row', '$data'),
      $callback_name . 'filter',
    );
  }

  /**
   * Implements views_handler_filter#query().
   */
  public function query() {
    // Inform views_callback_views_pre_execute() to seize control over the query.
    $this->view->views_callback = TRUE;
    $this->view->views_callback_query_all = TRUE;
  }

  /**
   *
   * @see views_callback_views_pre_execute()
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
        watchdog('views_callback', 'View: %view is calling undefined Filter Setup function %callback.', array(
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
    // Evaluate the callback function.
    if (!empty($this->options['callback_filter'])) {
      $function = $this->options['callback_filter'];
      if (function_exists($function)) {
        ob_start();
        foreach ($this->view->result as $i => $row) {
          $normalized_row = new stdClass();
          foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
            $normalized_row->$field = isset($row->{$handler->field_alias}) ? $row->{$handler->field_alias} : NULL;
          }
          if ($function($this->view, $this, $this->callback_static_variable, $normalized_row, $row)) {
            unset($this->view->result[$i]);
          }
        }
        ob_end_clean();
      }
      else {
        watchdog('views_callback', 'View: %view is calling undefined Filter function %callback.', array(
          '%callback' => $function,
          '%view' => $this->view->name,
        ), WATCHDOG_CRITICAL);
      }
    }
  }

}
