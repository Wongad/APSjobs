(function($, Drupal) {
	Drupal.APSJobsBlocksAgencyFilter = Drupal.APSJobsBlocksAgencyFilter || {};
	Drupal.behaviors.actionAPSJobsBlocksAgencyFilter = {
		attach: function(context) {
      Drupal.APSJobsBlocksAgencyFilter.agenciesSearch();
      Drupal.APSJobsBlocksAgencyFilter.showFilterAgenciesSearch();
      Drupal.APSJobsBlocksAgencyFilter.EnterSearch();
      Drupal.APSJobsBlocksAgencyFilter.showPopupCreateAlert();
		}
	}

  Drupal.APSJobsBlocksAgencyFilter.showPopupCreateAlert = function(){
    $(".create-alert-open-popup-save-job .pane-content p a").click(function(e){
      $('#modalSaveSearch').modal('show');
    });
  }

  Drupal.APSJobsBlocksAgencyFilter.EnterSearch = function(){
    $('.page-agencies .form-aps-job-search .title-text-search').on('keypress',function(e) {
      if(e.keyCode == 13) {
        e.preventDefault();
      }
    });
  }

  Drupal.APSJobsBlocksAgencyFilter.showFilterAgenciesSearch = function() {
    $('.search-classify .all-departments').on('click', function(){
      if (!$(this).hasClass('active')) {
        $('.search-classify .all-departments').addClass('active');
        $('.search-classify .by-portfolio').removeClass('active');
        $('.title-text-search').val("");
        $('#edit-title').val("");
        $('#edit-name').val("");
        $('#edit-submit-agency').trigger('click');
      }
    });

    $('.search-classify .by-portfolio').on('click', function(){
      if (!$(this).hasClass('active')) {
        $('.search-classify .by-portfolio').addClass('active');
        $('.search-classify .all-departments').removeClass('active');
        $('.title-text-search').val("");
        $('#edit-title').val("");
        $('#edit-name').val("");
        $('#edit-submit-agency').trigger('click');
      }
    });
  }
  
  Drupal.APSJobsBlocksAgencyFilter.agenciesSearch = function() {
    $('.title-text-search').keyup(function(){
      if ($('.all-departments').hasClass('active')) {
        $('#edit-title').val($(this).val());
        $('#edit-submit-agency').trigger('click');
      }
      if ($('.by-portfolio').hasClass('active')) {
        $('#edit-name').val($(this).val());
        $('#edit-submit-agency').trigger('click');
      }
    });

    if (!$('#agencies-filter-agencies').hasClass('success')) {
      $('#edit-field-portfolio-tid').clone().appendTo('#agencies-filter-agencies');
      $('#agencies-filter-agencies').addClass('success');
    }

    $('#agencies-filter-agencies select').on('change',function(){
      var selected = $(this).val();
      $('.form-item-field-portfolio-tid select').val(selected).change();
      $('#edit-submit-agency').trigger('click');
    });
    
    if (!$('#edit-sort-order-items').hasClass('success')) {
      $('#edit-sort-order').clone().appendTo('#edit-sort-order-items');
      $('#edit-sort-order-items').addClass('success');
    }

    $('#edit-sort-order-items select').on('change',function(){
      var selected = $(this).val();
      $('.form-item-sort-order select').val(selected).change();
      $('#edit-submit-agency').trigger('click');
    });

    $('#agencies-filter-agencies select').on('change',function(){
      var selected = $(this).val();
      $('.form-item-sort-order select').val(selected).change();
      $('#edit-submit-agency').trigger('click');
    });

    // $('#edit-sort-order option').on('click', function(){
    //   $('#edit-submit-agency').trigger('click');
    // });
  }
})(jQuery, Drupal);