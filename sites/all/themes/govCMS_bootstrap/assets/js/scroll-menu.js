(function($) {
  Drupal.Probody = Drupal.Probody || {};
  Drupal.Probody.currentRegion = null;
  Drupal.Probody.regions = [];
  Drupal.Probody.clickScrolling = false;
  Drupal.Probody.menuItems = [];

  Drupal.behaviors.scrollMenuAction = {
    attach: function (context) {


$(window).load(function() {
        if ($('body.front.not-logged-in').length) {
          $('#header #block-system-main-menu ul.menu > li > a').each(function() {
            $(this).smoothScroll();
            var menu_item = $(this);
            var hr = $(this).attr('href');
            var parts = hr.split('#');
            var id = parts[1];
            if (menu_item.length) {
              Drupal.Probody.menuItems.push(menu_item);
              menu_item.attr('data-id', id);
              menu_item.attr('data-link-type', 'scroll');
              // menu_item.attr('data-hash', Drupal.settings.basePath + "#" + id);
              // menu_item.attr('href', Drupal.settings.basePath + "#" + id);
            }
          });
          Drupal.Probody.mobileMenuScroll();
          Drupal.Probody.calculateScroll();
          Drupal.Probody.checkActiveItem();
          $(window).scroll(function() {
            Drupal.Probody.checkActiveItem();
          });
        }
      });
$('#header #block-system-main-menu ul.menu > li > a').click(function (){
  if ($('body.front.not-logged-in').length) {
  Drupal.Probody.checkActiveItem();
  }
});
      $(window).resize(function() {
        if ($('body.front.not-logged-in').length) {
        Drupal.Probody.calculateScroll();
      }
      });
    }
  };

  Drupal.Probody.checkActiveItem = function () {
    var top = $(window).scrollTop();
    var bottom = top + $(window).height();
    var selected_item = -1;
    //var current_delta = 0;
    var pre_Section = Drupal.Probody.menuItems[0].attr('data-bottom')/3;
    for(i = 0; i < Drupal.Probody.menuItems.length - 1; i ++) {
      var menu_item = Drupal.Probody.menuItems[i];
      var i_top = $(menu_item).attr('data-top');
      var i_bottom = $(menu_item).attr('data-bottom');
      //current_delta = Math.abs(top - i_bottom);
      if((top + pre_Section > i_top) && (top + pre_Section  < i_bottom)) {
        selected_item = i;
      }
    }
    if (selected_item !== Drupal.Probody.currentRegion) {
      for (j = 0; j < Drupal.Probody.menuItems.length -1; j ++) {
        if (j !== selected_item) {
          $(Drupal.Probody.menuItems[j]).removeClass('active');
          
        }
      }
      $(Drupal.Probody.menuItems[selected_item]).addClass('active');
    }    
  }
  Drupal.Probody.mobileMenuScroll = function(){
    $('#menu-toggle').click(function (){
      Drupal.Probody.calculateScroll();
          Drupal.Probody.checkActiveItem();
    });
  }
  Drupal.Probody.calculateScroll = function () {
    for(var x in Drupal.Probody.menuItems) {
      var menu_item = Drupal.Probody.menuItems[x];
      var id = $(menu_item).attr('data-id');
      var element = $("#" + id);
      if(element.length) {
        var offset = $(element).offset();
        $(menu_item).attr('data-top', offset.top);
        var next_x = parseInt(x) + 1;
        
        if(Drupal.Probody.menuItems.length - 1 > next_x) {
            var next_item = Drupal.Probody.menuItems[next_x];
            var next_id = $(next_item).attr('data-id');
            var next_element = $("#" + next_id);
            if(next_element.length) {
                $(menu_item).attr('data-bottom', $(next_element).offset().top);
            }
        }
        if(x == 2) {
            $(menu_item).attr('data-bottom', $('body').height());
        }
      }
      Drupal.Probody.currentActiveMenu = id;
    }
  };
})(jQuery);
