# Views Callback

This module allows an administrator to add fields, filters and sorts
to views which use custom callback functions. The callback functions need to be
placed in a custom module where they will be called.

While this module allows you to directly use a custom callback inside views
which may be useful for quick and easy solutions, it is highly advisable to use
regular handlers and plugins when available (or even to create one yourself).
Take note that filtering and sorting a view using PHP always has a considerable
performance impact.

There is heavily inspired by Views PHP but there is no upgrade path from that
module. It requires all PHP code to be in a module to avoid the danger of having
executable PHP code in the configuration.

## Installation

- Install this module using the [official Backdrop CMS instructions](https://backdropcms.org/guide/modules).
- Add a *Custom callback* field, area, filter, sort, access, or cache.
- Copy the displayed callback signature to a custom module.
- For example, if adding a callback field, you will need at least the Output
  callback. In your custom module you can manipulate the row and return a
  formatted string.
- If changing the callback name, you may need to flush caches so that it will
  recognize the new function.

## Example

With a View with the name `all_content` of content, and two fields: `Content Nid`
and `Title` add a custom callback field. Copy the suggested callback name into a
custom module (the name can be changed if needed). To display the two fields
together like "1 Test title", one could use:

```php
function views_callback__view_all_content__field_callback__step_output($view, $handler, &$static, $row, $data, $value) {
  return "$data->nid $data->node_title";
}
```

## Available handlers

### Area

An (header/footer/empty) area callback that returns a string with optional HTML.

### Field

A field which value and/or output is constructed. Click-sort is also available
and accepts a number of predefined sort algorithms or custom callback.

#### Setup callback

If checked, the setup callback can be used to run once right before the
execution of the view. The callback can be used for functions to be reused in
the value or output callback.

#### Value callback

The Value callback section can be used to contruct the value for this field. The
returned value for the field will then be injected in the `$value` variable of
the Output code section.

Underneath the callback is a list of Available Variables. The `$data` variable
is probably the variable that could be the most useful. `$data` contains a large
array of data containing nested arrays and objects.

The Value callback requires a return statement to be injected into the `$value`
variable. While users can actually skip the Value callback in some use cases,
the benefit of using the Value callback lies in its ability to be referenced
elsewhere, e.g., from another field or table column.

#### Output callback

The Output callback is used to render the field for display. The output returned
by the function can contain HTML. The Output callback receives the `$value`
created in the Value callback if used.

### Filter

A filter based on custom callback to filter out any row. The callback should
return `TRUE` so that the current row is filtered out, and `FALSE` to keep it in
the results. The handler also has a setup callback option.

### Sort

A sort based on custom callback to sort the result of a view. The callback
compares two rows and must return an integer. Returning less than zero means the
first row should appear before the second row; greater than zero means the
second row should appear before the first; and zero means they should remain
as-is.

## Available plugins

### Access

Access control based on custom callback to determine whether a user has
access to a view. If the callback function returns `TRUE` the requesting user is
granted access to the view; `FALSE` otherwise.

### Cache

Cache control based on custom callback to determine whether a view's
(result or output) cache is still fresh. The callback function must return `TRUE`
if the cache is still fresh, `FALSE` otherwise.

## Current Maintainers

- [Herb v/d Dool](https://github.com/herbdool)
- Seeking co-maintainers.

## Credit

Originally maintained on Drupal by:

- <https://www.drupal.org/u/casey>
- <https://www.drupal.org/u/fizk>
- <https://www.drupal.org/u/infojunkie>
- <https://www.drupal.org/u/liam-morland>
- <https://www.drupal.org/u/greg-boggs>
- <https://www.drupal.org/u/idtarzanych>

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
