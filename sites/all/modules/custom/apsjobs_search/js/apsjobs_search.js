(function($, Drupal) {
	Drupal.APSJobsSearch = Drupal.APSJobsSearch || {};
	Drupal.behaviors.actionAPSJobsSearch = {
		attach: function(context) {
			$(window).load(function() {
				if ($('.advanced-search').length) {
					$('.advanced-search .advanced').hide();
        }
        if ($('body').hasClass('page-search-jobs')) {
          Drupal.APSJobsSearch.agencyGetParam();
        }
			  Drupal.APSJobsSearch.showHideAdvancedSearch();
        Drupal.APSJobsSearch.advancedSearchTrigger();
        Drupal.APSJobsSearch.locationAutoComplete();
        Drupal.APSJobsSearch.changeSalaryRate();
        Drupal.APSJobsSearch.changeSelection();
        Drupal.APSJobsSearch.agencyFilter();
        Drupal.APSJobsSearch.dropdownAdvanceSearch();
        Drupal.APSJobsSearch.advanceShowMore();
        Drupal.APSJobsSearch.checkAll();
        Drupal.APSJobsSearch.selectedAdvanceSearch();
      });

      $( document ).ajaxComplete(function(event, xhr, settings) {
        $('.form-item-find-button img').hide();
      });
		}
  }

  Drupal.APSJobsSearch.advanceShowMore = function() {
    var items = $('.view-id-job_category.view-display-id-panel_pane_1 .views-row');
    $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').on('click', function(event){
      event.preventDefault();
      if($(window).width() > 991 ){
        if($('.view-id-job_category.view-display-id-panel_pane_1 .views-row.desktop-show').length < items.length) {
          var items_hidden = $('.view-id-job_category.view-display-id-panel_pane_1 .views-row').not('.desktop-show');
          if(items_hidden.length < 16 ) {
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View less');
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').addClass('view-less-custom');
          }
          items_hidden.each(function(index, value){
            if(index < 16) {
              $(this).addClass('desktop-show');
            }
          });

          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 500 }, 500);

        } else {
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View more');
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').removeClass('view-less-custom');
          items.each(function(index, value){
            if( index > 15) {
              $(this).removeClass('desktop-show');
            }
          });
          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 500 }, 500);
        }
      }
      else if ($(window).width() > 640) {
        if($('.view-id-job_category.view-display-id-panel_pane_1 .views-row.tablet-show').length < items.length) {
          var items_hidden = $('.view-id-job_category.view-display-id-panel_pane_1 .views-row').not('.tablet-show');
          if(items_hidden.length < 9 ) {
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View less');
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').addClass('view-less-custom');
          }
          items_hidden.each(function(index, value){
            if(index < 8) {
              $(this).addClass('tablet-show');
            }
          });
          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 700 }, 500);
        } else {
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View more');
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').removeClass('view-less-custom');
          items.each(function(index, value){
            if( index > 7) {
              $(this).removeClass('tablet-show');
            }
          });
          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 700 }, 500);
        }
      } else {
        if($('.view-id-job_category.view-display-id-panel_pane_1 .views-row.mobile-show').length < items.length) {
          var items_hidden = $('.view-id-job_category.view-display-id-panel_pane_1 .views-row').not('.mobile-show');
          if(items_hidden.length < 3 ) {
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View less');
            $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').addClass('view-less-custom');
          }
          items_hidden.each(function(index, value){
            if(index < 2) {
              $(this).addClass('mobile-show');
            }
          });
          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 700 }, 500);
        } else {
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').text('View more');
          $('.view-id-job_category.view-display-id-panel_pane_1 .toc-filter-processed').removeClass('view-less-custom');
          items.each(function(index, value){
            if( index > 1) {
              $(this).removeClass('mobile-show');
            }
          });
          $('html, body').animate({ scrollTop: jQuery('.view-more .toc-filter-processed').offset().top - 700 }, 500);
        }
      }
    });
  }

	Drupal.APSJobsSearch.showHideAdvancedSearch = function() {
    var $adv_search = $('.advanced-search');
		$adv_search.find('.togled').on('click', function(e) {
			if ($adv_search.hasClass('show')) {
				$adv_search.find('.advanced').hide();
				$adv_search.removeClass('show');
				$(this).find('span').text('Advanced');
			} else {
				$adv_search.find('.advanced').show();
				$adv_search.addClass('show');
				$(this).find('span').text('Hide Advanced');
			}
    });
    //group-search-togle
    var $custom_adv_search = $('.custom-advance-search');
    $custom_adv_search.find('.fitler-togled').on('click', function(e) {
			if ($custom_adv_search.hasClass('show')) {
        $('#banner').addClass('group-search-togle');
				$custom_adv_search.find('.form-aps-job-secrh').hide();
				$custom_adv_search.removeClass('show');
				$(this).find('span').text('Show Filters');
			} else {
        $('#banner').removeClass('group-search-togle');
				$custom_adv_search.find('.form-aps-job-secrh').show();
				$custom_adv_search.addClass('show');
				$(this).find('span').text('Hide Filters');
			}
			e.preventDefault();
    });
  }

  Drupal.APSJobsSearch.advancedSearchTrigger = function() {
    var $form_exposed = $('#views-exposed-form-aps-vacancy-search-panel-pane-1');
    $form_exposed.find('.form-item input[type="checkbox"]').each(function(){
      if($(this).is(':checked')) {
        $('.custom-advance-search input[value="'+ $(this).val() +'"]').prop('checked',$(this).is(':checked'));
      }
    });
    //page home to page search
    $('.job-level-all').each(function() {
      var boxAll = $(this);
      boxAll.closest('ul').find(':checkbox').each(function() {
        var boxes = boxAll.closest('ul').find(':checkbox').not(boxAll);
        for (var i = 0; i < boxes.length; i++) {
          if (!boxes[i].checked) {
            boxAll.prop('checked', false);
            return;
          }
        }
        boxAll.prop('checked', true);
      });
    });

    $('.custom-advance-search .advanced-search-job-level ul li input').change(function(){
      var isCheck = $('#ajob-level' + $(this).val()).is(':checked') || $(this).is(':checked')
      $('.form-item-job-level .form-item-edit-job-level-' + $(this).val() + ' input').prop('checked',isCheck);
    });

    $('#job-level-all-266, #job-level-all-206, #job-level-all-207, #job-level-all-208').on('change', function() {
      var all = $(this);
      var part_id = $(this).attr('id').split('-');
      var aid = part_id[part_id.length - 1];
      var isCheck = $('#job-level-all2-' + aid).is(':checked') || all.is(':checked');
      $(this).closest('ul').find(':checkbox:not(:first)').each(function() {
        $('.form-item-job-level .form-item-edit-job-level-' + $(this).val() + ' input').prop('checked',isCheck);
      });
    });


    //for advanced search
    $('.custom-advance-search .advanced-search-job-level2 ul li input').change(function(){
      var isCheck = $('#job-level' + $(this).val()).is(':checked') || $(this).is(':checked');
      $('.form-item-job-level .form-item-edit-job-level-' + $(this).val() + ' input').prop('checked',isCheck);
    });

    $('#job-level-all2-266, #job-level-all2-206, #job-level-all2-207, #job-level-all2-208').on('change', function() {
      var all = $(this);
      var part_id = $(this).attr('id').split('-');
      var aid = part_id[part_id.length - 1];
      var isCheck = $('#job-level-all-' + aid).is(':checked') || all.is(':checked');
      $(this).closest('ul').find(':checkbox:not(:first)').each(function() {
        $('.form-item-job-level .form-item-edit-job-level-' + $(this).val() + ' input').prop('checked',isCheck);
      });
    });
    //
    $('.custom-advance-search .advanced-search-job-category ul li input').change(function(){
        $('.form-item-job-category .form-item-edit-job-category-' + $(this).val() + ' input').prop('checked',$(this).is(':checked'));
    });

    $('.custom-advance-search ul#agency-list li input').change(function(){
      if ($(this).is(":checked")){
        $('.form-item-agency-ref select option[value=' + $(this).val()  + ']').attr('selected', 'selected');
      }
      else
        $('.form-item-agency-ref select option[value=' + $(this).val()  + ']').removeAttr('selected');
    });

    $('.custom-advance-search .advanced-search-job-type .working-hours ul li input').change(function(){
        $('.form-item-working-hours .form-item-edit-working-hours-' + $(this).val() + ' input').prop('checked',$(this).is(':checked'));
    });

    $('.custom-advance-search .advanced-search-job-type .engagement-type ul li input').change(function(){
        $('.form-item-engagement-types .form-item-edit-engagement-types-' + $(this).val() + ' input').prop('checked',$(this).is(':checked'));
    });

    $('.custom-advance-search .advanced-search-job-type .specific-considerations ul li input').change(function(){
        $('.form-item-specific-considerations .form-item-edit-specific-considerations-' + $(this).val() + ' input').prop('checked',$(this).is(':checked'));
    });

    $('.custom-advance-search .advanced-search-clearance-level ul li input').change(function(){
        $('.form-item-clearance-level .form-item-edit-clearance-level-' + $(this).val() + ' input').prop('checked',$(this).is(':checked'));
    });

    $('.form-search-jobs #edit_form_salary').val($('.form-item-salary-range select').val());
    $('.form-search-jobs #edit_form_salary').trigger("chosen:updated");
    $('.form-search-jobs #edit_form_salary').on('change',function(){
      $('.form-item-salary-range select').val($(this).val());
      //
      var range = $(this).find('option:selected').text();
      if ($(this).val() == 'All') {
        $("#edit-field-salary-to").val('');
        $("#edit-field-salary-from").val('');
      }
      var rangeValues = range.split(' ');
      if (rangeValues.length == 3) {
        var from = Number(rangeValues[0].replace(/[^0-9\.-]+/g,""));
        var to = Number(rangeValues[2].replace(/[^0-9\.-]+/g,""));
        $("#edit-field-salary-from").val(to);
        $("#edit-field-salary-to").val(from);
      }
      else if (rangeValues.length == 2) {
        if (rangeValues[0] == 'To') {
          var to = Number(rangeValues[1].replace(/[^0-9\.-]+/g,""));
          $("#edit-field-salary-from").val(to);
          $("#edit-field-salary-to").val(0);
        } else if (rangeValues[0] == 'Upper') {
          var from = Number(rangeValues[1].replace(/[^0-9\.-]+/g,""));
          $("#edit-field-salary-from").val('');
          $("#edit-field-salary-to").val(from);
        }
      }
    });

    $('.custom-advance-search div.item-add-jobs').on('click', function(){
      $('.form-item-find-button img').show();
      $('.views-exposed-form #edit-key-location').val($(".custom-advance-search #edit-key-location").val());
      Drupal.APSJobsSearch.saveSearchOption();
      if ($('body').hasClass('page-search-jobs'))
        $('.form-item-salary-range select').val('All');
      $('#edit-submit-aps-vacancy-search').trigger('click');
    });

    $('.form-search-jobs .form-item-key-word input').val($form_exposed.find('.form-item-keyword input').val());
    $('.form-search-jobs .form-item-key-word input').keyup(function(){
      $form_exposed.find('.form-item-keyword input').val($(this).val());
    });

    //location
    $('.form-search-jobs .form-item-key-location input').val($form_exposed.find('.form-item-key-location input').val());
    $('.form-search-jobs .form-item-key-location input').keyup(function(){
      $form_exposed.find('.form-item-key-location input').val($(this).val());
    });
    $('.form-search-jobs .form-item-key-location input').on('autocompletechange', function(){
      $form_exposed.find('.form-item-key-location input').val($(this).val());
    });
    //

    $('.custom-advance-search .advanced-search-agency ul li input').change(function(){
      var self = $(this);
      var selectedVals = [];
      var selected = self.closest('ul').find('li input:checked');
      selected.each(function() {
        selectedVals.push($(this).val());
      });
      var $agencyRef = $form_exposed.find('.form-item-agency-ref select');
      $agencyRef.val(selectedVals);
    });
  }

  Drupal.APSJobsSearch.customSearchDropdown = function() {
    $(".advanced-search-agency .form-filter input").keyup(function(){
      var input, filter, ul, li, a, i;
      input = $(this);
      filter = input.val().toUpperCase();
      li = $(".advanced-search-agency ul li");
      for (i = 0; i < li.length; i++) {
          a = li[i].getElementsByTagName("a")[0];
          if (a.innerHTML.toUpperCase().indexOf(filter) > -1) {
              li[i].style.display = "";
          } else {
              li[i].style.display = "none";
          }
      }
    });
  }

  Drupal.APSJobsSearch.dropdownAdvanceSearch = function() {
    Drupal.APSJobsSearch.selectedClassification();
    $('body').on( 'click', function( event ) {
      $('.custom-advance-search .dropdown').removeClass('open');
      $('.views-field-field-outcome .field-content .dropdown').removeClass('open');
    });

    $(".dropdown").click(function(e){
      e.stopPropagation();
    });
    $('.custom-advance-search .dropdown a.dropdown-toggle').on('click', function(event){
      var parent = $(this).parent();
      if(parent.hasClass('open')) {
        parent.toggleClass('open');
      } else {
        $('.custom-advance-search .dropdown').removeClass('open');
        parent.toggleClass('open');
      }
    });

    $('.item-classification').on('click',function(e) {
      $(this).parent().toggleClass('open');
    });
  }

  Drupal.APSJobsSearch.agencyFilter = function() {
    $('.form-filter span.btn-clear').hide();
    $('.form-filter input').on('keyup', function() {
      var $this = $(this);
      var filter = $this.val().toUpperCase();
      if (filter == '')
        $('.form-filter span.btn-clear').hide();
      else
        $('.form-filter span.btn-clear').show();

      $("#agency-list li").each(function() {
        var self = $(this);
        var label = self.find('label');
        if (label.html().toUpperCase().indexOf(filter) > -1) {
          self.show();
        } else {
          self.hide();
        }
      });
    });

    $('.form-filter span.btn-clear').on('click', function() {
      $('.form-filter input').val('');
      $('.form-filter input').focus();
      $(this).hide();

    })
  }

  Drupal.APSJobsSearch.locationAutoComplete = function() {
    $('.form-search-jobs .form-item-key-location input').autocomplete({
      source: "aps-location/autocomplete",
      minLength: 1
    });
  }

  Drupal.APSJobsSearch.sliderSalary = function(min, max, step, pipStep, start, end) {
    $( "#slider-salary-range" ).slider({
      step: step,
      range: true,
      min: min,
      max: max,
      values: [ start, end ],
      slide: function( event, ui ) {
        var from = ui.values[ 0 ];
        var to = ui.values[ 1 ];
        $('.form-search-jobs #edit_form_salary').val('All');
        $('.form-search-jobs #edit_form_salary').trigger("chosen:updated");

        $( ".select-range .from-val" ).text( "$" + from);
        $( ".select-range .to-val" ).text( "$" + to);
        $("#edit-field-salary-to").val(from);
        $("#edit-field-salary-from").val(to);
        // if (to < max)
        //   $("#edit-field-salary-from").val(to);
        // else
        //   $("#edit-field-salary-from").val('');
      }
    })
    .slider("pips", {
      rest: "label",
      prefix: "$",
      step: pipStep
    })
    .slider("float");
    var min = $( "#slider-salary-range" ).slider( "values", 0 );
    var max = $( "#slider-salary-range" ).slider( "values", 1 );
    $( ".select-range .from-val" ).text( "$" + min );
    $( ".select-range .to-val" ).text( "$" + max );
  }

  Drupal.APSJobsSearch.changeSalaryRate = function() {
    Drupal.APSJobsSearch.sliderSalary(20000, 500000, 1000, 480, 20000, 500000);
    $('.type-rate ul > li .salary-type').on('click', function() {
      var self = $(this);
      self.closest('ul').find('li').removeClass('active');
      self.parent().addClass('active');
      var salaryType = self.attr('data-type');
      if (salaryType == 'annually') {
        //reset
        Drupal.APSJobsSearch.sliderSalary(20000, 500000, 1000, 480, 20000, 500000);
        $("#edit-field-salary-to").val(20000);
        $("#edit-field-salary-from").val(500000);

        $('.form-item-salary-type .form-item-edit-salary-type-' + salaryType + ' input').prop('checked', true);
        $('.form-item-salary-type .form-item-edit-salary-type-hourly input').prop('checked', false);
      } else if(salaryType == 'hourly') {
        Drupal.APSJobsSearch.sliderSalary(17, 300, 1, 283, 17, 300);
        $('.form-item-salary-type .form-item-edit-salary-type-' + salaryType + ' input').prop('checked', true);
        $('.form-item-salary-type .form-item-edit-salary-type-annually input').prop('checked', false);
        $("#edit-field-salary-to").val(17);
        $("#edit-field-salary-from").val(300);
      }
    })
  }

  Drupal.APSJobsSearch.changeSelection = function() {
    if ($('body').hasClass('front')) {
      localStorage.removeItem('searchSwitchSelection');
    }
    if (localStorage.getItem('searchSwitchSelection') !== null) {
      var selected = localStorage.getItem('searchSwitchSelection');
      if (selected == 'classification'){
        $(".form-item-salary-range").hide();
        $(".classification").show();
        $(this).html('Salary');
      }
      else {
        var options = selected.split('-');
        if (options[1] == 'annually') {
          Drupal.APSJobsSearch.loadSearchOption(20000, 500000, 1000, 480);
          $('.type-rate ul > li').first().addClass('active');
          $('.type-rate ul > li').last().removeClass('active');
        }
        else if (options[1] == 'hourly') {
          Drupal.APSJobsSearch.loadSearchOption(17, 300, 1, 283);
          $('.type-rate ul > li').last().addClass('active');
          $('.type-rate ul > li').first().removeClass('active');
        }
        $(".classification").hide();
        $(".form-item-salary-range").show();
        $(".advanced-search-job-level ul li input").prop('checked', false);
        if (!$('.advanced-search-job-level2 ul li input:checked').length)
          $(".form-item-job-level input").prop('checked', false);
        $(this).html('Classification');

        // $("#edit-field-salary-to").val('');
        // $("#edit-field-salary-from").val('');
      }
    }
    var salaryType = $('.type-rate ul li.active').find('.salary-type').attr('data-type');
    $('.form-item-salary-type .form-item-edit-salary-type-' + salaryType + ' input').prop('checked', true);

    $("#switch-select-slider").on("click", function() {
      if ($(".form-item-salary-range").is(":visible")) {
        $(".form-item-salary-range").hide();
        Drupal.APSJobsSearch.sliderSalary(20000, 500000, 1000, 480, 20000, 500000);
        $('.type-rate ul > li').first().addClass('active');
        $('.type-rate ul > li').last().removeClass('active');
        $(".classification").show();
        $(this).html('Salary');
        localStorage.setItem('searchSwitchSelection', 'classification');
      } else {
        $(".classification").hide();
        $(".form-item-salary-range").show();
        $(".advanced-search-job-level ul li input").prop('checked', false);
        $(".form-item-job-level input").prop('checked', false);
        $(this).html('Classification');
        localStorage.setItem('searchSwitchSelection', 'salary');

        $("#edit-field-salary-to").val('');
        $("#edit-field-salary-from").val('');
      }
    });
  }

  Drupal.APSJobsSearch.agencyGetParam = function() {
    var selectedAgencyRef = [];
    $('.form-item-agency-ref select option[selected=selected]').each(function() {
      $('ul#agency-list li input[value='+ $(this).val() + ']').prop('checked', true);
    })
    var param = Drupal.APSJobsSearch.getParameterByName('agency_ref');
    if (param != null) {
      $('ul#agency-list li input[value='+param + ']').prop('checked', true);
    }
    var selected = [];
    selected.push(param);
    var $form_exposed = $('#views-exposed-form-aps-vacancy-search-panel-pane-1');
    $form_exposed.find('.form-item-agency-ref select').val(selected);
    $('#edit-submit-aps-vacancy-search').trigger('click');
  }

  Drupal.APSJobsSearch.getParameterByName = function(name, url) {
    if (!url) url = window.location.href;
    name = name.replace(/[\[\]]/g, "\\$&");
    var regex = new RegExp("[?&]" + name + "(=([^&#]*)|&|#|$)"),
        results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, " "));
  }

  Drupal.APSJobsSearch.selectedClassification = function() {
    var selectedNum = $(".advanced-search-job-level ul li input:checked").length;
    var selectedAll = $(".advanced-search-job-level ul li input[value=all]:checked").length;
    if (selectedNum)
      $('.item-classification span').first().html(selectedNum - selectedAll + ' Classifications');
    else
      $('.item-classification span').first().html('Any Classification');
  }

  Drupal.APSJobsSearch.selectedJobLevel = function() {
    var selectedNum = $(".advanced-search-job-level2 ul li input:checked").length;
    var selectedAll = $(".advanced-search-job-level2 ul li input[value=all]:checked").length;
    if (selectedNum){
      var txt = selectedNum - selectedAll;
      $('.advanced-search-job-level2 a.dropdown-toggle .menu-label').html(txt);
      if (txt <= 1) {        
        $('.advanced-search-job-level2 a.dropdown-toggle .plural').html("Job Level");
      }
      else {
        $('.advanced-search-job-level2 a.dropdown-toggle .plural').html("Job Levels");
      }
    }
    else{
      $('.advanced-search-job-level2 a.dropdown-toggle .menu-label').html('Any');
      $('.advanced-search-job-level2 a.dropdown-toggle .plural').html("Job Level");
    }
  }

  Drupal.APSJobsSearch.selectedSameLevelSearch = function($selectElement, single, plural) {
    //.advanced-search-job-category
    var $optionsJobCategory = $($selectElement + " .dropdown-block ul");
    $optionsJobCategory.find("li input").on('change', function() {
      var selectedNum = $optionsJobCategory.find("li input:checked").length;
      var selectedAll = $optionsJobCategory.find("li input[value=all]:checked").length;
      if (selectedNum) {
        var txt = (selectedNum - selectedAll);
        $($selectElement + ' a.dropdown-toggle .menu-label').html(txt);
        if (txt <= 1)
          $($selectElement + ' a.dropdown-toggle span.plural').html(single);
        else
          $($selectElement + ' a.dropdown-toggle span.plural').html(plural);
      }
      else {
        $($selectElement + ' a.dropdown-toggle .menu-label').html('Any');
        $($selectElement + ' a.dropdown-toggle .plural').html(single);
      }
    });
  }

  Drupal.APSJobsSearch.selectedAdvanceSearch = function (){
    Drupal.APSJobsSearch.selectedSameLevelSearch(".advanced-search-job-category", "Occupation", "Occupations");
    Drupal.APSJobsSearch.selectedSameLevelSearch(".advanced-search-agency", "Agency", "Agencies");
    Drupal.APSJobsSearch.selectedSameLevelSearch(".advanced-search-job-type", "Job Type", "Job Types");
    Drupal.APSJobsSearch.selectedSameLevelSearch(".advanced-search-clearance-level", "Clearance Level", "Clearance Levels");    
  }

  Drupal.APSJobsSearch.saveSearchOption = function() {
    var option;
    if ($(".form-item-salary-range").is(":visible")) {
      var rate = $('.type-rate ul li.active div').attr('data-type');
      option = 'salary-' + rate;
    }
    else {
      option = 'classification';
    }
    localStorage.setItem('searchSwitchSelection', option);
  }

  Drupal.APSJobsSearch.loadSearchOption = function(min, max, step, pipStep) {
    var start = min;
    var end = max;
    var from = $("#edit-field-salary-to").val();
    var to = $("#edit-field-salary-from").val();
    if (from !== null && from !== undefined && from !== '') {
      if (from == max) {
        start = max - 1000;
      }
      else
        start = from;
    }
    if (to !== null && to !== undefined && to !== '') {
      end = to;
    }
    Drupal.APSJobsSearch.sliderSalary(min, max, step, pipStep, start, end);
  }

  Drupal.APSJobsSearch.checkAll = function() {
    $('.job-level-all').each(function() {
      //check parent then check child
      var boxAll = $(this);
      boxAll.on('change', function() {
        $(this).closest('ul').find(':checkbox').prop('checked', this.checked);
        Drupal.APSJobsSearch.selectedClassification();
        Drupal.APSJobsSearch.selectedJobLevel();
      });

      //check child check parent
      boxAll.closest('ul').find(':checkbox').each(function() {
        $(this).change(function() {
          var isCheckAll = (boxAll.closest('ul').find(':checkbox').not(boxAll).length == boxAll.closest('ul').find('input:checked').not(boxAll).length);
          boxAll.prop('checked', isCheckAll);
          Drupal.APSJobsSearch.selectedClassification();
          Drupal.APSJobsSearch.selectedJobLevel();
        });
      });
    });
  }

})(jQuery, Drupal);
