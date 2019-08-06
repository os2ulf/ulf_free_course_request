/**
 * Add popup with range limit.
 **/
(function ($) {
  "use strict";
  Drupal.behaviors.free_course_request_form = {
    attach: function (context, settings) {
      var from = settings.fcr.from;
      var to = settings.fcr.to;
      $('.field-name-field-rfc-date input').datepicker({
        minDate: new Date(from[0], parseInt(from[1]) + 1, from[2]),
        maxDate: new Date(to[0], parseInt(to[1]) + 1, to[2]),
        dateFormat: 'dd/mm/yy'
      });
    }
  }
}(jQuery));