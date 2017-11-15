(function($, Drupal) {

  Drupal.aimpocSearch = Drupal.Search || {};
  Drupal.behaviors.actionaimpocSearch = {
    attach: function(context) {
      Drupal.aimpocSearch.liveChat();
      Drupal.aimpocSearch.activeAccordion();
      //Drupal.aimpocSearch.facetSearch();
    }
  };

  Drupal.aimpocSearch.activeAccordion = function() {
      // $(".page-search-course .views-exposed-widget label").closest('')
      // $('.accordion-facet .pane-title', context).once('pane-title', function () {
      //   $(this).click(function () {
      //     $(this).toggleClass('open');
      //     $(this).next().find('ul').toggle('fast');
      //   })
      // });
      // var $firstAccordion = $('.accordion-facet .pane-title');
      // $firstAccordion.addClass('open');
      // $firstAccordion.next().find('ul').css({
      //   display: "block"
      // })
  };

  Drupal.aimpocSearch.facetSearch = function() {
    var counters = {};
    $('.facet-search-subject ul li a').each(function() {
      $(this).find("span").remove();
      var text = $(this).text();
      var parts = text.split("(", 2);
      text = parts[0].trim();
      var count = parts[1] ? parts[1] : "";
      parts = count.split(")", 2);
      count = parts[0];
      counters[text] = count;
    });
    $(".page-search-course #edit-subject-area-wrapper .form-type-bef-checkbox label").each(function() {
      var text = $(this).text().trim();
      if(counters[text] != undefined) {
        $(this).text($(this).text() + " (" + counters[text] + ")")
      }
      else {
        $(this).text($(this).text() + " (0)")
      }
    });
    counters = {};
    $('.facet-search-traning ul li a').each(function() {
      $(this).find("span").remove();
      var text = $(this).text();
      var parts = text.split("(", 2);
      text = parts[0].trim();
      var count = parts[1] ? parts[1] : "";
      parts = count.split(")", 2);
      count = parts[0];
      counters[text] = count;
    });
    $(".page-search-course #edit-training-level-wrapper .form-type-bef-checkbox label").each(function() {
      var text = $(this).text().trim();
      if(counters[text] != undefined) {
        $(this).text($(this).text() + " (" + counters[text] + ")")
      }
      else {
        $(this).text($(this).text() + " (0)")
      }
    });

  };

  Drupal.aimpocSearch.liveChat = function() {
    $(".livechat-btn-group a").click(function() {
      if($('.click-desk-visitor').length) {
        $('.click-desk-visitor').trigger('click');
        return false;
      }
    })
  };

})(jQuery, Drupal);
