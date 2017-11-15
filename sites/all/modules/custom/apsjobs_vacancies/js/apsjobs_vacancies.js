(function($, Drupal) {
  Drupal.apsjobsVacancies = Drupal.apsjobsVacancies || {};
  Drupal.behaviors.actionapsjobsVacancies = {
    attach: function(context) {
      $(window).load(function() {
        //$('#edit-non-ongoing-duration').hide();
      });
      if ($('body').hasClass('page-aps-agency-post-new-vacancy') || $('body').hasClass('page-aps-agency-edit-vacancy')) {
        Drupal.apsjobsVacancies.tooltip();
        Drupal.apsjobsVacancies.chosen();
        Drupal.apsjobsVacancies.clearTitle();
        Drupal.apsjobsVacancies.nonOngoingDuration();
        Drupal.apsjobsVacancies.handleCheckboxes('#edit-working-hours', '#edit-working-hours-all');
        Drupal.apsjobsVacancies.handleCheckboxes('#edit-engagement-types', '#edit-engagement-types-all');
        Drupal.apsjobsVacancies.preventNonNumbericInput('#edit-salary-from');
        Drupal.apsjobsVacancies.preventNonNumbericInput('#edit-salary-to');
      }
    }
  }

  Drupal.apsjobsVacancies.chosen = function() {
    $('#edit-approver').chosen();
  }

  Drupal.apsjobsVacancies.tooltip = function() {
    $('.description a').tooltip({
      open: function (event, ui) {
        ui.tooltip.css('max-height', '100px');
      }
    });
    $('.description a').click(function() {return false;});
  }

  Drupal.apsjobsVacancies.clearTitle = function() {
    $('.form-item.form-type-textfield.form-item-job-title').append('<span class="clear-text">x</span>');
    $('.clear-text').click(function() {
      $(this).parent().find("input").val('').focus();
    });
  }

  Drupal.apsjobsVacancies.nonOngoingDuration = function() {
    if ($('#edit-engagement-types-248').is(':checked')) {
      $('#edit-non-ongoing-duration').removeClass('hidden');
      $('#edit-non-ongoing-duration').chosen();
      $('#edit_non_ongoing_duration_chosen').removeClass('hidden');
    } else {
      $('#edit-non-ongoing-duration').addClass('hidden');
      $('#edit_non_ongoing_duration_chosen').addClass('hidden');
    }
    $('#edit-engagement-types-248').change(function() {
      if ($('#edit-engagement-types-248').is(':checked')) {
        $('#edit-non-ongoing-duration').removeClass('hidden');
        $('#edit-non-ongoing-duration').chosen();
        $('#edit_non_ongoing_duration_chosen').removeClass('hidden');
      } else {
        $('#edit-non-ongoing-duration').addClass('hidden');
        $('#edit_non_ongoing_duration_chosen').addClass('hidden');
      }
    });
  }

  Drupal.apsjobsVacancies.handleCheckboxes = function(wrapper, boxAll) {
    // get all boxes exclude box "All"
    var boxes = $(wrapper + ' :checkbox').not($(boxAll));
    // check box "All" when all boxes were checked
    $(boxAll).prop('checked', $(wrapper + ' input:checked').not($(boxAll)).length == boxes.length);
    // trigger to check box "All"
    boxes.each(function() {
      $(this).change(function() {
        $(boxAll).prop('checked', $(wrapper + ' input:checked').not($(boxAll)).length == boxes.length);
      });
    });
    // check all boxes when box "All" was checked
    $(boxAll).change(function() {
      boxes.prop('checked', $(this).is(':checked'));
      boxes.each(function() {
        $(this).trigger('change');
      });
    });
  }

  Drupal.apsjobsVacancies.preventNonNumbericInput = function(input) {
    $(input).keydown(function (e) {
        //console.log($input.attr('id') + ' key press', e.keyCode);
        // Allow: backspace, delete, tab, escape, enter and .
        if ($.inArray(e.keyCode, [46, 8, 9, 27, 13, 110, 190]) !== -1 ||
             // Allow: Ctrl+A, Command+A
            (e.keyCode === 65 && (e.ctrlKey === true || e.metaKey === true)) ||
             // Allow: home, end, left, right, down, up
            (e.keyCode >= 35 && e.keyCode <= 40)) {
                 // let it happen, don't do anything
                 return;
        }
        // Ensure that it is a number and stop the keypress
        if ((e.shiftKey || (e.keyCode < 48 || e.keyCode > 57)) && (e.keyCode < 96 || e.keyCode > 105)) {
            e.preventDefault();
        }
    });
  }
})(jQuery, Drupal);
