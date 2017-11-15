(function($, Drupal) {
  Drupal.apsjobsJobApproval = Drupal.apsjobsJobApproval || {};
  Drupal.behaviors.actionapsjobsJobApproval = {
    attach: function(context) {
      Drupal.apsjobsJobApproval.cancelRejectPopup();
      $(window).load(function() {
       Drupal.apsjobsJobApproval.showDialogUnpublish();
      });
    }
  }

  Drupal.apsjobsJobApproval.cancelRejectPopup = function() {
    if ($('body').hasClass('node-type-aps-vacancies')) {
      $('#edit-popup button').click(function() {
        $('#edit-popup textarea').val('');
      });
    }
  }

  Drupal.apsjobsJobApproval.showDialogUnpublish = function() {
    $('.views-field-nothing .unpublish a.toc-filter-processed').on('click', function(e) {
      var nid = $(this).attr('data-nid');
      console.log('page nid=', nid);
      $('#unpublishJobModal').modal('show');
      $('.modal-footer span.btn').attr('data-nid', nid);
      console.log($('.modal-footer span.btn').attr('data-nid'));
      return false;
    });
    $('.page-node .unpublish a.toc-filter-processed').on('click', function(e) {
      var nid = $(this).attr('href').substr(1);
      $('#unpublishJobModal').modal('show');
      $('.modal-footer span.btn').attr('data-nid', nid);
      return false;
    });

    $('.modal-header span.close').on('click', function() {
      $('#unpublishJobModal').modal('hide');
    });

    $('.modal-footer span.btn').on('click', function() {
      var vid = $(this).attr('data-nid');
      console.log('vid', vid);
      var path = '/vacancy/unpublish'
      $.ajax({
        type: 'POST',
        url: path,
        data: {nid: vid},
        async: false,
        success: function(data) {
          if (data == 'true') {
            $('#unpublishJobModal').modal('hide');
            $(this).removeAttr('data-nid');
            location.reload();
          }
        },
      });
    });
  }
})(jQuery, Drupal);
