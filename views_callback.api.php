<?php

/**
 * @file
 * Hooks and callbacks
 */

/**
 * Output callback for Header, Footer and No Results areas.
 *
 * @param view $view
 * @param views_callback_handler_area $handler
 * @param object $results
 *
 * @return string
 */
function callback_area_output($view, $handler, $results) {}

/**
 * Setup callback for Field
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 *
 * @return mixed
 */
function callback_field_setup($view, $handler, &$static) {}

/**
 * Value callback for Field
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 * @param object $row
 * @param object $data
 *
 * @return mixed
 */
function callback_field_value($view, $handler, &$static, $row, $data) {}

/**
 * Sort comparison callback for Field
 *
 * @param view $view
 * @param object $row1
 * @param object $row2
 *
 * @return int
 *   Return less than zero for the first row to appear before the second;
 *   return zero to remain the same; or greater than zero for the first to
 *   appear after the second row.
 */
function callback_field_comparison($view, $row1, $row2) {}

/**
 * Output callback for Field
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 * @param object $row
 * @param object $data
 * @param mixed $value
 *
 * @return string
 */
function callback_field_output($view, $handler, &$static, $row, $data, $value) {}

/**
 * Setup callback for Filter
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 *
 * @return mixed
 */
function callback_filter_setup($view, $handler, &$static) {}

/**
 * Filter callback
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 * @param object $row
 * @param object $data
 *
 * @return bool
 *   Return TRUE to remove the current row from the results; FALSE otherwise.
 */
function callback_filter_value($view, $handler, &$static, $row, $data) {}

/**
 * Setup callback for Sort
 *
 * @param view $view
 * @param views_callback_handler_field $handler
 * @param array $static
 *
 * @return mixed
 */
function callback_sort_setup($view, $handler, &$static) {}

/**
 * Comparison callback for Sort
 *
 * @param view $view
 * @param object $row1
 * @param object $row2
 *
 * @return int
 *   Return less than zero for the first row to appear before the second;
 *   return zero to remain the same; or greater than zero for the first to
 *   appear after the second row.
 */
function callback_sort_comparison($view, $row1, $row2) {}

/**
 * Access callback
 *
 * @param string $view_name
 * @param string $display_id
 * @param User $account
 *
 * @return bool
 */
function callback_access($view_name, $display_id, $account) {}

/**
 * Result callback for Cache
 *
 * @param view $view
 * @param object $plugin
 * @param object $cache
 *
 * @return bool
 *   Return TRUE if the cache is still fresh, FALSE otherwise.
 */
function callback_cache_result($view, $plugin, $cache) {}

/**
 * Output callback for Cache
 *
 * @param view $view
 * @param object $plugin
 * @param object $cache
 *
 * @return bool
 *   Return TRUE if the cache is still fresh, FALSE otherwise.
 */
function callback_cache_output($view, $plugin, $cache) {}
