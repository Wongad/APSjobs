(function($, Drupal) {
	Drupal.apsSavedSearches = Drupal.apsSavedSearches || {};
	Drupal.behaviors.actionapsSavedSearches = {
		attach: function(context) {
			Drupal.apsSavedSearches.deleteSavedSearch();
			$(window).load(function() {

			});
		}
	}

	Drupal.apsSavedSearches.deleteSavedSearch = function() {
		$("li.item-saved-search").click(function(){
			console.log($(this).attr("data-item-id"));
			var item_id = $(this).attr("data-item-id");
			var path = 'apsjobs-saved-searches/delete';
      var item = $(this);
			$.ajax({
        type: 'GET',
        url: path,
        data: {ssid: item_id},
        async: false,
        success: function(data) {
        	console.log(data);
          item.hide();
        }
      })
		});
	}
})(jQuery, Drupal);
