<?php

/**
 * Access plugin that provides access control based on a custom callback.
 *
 * @ingroup views_access_plugins
 */
class views_callback_plugin_access extends views_plugin_access {

  /**
   * Implements views_plugin_access#summary_title().
   */
  public function summary_title() {
    return t('Custom callback');
  }

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['callback_access'] = array('default' => '');

    return $options;
  }

  /**
   * Implements views_plugin#options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__plugin_access';

    $form += views_callback_form_element($this,
      FALSE,
      array('callback_access', t('Access callback'), t('If the callback function returns TRUE the requesting user is granted access to the view; FALSE otherwise.'), FALSE),
      array(
        '$view_name' => t('The name of the view to check.'),
        '$display_id' => t('The ID of the display to check.'),
        '$account' => t('The account to check.'),
      ),
      $callback_name,
    );
  }

  /**
   * Implements views_plugin#options_submit().
   */
  public function options_submit(&$form, &$form_state) {
    $form_state['values']['access_options']['callback_access'] = $form_state['values']['options']['callback_access'];
  }

  /**
   * Implements views_plugin_access#access().
   */
  public function access($account) {
    if (!empty($this->options['callback_access']) && function_exists($this->options['callback_access'])) {
      return views_callback_check_access($this->options['callback_access'], $this->view->name, $this->display->id, $account);
    }
    return TRUE;
  }

  /**
   * Implements views_plugin_access#get_access_callback().
   */
  public function get_access_callback() {
    if (!empty($this->options['callback_access']) && function_exists($this->options['callback_access'])) {
      return array('views_callback_check_access', array($this->options['callback_access'], $this->view->name, $this->display->id));
    }
    else {
      watchdog('views_callback', 'View: %view is calling undefined Access Value function %callback.', array(
        '%callback' => $this->options['callback_access'],
        '%view' => $this->view->name,
      ), WATCHDOG_CRITICAL);
    }
    return TRUE;
  }

}
