(function ($, Drupal) {

    Drupal.apsjobs = Drupal.apsjobs || {};
    Drupal.apsjobs.breakPoint = 992;
    Drupal.behaviors.actionaimpoc = {
      attach: function (context) {
        Drupal.apsjobs.backToTop();
        Drupal.apsjobs.moveMainMenu();
        Drupal.apsjobs.addPlaceholderAgencyDetail();
        Drupal.apsjobs.showCheckboxOutcomeDetail();
        $(window).load(function () {
          $('#content-fifth-wrapper .pane-content .view-less').hide();
          if(!$('body').hasClass('not-logged-in')) {
            $('#block-system-user-menu ul li:nth-child(2)').hide();
          }
          // Drupal.apsjobs.clickViewLessHome();
          Drupal.apsjobs.scrollClass();
          $(window).scroll(Drupal.apsjobs.scrollClass);
          Drupal.apsjobs.showMoreGazette();
          Drupal.apsjobs.menuClickSub();
          Drupal.apsjobs.sortDateAgency();
          Drupal.apsjobs.showMoreJobApproval();
          Drupal.apsjobs.showMoreAwaitingApproval();
          Drupal.apsjobs.showMoreDraft();
          Drupal.apsjobs.showMoreArchived();
          Drupal.apsjobs.showMoreOutcome();
          Drupal.apsjobs.showFullUserName();
          Drupal.apsjobs.showMoreOutcomeDetail();
          Drupal.apsjobs.showMoreOutComeDrafts();
          Drupal.apsjobs.searchFormVacancy();
          Drupal.apsjobs.btnSearchForm();
          Drupal.apsjobs.EnterSearch();
          Drupal.apsjobs.changeImageWithText();
          Drupal.apsjobs.showMoreJobsofAgency();
          if ($('body').hasClass('page-search-jobs'))
            Drupal.apsjobs.moveSearchButton();
          $('.page-aps-work .ui-accordion .pane-quicktabs-job-level h2').trigger('click');
        });

        $( document ).ajaxComplete(function(event, xhr, settings) {
          var $class = 'pager-load-more-empty';
          if(xhr.responseText.indexOf($class) !== -1){
            $('#content-fifth-wrapper .pane-content .view-less').show();
          }
          Drupal.apsjobs.showMoreJobApproval();
          Drupal.apsjobs.showMoreAwaitingApproval();
          Drupal.apsjobs.showMoreDraft();
          Drupal.apsjobs.showMoreArchived();
          Drupal.apsjobs.showMoreOutcome();
          Drupal.apsjobs.showFullUserName();
          Drupal.apsjobs.showMoreOutcomeDetail();
          Drupal.apsjobs.showMoreOutComeDrafts();
          Drupal.apsjobs.showMoreJobsofAgency();
        });
      }
    };

    Drupal.apsjobs.showCheckboxOutcomeDetail = function () {
      var isIsac = $('.node-type-aps-outcomes .views-field-field-is-isac .field-content').text();
      var isRecruitability = $('.node-type-aps-outcomes .views-field-field-is-recruitability .field-content').text();
      $('.node-type-aps-outcomes .views-field-field-is-isac .field-content').remove();
      $('.node-type-aps-outcomes .views-field-field-is-recruitability .field-content').remove();
      if (isIsac == '1') {
        $('.node-type-aps-outcomes .views-field-field-is-isac').append('<input type="checkbox" checked><label>checkbox</label>');
      }
      else {
        $('.node-type-aps-outcomes .views-field-field-is-isac').append('<input type="checkbox"><label>checkbox</label>');
      }
      if (isRecruitability == '1') {
        $('.node-type-aps-outcomes .views-field-field-is-recruitability').append('<input type="checkbox" checked><label>checkbox</label>');
      }
      else {
        $('.node-type-aps-outcomes .views-field-field-is-recruitability').append('<input type="checkbox"><label>checkbox</label>');
      }
    }

    Drupal.apsjobs.showFullUserName = function () {
      var userName = $('.show-full-user').text();
      $('.show-full-user').attr('Title','' + userName + '');
    }

    Drupal.apsjobs.addPlaceholderAgencyDetail = function() {
      $('#views-exposed-form-aps-vacancy-panel-pane-8 .form-item-title .form-text').attr("placeholder", "Type here to search");
    }

    Drupal.apsjobs.menuClickSub = function(){
      $('.block-menu-jop-agency-name h2').on('click',function(){
        $('.block-menu-jop-agency-name ul.menu').toggleClass('show-expand');
      });
      $(document).click(function(){
        $(".block-menu-jop-agency-name ul.menu").removeClass('show-expand');
      });
      $(".block-menu-jop-agency-name h2").click(function(e){
        e.stopPropagation();
      });
    }

    Drupal.apsjobs.backToTop = function () {
      $('.btn-btt').smoothScroll({speed: 600});
      $(window).scroll(function() {
          if($(window).scrollTop() > 200) {
            $('.btn-btt').show();
          }
          else {
            $('.btn-btt').hide();
          }
        });
    }

    Drupal.apsjobs.scrollClass = function () {
        if ($('body').length) {
            var scroll = $(window).scrollTop();
            if (scroll >= 1) {
                $("#page").addClass("scrolled");
            } else {
                $("#page").removeClass("scrolled");
            }
        }
    }

    Drupal.apsjobs.showMoreGazette = function(){
      $('.page-aps-gazette .view-view-gazette .views-field-body').addClass('hidden');

          $('.page-aps-gazette .view-view-gazette .views-field-nothing p').click(function(){
            if($(this).hasClass('show-more')){
              $(this).text('Show Less');
              $(this).removeClass('show-more');
              $(this).closest('.views-row').find('.views-field-body').removeClass('hidden');
            }
            else {
              $(this).text('Show More');
              $(this).addClass('show-more');
              $(this).closest('.views-row').find('.views-field-body').addClass('hidden');
            }

        });

    }

    Drupal.apsjobs.showMoreJobApproval = function(){
      $('.page-aps-agency-jobs-approval .view-aps-vacancy .agency-list-approval').addClass('hidden');

      $('.page-aps-agency-jobs-approval .view-aps-vacancy .views-field-nothing p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreAwaitingApproval = function(){
      $('.page-aps-agency-my-job-vacancies .view-aps-vacancy .agency-list-approval').addClass('hidden');

      $('.page-aps-agency-my-job-vacancies .view-aps-vacancy .views-field-nothing-2 p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreDraft = function(){
      $('.page-aps-agency-my-job-vacancies-drafts .view-aps-vacancy .agency-list-draft').addClass('hidden');

      $('.page-aps-agency-my-job-vacancies-drafts .view-aps-vacancy .views-field-nothing-1 p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-draft').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-draft').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreArchived = function(){
      $('.page-aps-agency-my-job-vacancies-archive .view-aps-vacancy .agency-list-archived').addClass('hidden');

      $('.page-aps-agency-my-job-vacancies-archive .view-aps-vacancy .views-field-nothing-1 p').click(function(){
        if($(this).hasClass('show-more')){
          console.log('111eqdada');
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-archived').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-archived').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreOutComeDrafts = function(){
      $('.page-aps-agency-my-outcomes .view-aps-outcomes .outcome-list-item').addClass('hidden');

      $('.page-aps-agency-my-outcomes .view-aps-outcomes .views-field-nothing p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.outcome-list-item').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.outcome-list-item').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreOutcome = function(){
      $('.page-outcomes-create-outcome .view-aps-vacancy .agency-list-approval').addClass('hidden');

      $('.page-outcomes-create-outcome .view-aps-vacancy .views-field-nothing-2 p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreOutcomeDetail = function(){
      $('.node-type-aps-outcomes .view-aps-vacancy .agency-list-approval').addClass('hidden');

      $('.node-type-aps-outcomes .view-aps-vacancy .views-field-nothing-2 p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.showMoreJobsofAgency = function(){
      $('.node-type-aps-agencies .view-aps-vacancy .agency-list-approval').addClass('hidden');

      $('.node-type-aps-agencies .view-aps-vacancy .views-field-nothing-1 p').click(function(){
        if($(this).hasClass('show-more')){
          $(this).text('Show Less');
          $(this).removeClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').removeClass('hidden');
        }else{
          $(this).text('Show More');
          $(this).addClass('show-more');
          $(this).closest('.views-row').find('.agency-list-approval').addClass('hidden');
        }
      });
    }

    Drupal.apsjobs.setInputPlaceHolder = function (name, text, selector) {
        selector = selector == undefined ? '' : selector + ' ';
        if ($.support.placeholder) {
            $(selector + 'input[name="' + name + '"]').attr('placeholder', Drupal.t(text));
        } else {
            $(selector + 'input[name="' + name + '"]').val(Drupal.t(text));
            $(selector + 'input[name="' + name + '"]').focus(function () {
                if (this.value == Drupal.t(text)) {
                    this.value = '';
                }
            }).blur(function () {
                if (this.value == '') {
                    this.value = Drupal.t(text);
                }
            });
        }
    };

    Drupal.apsjobs.sortDateAgency = function(){
      $('.sort-date #edit-sort-date').on('change',function(){
        var self = $(this);
        var selectedVal = self.val();
        $(".views-widget-sort-order #edit-sort-order").val(selectedVal);
        var test = $(".views-widget-sort-order #edit-sort-order").val();
        $('.views-submit-button .form-submit').trigger('click');
      });
    }

    Drupal.apsjobs.moveMainMenu = function() {
      if ($(window).width() <= 1199) {
        if ($('#block-menu-block-govcms-menu-block-footer').length){
          $('#block-menu-block-govcms-menu-block-footer').appendTo($('#header .container'));
          $('#block-menu-block-govcms-menu-block-footer').addClass('menu-moved');
        }
        if ($('#block-menu-menu-agency-home-page').length){
          $('#block-menu-menu-agency-home-page').appendTo($('#header .container'));
          $('#block-menu-menu-agency-home-page').addClass('menu-moved');
        }
      }
      $(window).resize(function() {
        if ($(window).width() <= 1199) {
          if ($('#block-menu-block-govcms-menu-block-footer').length) {
            $('#block-menu-block-govcms-menu-block-footer').appendTo($('#header .container'));
            $('#block-menu-block-govcms-menu-block-footer').addClass('menu-moved');
          }
          if ($('#block-menu-menu-agency-home-page').length) {
            $('#block-menu-menu-agency-home-page').appendTo($('#header .container'));
            $('#block-menu-menu-agency-home-page').addClass('menu-moved');
          }
        } else {
          if ($('#block-menu-block-govcms-menu-block-footer').length)
            $('#block-menu-block-govcms-menu-block-footer').prependTo($('#bs-example-navbar-collapse-1 .region-main-menu'));
          if ($('#block-menu-menu-agency-home-page').length)
            $('#block-menu-menu-agency-home-page').prependTo($('#bs-example-navbar-collapse-1 .region-main-menu'));
        }
      });
    }

    Drupal.apsjobs.moveSearchButton = function() {
      if ($(window).width() <= 767) {
        if ($('.form-item-find-button').length)
          $('.form-item-find-button').addClass('moved');
          $('.form-item-find-button').appendTo($('form.form-aps-job-secrh'));
      }
      $(window).resize(function() {
        if ($(window).width() <= 767) {
          if ($('.form-item-find-button').length)
            $('.form-item-find-button').addClass('moved');
            $('.form-item-find-button').appendTo($('form.form-aps-job-secrh'));
        } else {
          if ($('.form-item-find-button').length)
            $('.form-item-find-button').addClass('moved');
            $('.form-item-find-button').appendTo($('.form-search-jobs'));
        }
      });
    }

    Drupal.apsjobs.searchFormVacancy = function(){
      $('.form-search-vacancy input').keyup(function(){
        var textSearch = $(this).val();
        $('.views-widget .form-item-title input').val(textSearch);
        $('.views-widget .form-item-field-position-number-value input').val(textSearch);
        $('.views-widget .form-item-field-job-description-value input').val(textSearch);
      });
    }

    Drupal.apsjobs.btnSearchForm = function(){
      $('.btn-submit .apply-search').on('click',function(){
        $('.views-exposed-widget.views-submit-button input').trigger('click');
      });
    }

    Drupal.apsjobs.EnterSearch = function(){
      $('.form-search-vacancy input').on('keypress',function(e) {
        if(e.keyCode == 13) {
          $('.views-exposed-widget.views-submit-button input').trigger('click');
        }
      });
    }

    Drupal.apsjobs.changeImageWithText = function() {
      if ($('.owl-carousel-panel_pane_123').length) {
        var $body = $('.owl-carousel-panel_pane_123 .views-field-body .field-content .panel-title-home');
        //orderNum
        var itemLength = $body.find('h2 span').length;
        if (localStorage.getItem('apsHomeSliderLast') !== null) {
          var lastItem = localStorage.getItem('apsHomeSliderLast');
          var nextItem = (parseInt(lastItem) + 1) % itemLength;
          if (localStorage.getItem('apsHomeSliderLast')) {
            localStorage.setItem("apsHomeSliderLast", nextItem);
            $body.find('h2 span.order-' + nextItem).addClass('active');
            $('.owlcarousel-fields-250').trigger('owl.goTo', nextItem);
          }
        }
        //
        $body.find('h2 span.highlight').each(function() {
          var self = $(this);

          self.on('mouseover click', function() {
            $(this).parent().find('span.highlight').removeClass('active');
            $(this).addClass('active');
            var order = $(this).attr('class');
            order = order.split(" ")[1];
            var orderNum = order.split("-")[1];
            localStorage.setItem("apsHomeSliderLast", orderNum);
            $('.owlcarousel-fields-250').trigger('owl.jumpTo', orderNum);
          });

        })
      }
    }
})(jQuery, Drupal);
