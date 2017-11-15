(function ($, Drupal) {

    Drupal.apsjobs = Drupal.apsjobs || {};
    Drupal.behaviors.actionAPSFlags = {
      attach: function (context) {
        $(".flag-action").unbind("click").bind("click", function() {
          alert("adfasdfasdf");
          console.log($(this).closest(".panel-section").find('.unflag-action'));
          $(this).closest(".panel-section").find('.unflag-action').trigger('click');
        });
      }
    };    
})(jQuery, Drupal);
