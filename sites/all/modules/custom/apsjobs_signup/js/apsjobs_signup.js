(function($, Drupal) {
	
	Drupal.APSJobsSignUp = Drupal.APSJobsSignUp || {};
	Drupal.behaviors.actionAPSJobsSignUp = {
		attach: function(context) {
			Drupal.APSJobsSignUp.cloneHRApprover();
      Drupal.APSJobsSignUp.checkConfirmPassword();
			Drupal.APSJobsSignUp.selectAgencyDomain();
			Drupal.APSJobsSignUp.validateApprover();
			Drupal.APSJobsSignUp.disableWhenSelect();
			$(window).load(function() {
      });
      $(document).ready(function() {
        Drupal.APSJobsSignUp.initAgencyForm();
			});
		}
	}
	
	Drupal.APSJobsSignUp.cloneHRApprover = function() {
		$('#edit-btn-group .link-add').on('click', function(e){
			var $amount = $('#apsjobs-signup-agency-account-detail-form div input[name=approver_amount]');
			var amount_approver = $amount.val();
			amount_approver++;
			$amount.val(amount_approver);
			var $agency_form = $('#edit-agency-detail').clone(true);
			//add button remove
			$('<span class="btn-rm-form">&times;</span>').insertAfter($agency_form.find('legend span'));
			$agency_form.find('legend span.btn-rm-form').on('click', function() {
				var self = $(this);
				self.closest('#edit-agency-detail').remove();
				amount_approver--;
				$amount.val(amount_approver);
			});
			//clear input's value
			$agency_form.find('input').val("");
			$agency_form.find('input').removeClass('error');
			$agency_form.find('div.form-item-email- span.error-message').hide();
			//clear disable
			$agency_form.find('.form-item-first-name- input').removeAttr('disabled');
			$agency_form.find('.form-item-surname- input').removeAttr('disabled');
			$agency_form.find('.form-item-employee-number- input').removeAttr('disabled');
			$agency_form.find('.form-item-email- input').removeAttr('disabled');
			$agency_form.find('.form-item-job-level- select').removeAttr('disabled');
			$agency_form.find('.form-item-job-level- select').removeAttr('disabled').trigger("chosen:updated");
			
			$agency_form.insertAfter('#apsjobs-signup-agency-account-detail-form div fieldset:last');
			e.preventDefault();
		})
	}

	Drupal.APSJobsSignUp.checkConfirmPassword = function() {
		if ($('#edit-password, #edit-confirm-password').length) {
			$('#edit-password, #edit-confirm-password').on('keyup', function () {
				if ($('#edit-password').val() == $('#edit-confirm-password').val()) {
					$('#edit-message').html('Matching').css('color', 'green');
				} else {
					$('#edit-message').html('Not Matching').css('color', 'red');
				}
			});
		}
	}

  Drupal.APSJobsSignUp.selectAgencyDomain = function() {
    $('#edit-gov-agency').on('change', function (e) {
      var valueSelected = this.value;
      var $itemEmail = $('#apsjobs-signup-form div div.form-item-email');
      if (valueSelected != ''){
        var domain = valueSelected.split(':')[1];
        var title = Drupal.t('HR department generic email address ending in ') + domain;
        var $spanRequired = $itemEmail.find('label span');
        $itemEmail.find('label').text(title);
        $itemEmail.find('label').append($spanRequired);
        $itemEmail.show();
      } else {
        $itemEmail.hide();
      }
    });
  }

  Drupal.APSJobsSignUp.initAgencyForm = function() {
    var $selectAgency = $('#edit-gov-agency');
    var optionSelected = $("option:selected", $selectAgency);
    var value = optionSelected.attr('value');
    if (value == '') {
      $('#apsjobs-signup-form div div.form-item-email').hide();
    }
	}

	Drupal.APSJobsSignUp.disableWhenSelect = function() {
		var $formRequest = $('form#apsjobs-signup-agency-account-detail-form');
		$formRequest.find('.form-item-nominate-approver- select').on('change', function() {
			var self = $(this);
			var selectedApprover = self.val();
			var selfParent = self.closest('.fieldset-wrapper');
			if (selectedApprover != '') {
				selfParent.find('.form-item-first-name- input').attr('disabled','disabled');
				selfParent.find('.form-item-surname- input').attr('disabled','disabled');
				selfParent.find('.form-item-employee-number- input').attr('disabled','disabled');
				selfParent.find('.form-item-email- input').attr('disabled','disabled');
				selfParent.find('.form-item-job-level- select').attr('disabled','disabled').trigger("chosen:updated");
			}
			else {
				selfParent.find('.form-item-first-name- input').removeAttr('disabled');
				selfParent.find('.form-item-surname- input').removeAttr('disabled');
				selfParent.find('.form-item-employee-number- input').removeAttr('disabled');
				selfParent.find('.form-item-email- input').removeAttr('disabled');
				selfParent.find('.form-item-job-level- select').removeAttr('disabled').trigger("chosen:updated");
			}
		})
	}
	
	Drupal.APSJobsSignUp.validateApprover = function() {
		var $formRequest = $('form#apsjobs-signup-agency-account-detail-form');
		$formRequest.submit(function(event) {
			var validation = true;
			$formRequest.find('.form-item-email- input').each(function() {
				var self = $(this);
				if (self.prop('disabled') == false || self.attr('disabled') != 'disabled') {
					if (self.val() == '') {
						validation = false;
						self.addClass('error');
						if (self.parent().find('span.error-message').length)
							self.parent().find('span.error-message').html("Email field is required");
						else 
							self.parent().append("<span class='error-message'>Email field is required</span>");
					} else if (!Drupal.APSJobsSignUp.validateApproverEmail(self.val())){
						validation = false;
						self.addClass('error');
						if (self.parent().find('span.error-message').length)
							self.parent().find('span.error-message').html("Email invalid");
						else 
							self.parent().append("<span class='error-message'>Email invalid</span>");
					} else {
						if (self.parent().find('span.error-message').length)
							self.removeClass('error');
							self.parent().find('span.error-message').hide();
					}
				}				
			});
			if (!validation)
				return false;
		});
	}

	Drupal.APSJobsSignUp.validateApproverEmail = function(email) {
		if ($('#apsjobs-signup-agency-account-detail-form div').length) {
			var domain = $('#apsjobs-signup-agency-account-detail-form div input[name=agency_domain]').val();
			var re = /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
			return (domain != '' && email.indexOf(domain) && re.test(email));
		}		
	}
	
})(jQuery, Drupal);;