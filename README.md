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
- If changing the callback name, you may need to flush caches so that it will
  recognize the new function.

## Available handlers

### Area

An (header/footer/empty) area which output is constructed by the administrator
using PHP.

### Field

A field which value and/or output is constructed by the administrator using PHP.
Click-sort is also available and accepts a number of predefined sort algorithms
or custom callback.

### Filter

A filter based on custom callback; use PHP to filter out any row.

### Sort

A sort based on custom callback; use PHP to sort the result of a view.

## Available plugins

### Access

Access control based on custom callback; use PHP to determine whether a user has
access to a view.

### Cache

Cache control based on custom callback; use PHP to determine whether a view's
(result/output) cache is still fresh.

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
