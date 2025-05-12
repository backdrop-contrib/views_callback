<?php

/**
 * Caching plugin that provides cache control based on custom callback.
 *
 * @ingroup views_cache_plugins
 */
class views_callback_plugin_cache extends views_plugin_cache {

  /**
   * Implements views_plugin_cache#summary_title().
   */
  public function summary_title() {
    return t('Custom callback');
  }

  /**
   * Implements views_object#option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['callback_cache_results'] = array('default' => '');
    $options['callback_cache_output'] = array('default' => '');

    return $options;
  }

  /**
   * Implements views_plugin#options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callback_name = 'views_callback__view_' . $this->view->name . '__display_' . $form_state['display_id'] . '__plugin_cache__step_';

    $form += views_callback_form_element($this,
      FALSE,
      array('callback_cache_results', t('Result cache callback'), t('The callback function must return TRUE if the cache is still fresh, FALSE otherwise.'), FALSE),
      array('$view', '$plugin', '$cache'),
      $callback_name . 'results',
    );
    $form += views_callback_form_element($this,
      FALSE,
      array('callback_cache_output', t('Output cache callback'), t('The callback function must return TRUE if the cache is still fresh, FALSE otherwise.'), FALSE),
      array('$view', '$plugin', '$cache'),
      $callback_name . 'output',
    );
  }

  /**
   * Implements views_plugin#options_submit()
   */
  public function options_submit(&$form, &$form_state) {
    $form_state['values']['cache_options'] += $form_state['values']['options'];
  }

  /**
   * Implements views_plugin_cache#cache_get()
   */
  public function cache_get($type) {
    // $cutoff = $this->cache_expire($type);
    switch ($type) {
      case 'query':
        // Not supported currently, but this is certainly where we'd put it.
        return FALSE;

      case 'results':
        $cache = cache_get($this->get_results_key(), $this->bin);
        $fresh = !empty($cache);
        if ($fresh && !empty($this->options['callback_cache_results'])) {
          $function = $this->options['callback_cache_results'];
          if (function_exists($function)) {
            ob_start();
            $fresh = $function($this->view, $this, $cache);
            ob_end_clean();
          }
          else {
            watchdog('views_callback', 'View: %view is calling undefined Cache Results function %callback.', array(
              '%callback' => $function,
              '%view' => $this->view->name,
            ), WATCHDOG_CRITICAL);
          }
        }
        // Values to set: $view->result, $view->total_rows, $view->execute_time,
        // $view->current_page.
        if ($fresh) {
          // @code
          // if (!$cutoff || $cache->created > $cutoff) {
          // @endcode
          $this->view->result = $cache->data['result'];
          $this->view->total_rows = $cache->data['total_rows'];
          $this->view->set_current_page = $cache->data['current_page'];
          $this->view->execute_time = 0;
          return TRUE;
          // }
        }
        return FALSE;

      case 'output':
        $cache = cache_get($this->get_output_key(), $this->bin);
        $fresh = !empty($cache);
        if ($fresh && !empty($this->options['callback_cache_output'])) {
          $function = $this->options['callback_cache_output'];
          if (function_exists($function)) {
            ob_start();
            $fresh = $function($this->view, $this, $cache);
            ob_end_clean();
          }
          else {
            watchdog('views_callback', 'View: %view is calling undefined Cache Output function %callback.', array(
              '%callback' => $function,
              '%view' => $this->view->name,
            ), WATCHDOG_CRITICAL);
          }
        }
        if ($fresh) {
          // @code
          // if (!$cutoff || $cache->created > $cutoff) {
          // @endcode
          $this->storage = $cache->data;
          $this->view->display_handler->output = $cache->data['output'];
          $this->restore_headers();
          return TRUE;
          // }
        }
        return FALSE;
    }
  }

}
