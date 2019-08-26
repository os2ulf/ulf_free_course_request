/**
 * Add Remove course selection and price info, course not found is selected.
 **/
(function ($) {
  "use strict";
  Drupal.behaviors.free_course_request_remove_course = {
    attach: function (context, settings) {
      $('#edit-field-rfc-course-not-found-und').change(function() {
        $('#edit-field-rfc-course-und').val('_none');
        $('#edit-field-rfc-course-und').trigger("chosen:updated");
        $('.price-info').remove();
      });
    }
  }
}(jQuery));