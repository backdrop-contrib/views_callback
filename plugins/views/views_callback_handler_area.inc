<?php

/**
 * A handler to provide an area that is constructed by the administrator using PHP.
 *
 * @ingroup views_area_handlers
 */
class views_callback_handler_area extends views_handler_area {

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['callback_output'] = array('default' => "");
    return $options;
  }

  /**
   * Implements views_handler#option_definition().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__handler_area_' . $form_state['id'];

    $form += views_callback_form_element($this,
      FALSE,
      array('callback_output', t('Output callback'), t('Callback function to render the output of this area. Return a string with optional HTML to be displayed.'), TRUE),
      array('$view', '$handler', '$results'),
      $callback_name,
    );
  }

  /**
   * Implements views_handler_area#render().
   */
  public function render($empty = FALSE) {
    // Execute output a custom callback.
    if ((!$empty || !empty($this->options['empty'])) && !empty($this->options['callback_output'])) {
      $function = $this->options['callback_output'];
      if (function_exists($function)) {
        ob_start();
        echo $function($this->view, $this, $this->view->result);
        return ob_get_clean();
      }
      else {
        watchdog('views_callback', 'View: %view is calling undefined Area Output function %callback.', array(
          '%callback' => $function,
          '%view' => $this->view->name,
        ), WATCHDOG_CRITICAL);
      }
    }
    return '';
  }

}
