(function($, Drupal) {
	Drupal.APSJobsOutComes = Drupal.APSJobsOutComes || {};
	Drupal.behaviors.actionAPSJobsOutComes = {
		attach: function(context) {
      $(window).load(function(){
        Drupal.APSJobsOutComes.chosen();
        Drupal.APSJobsOutComes.dropDown();
        Drupal.APSJobsOutComes.disableInvalidOutcome();
        Drupal.APSJobsOutComes.validateNumber();
      });      
		}
	}

  Drupal.APSJobsOutComes.dropDown = function(){
    $('span.create-outcome').on('click',function(e) {
      $(this).parent().toggleClass('open');
    });
  }

  Drupal.APSJobsOutComes.disableInvalidOutcome = function(){
    if ('apsjobsOutcome' in Drupal.settings && Drupal.settings.apsjobsOutcome.hasOwnProperty('invalidLevelTids')) {
      var invalidLevelTids = Drupal.settings.apsjobsOutcome.invalidLevelTids;
      if (invalidLevelTids != undefined && invalidLevelTids.length) {
        $.each(invalidLevelTids, function(index, value) {
          $('#edit-from-job-level option[value=' + value + ']').attr("disabled", true);
        })
      }
      if ($("#edit-from-job-level").length)
        $("#edit-from-job-level").chosen();
    }
  }

  Drupal.APSJobsOutComes.chosen = function() {    
    if ($("#edit-section-of-the-act").length) 
      $("#edit-section-of-the-act").chosen();
  }

  Drupal.APSJobsOutComes.validateNumber = function() {
    $('#edit-ags-number').on('keydown', function(e) {
      // Allow: backspace, delete, tab, escape, enter and .
      if ($.inArray(e.keyCode, [46, 8, 9, 27, 13, 110, 190]) !== -1 ||
        // Allow: Ctrl+A, Command+A
        (e.keyCode === 65 && (e.ctrlKey === true || e.metaKey === true)) || 
          // Allow: home, end, left, right, down, up
        (e.keyCode >= 35 && e.keyCode <= 40)) {
          return;
      }
      // Ensure that it is a number and stop the keypress
      if ((e.shiftKey || (e.keyCode < 48 || e.keyCode > 57)) && (e.keyCode < 96 || e.keyCode > 105)) {
        e.preventDefault();
      }
    });
  }
})(jQuery, Drupal);