(function($, Drupal) {
	Drupal.apsJobAlerts = Drupal.apsJobAlerts || {};
	Drupal.behaviors.actionapsJobAlerts = {
		attach: function(context) {
      Drupal.apsJobAlerts.alertActionButton();
      Drupal.apsJobAlerts.alertCheckAll();
      Drupal.apsJobAlerts.resetModalCreateAlertClosed();
			$(window).load(function() {

			});
		}
	}

  Drupal.apsJobAlerts.alertCheckAll = function() {
    $('#apsjobs-alerts-all').change(function(){
      $('tbody :checkbox').prop('checked', $(this).is(':checked'));
    });
    $('tbody :checkbox').each(function() {
      $(this).change(function() {
        $('#apsjobs-alerts-all').prop('checked', $('tbody input:checked').length == $('tbody :checkbox').length);
      });
    });
  }

  Drupal.apsJobAlerts.alertActionButton = function() {
    if ($('body').hasClass('page-manage-alert')) {
      var actionButtons = $('.alert-action');
      actionButtons.each(function() {
        var button = $(this);
        button.on('click', function() {
          var action = button.attr('data-action');
          var path = '/manage-alert/' + action;
          var ids = [];
          if (action == 'cancel-all' || action == 'delete-all') {
            $('tbody input:checked').each(function() {
              ids.push($(this).val());
            });
          } else {
            ids.push(button.attr('data-id'));
          }
          $('#modal-alert-action .modal-body').hide();
          $('#modal-alert-action .modal-footer').show();

          $.ajax({
            type: 'GET',
            url: path,
            data: {ids : ids},
            asyn: false,
            success: function(data) {
              if (data == 'reload'){
                window.location.reload();
              }
              else {
                $('#modal-alert-action .modal-body').html(data);
                $('#modal-alert-action .modal-body').show();
                $('#modal-alert-action .modal-footer').hide();
                Drupal.apsJobAlerts.validateEditAlert()
              }
            }
          });
        });
      });
    }
  }

  Drupal.apsJobAlerts.validateEditAlert = function() {
    var $formRequest = $('form#apsjobs-alerts-edit-alert-form');
    $formRequest.submit(function(event) {
      var validation = true;
      var editInput = $formRequest.find('#edit-name');
      if (editInput.val() == '') {
        validation = false;
        if (!editInput.hasClass('error')){
          editInput.addClass('error');
          editInput.parent().append('<span class="error-message">Choose a Title for your Alert field is required.</span>');
        }
      }
      if (!validation)
        return false;
    });
  }

  Drupal.apsJobAlerts.resetModalCreateAlertClosed = function() {
    if ($('body').hasClass('page-search-jobs')) {
      $('#modalSaveSearch').on('hidden.bs.modal', function () {
        $('#modalSaveSearch .modal-body #edit-name').val('');
        $('#modalSaveSearch .modal-body #edit-mail').val('');
        $('#modalSaveSearch .modal-body #edit-notify-interval').val('');
      });
    }
  }

})(jQuery, Drupal);
