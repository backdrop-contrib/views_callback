<?php

/**
 * A (fake) pager plugin that wraps around the actual query.
 *
 * @ingroup views_query_plugins
 */
class views_callback_plugin_query extends views_callback_plugin_wrapper {

  /**
   * Implements views_plugin_query#execute().
   */
  public function execute(&$view) {
    $pager = new views_callback_plugin_pager();
    $pager->callback_wrap($this->wrapped->pager);

    $this->wrapped->execute($view);

    $pager->callback_unwrap();
  }

}
