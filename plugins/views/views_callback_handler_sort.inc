<?php

/**
 * A handler to sort a view using PHP defined by the administrator.
 *
 * @ingroup views_sort_handlers
 */
class views_callback_handler_sort extends views_handler_sort {

  protected $callback_static_variable = NULL;

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['use_callback_setup'] = array('default' => FALSE);
    $options['callback_setup'] = array('default' => '');
    $options['callback_sort'] = array('default' => '');
    return $options;
  }

  /**
   * Implements views_handler#option_definition().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__handler_sort_' . $form_state['id'] . '__step_';

    $form += views_callback_form_element($this,
      array('use_callback_setup', t('Use setup callback'), t('If checked, you can provide a custom callback to be run once right before execution of the view. This may be useful to define functions to be re-used in the value and/or output callback.')),
      array('callback_setup', t('Setup callback'), t('Callback function to run right before execution of the view. Store output to the static variable.'), FALSE),
      array('$view', '$handler', '&$static'),
      $callback_name . 'setup',
    );
    $form += views_callback_form_element($this,
      FALSE,
      array('callback_sort', t('Sort callback'), t('The comparison callback must return an integer less than, equal to, or greater than zero if the first row should respectively appear before, stay where it was compared to, or appear after the second row.'), FALSE),
      array(
        '$view', '$handler', '$static',
        '$row1' => t('Data of row.'),
        '$row2' => t('Data of row to compare against.'),
      ),
      $callback_name . 'sort',
    );
  }

  /**
   * Implements views_handler_sort#query().
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
        watchdog('views_callback', 'View: %view is calling undefined Sort Setup function %callback.', array(
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
    if (!empty($this->options['callback_sort']) && $this->view->style_plugin->build_sort()) {
      ob_start();
      usort($this->view->result, array($this, 'callback_sort'));
      ob_end_clean();
    }
  }

  /**
   * Helper function; usort() callback for sort support.
   */
  public function callback_sort($row1, $row2) {
    $factor = strtoupper($this->options['order']) == 'ASC' ? 1 : -1;
    $result = 0;

    $function = $this->options['callback_sort'];
    if (function_exists($this->options['callback_sort'])) {
      foreach (array('row1' => 'normalized_row1', 'row2' => 'normalized_row2') as $name => $normalized_name) {
        $$normalized_name = new stdClass();
        foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
          $$normalized_name->$field = isset($$name->{$handler->field_alias}) ? $$name->{$handler->field_alias} : NULL;
        }
      }
      $result = (int) $function($this->view, $this, $this->callback_static_variable, $normalized_row1, $normalized_row2);
    }
    else {
      watchdog('views_callback', 'View: %view is calling undefined Sort function %callback.', array(
        '%callback' => $function,
        '%view' => $this->view->name,
      ), WATCHDOG_CRITICAL);
    }

    return $factor * $result;
  }

}
