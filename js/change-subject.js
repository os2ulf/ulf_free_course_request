/**
 * Change subject when course is selected.
 **/
(function ($) {
  "use strict";
  Drupal.behaviors.free_course_request_change_subject = {
    attach: function (context, settings) {
      var subjectSelector = $('.data-selected-subject');
      var subjectDropdownSelector = $('#edit-field-rfc-subject-und');
      // Selector appears as part of ajax _rfc_show_price_info change function.

      // Clear values
      subjectDropdownSelector.empty();
      // Add _none as option.
      subjectDropdownSelector.append(
        $('<option>', {
          value: '_none',
          text: '- Select a subject  -'
        }, '</option>'));

      // Populate with topic elements from vocabulary.
      Object.keys(settings.fcr.subjects).forEach(function setChoices(item, index) {
        subjectDropdownSelector.append(
          $('<option>', {
            value: item,
            text: settings.fcr.subjects[item]
          }, '</option>'));
      });
      $(subjectDropdownSelector).trigger("chosen:updated");

      // If subjects data exists.
      if($(subjectSelector).length) {
        var subjectIds = $(subjectSelector).data('course-subject');
        // If multiple IDs.
        if (subjectIds.toString().search(':') >= 0) {
          var subjects = subjectIds.split(':');
          // Clear list and append options from course.
          subjectDropdownSelector.empty();
          subjects.forEach(function setChoices(item, index) {
            subjectDropdownSelector.append(
              $('<option>', {
                value: item,
                text: settings.fcr.subjects[item]
              }, '</option>'));
          });
          $(subjectDropdownSelector).trigger("chosen:updated");
        }
        else {
          // Clear list and append option from course.
          subjectDropdownSelector.empty();
          subjectDropdownSelector.append(
          $('<option>', {
            value: subjectIds,
            text: settings.fcr.subjects[subjectIds]
          }, '</option>'));
          $(subjectDropdownSelector).val(subjectIds);
          $(subjectDropdownSelector).trigger("chosen:updated");
        }
      }
    }
  }
}(jQuery));
