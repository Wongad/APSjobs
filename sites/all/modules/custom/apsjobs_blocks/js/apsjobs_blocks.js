(function($, Drupal) {
	Drupal.APSJobsBlocks = Drupal.APSJobsBlocks || {};
	Drupal.behaviors.actionAPSJobsBlocks = {
		attach: function(context) {
			$(window).load(function() {
        Drupal.APSJobsBlocks.chosen();
        Drupal.APSJobsBlocks.selectSortOption();
			});
      Drupal.APSJobsBlocks.carouselHomepageStatistics();
      Drupal.APSJobsBlocks.slideToggleAgencyDashboard();
		}
  }

  Drupal.APSJobsBlocks.carouselHomepageStatistics = function() {
    if ($('body').hasClass('front')) {
      $(".aps-left-block .owl-carousel").owlCarousel({
        items: 3,
        singleItem: true,
        itemsScaleUp : true,
        slideSpeed: 500,
        autoPlay: 5000,
        stopOnHover: true
      });
    }
  }

  Drupal.APSJobsBlocks.chosen = function() {
    if (!$('body').hasClass('page-admin') && !$('body').hasClass('page-node')) {
      if ($("#edit_form_salary").length)
        $("#edit_form_salary").chosen();
      if ($("#edit-salary").length)
        $("#edit-salary").chosen();
      if ($('body').hasClass('page-aps-agency-post-new-vacancy') || $('body').hasClass('page-aps-agency-edit-vacancy')) {
        if ($("#edit-job-level").length)
          $("#edit-job-level").chosen();
        //$("#edit-non-ongoing-duration").chosen();
      }
      if ($("#edit-clearance-level").length)
        $("#edit-clearance-level").chosen();
      if ($("#edit-employer").length)
        $("#edit-employer").chosen();
      // $("#edit-nominate-approver-").chosen();
      if ($("#edit-sort-date").length)
        $("#edit-sort-date").chosen({
          "disable_search": true
        });
    }
  }

  Drupal.APSJobsBlocks.slideToggleAgencyDashboard = function() {
    if ($('body').hasClass('page-aps-agency-dashboard')) {
      $('.pane-aps-notificaitons-panel-pane-1 .pane-content').hide();
      $('.pane-aps-vacancy-panel-pane-3 .pane-content').hide();
      $('.pane-aps-vacancy-panel-pane-10 .pane-content').hide();
      $('.pane-aps-outcomes-panel-pane-1 .pane-content').hide();
      $('.pane-view-gazette-panel-pane-2 .pane-content').hide();

      $('.pane-aps-notificaitons-panel-pane-1 .pane-title').click(function() {
        $('.pane-aps-notificaitons-panel-pane-1 .pane-content').slideToggle(500);
      });
      $('.pane-aps-vacancy-panel-pane-3 .pane-title').click(function() {
        $('.pane-aps-vacancy-panel-pane-3 .pane-content').slideToggle(500);
      });
      $('.pane-aps-vacancy-panel-pane-10 .pane-title').click(function() {
        $('.pane-aps-vacancy-panel-pane-10 .pane-content').slideToggle(500);
      });
      $('.pane-aps-outcomes-panel-pane-1 .pane-title').click(function() {
        $('.pane-aps-outcomes-panel-pane-1 .pane-content').slideToggle(500);
      });
      $('.pane-view-gazette-panel-pane-2 .pane-title').click(function() {
        $('.pane-view-gazette-panel-pane-2 .pane-content').slideToggle(500);
      });
    }
  }

  Drupal.APSJobsBlocks.selectSortOption = function() {
    if ($('#edit_sort_date_chosen').length) {
      $('#edit_sort_date_chosen .chosen-single').on('click', function() {
        var $parent = $(this).parent();
        if ($parent.hasClass('chosen-with-drop')) {
          $parent.find('.chosen-drop ul li.result-selected').hide();
        }
      });
    }
  }

})(jQuery, Drupal);
