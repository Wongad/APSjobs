<?php
function apsjobs_vacancies_edit_access($current_nid) {
	global $user;

	$agency_user_access_states = array('draft', 'needs_review', 'rejected');
	$agency_user_access_role = 'APS Agency User';

	$agency_approver_access_states = array('needs_review');
	$agency_approver_access_role = 'APS Agency Approver';

	return (in_array($agency_user_access_role, $user->roles) && apsjobs_vacancies_check_state($current_nid, $agency_user_access_states))
	|| (in_array($agency_approver_access_role, $user->roles) && apsjobs_vacancies_check_state($current_nid, $agency_approver_access_states));
}

function apsjobs_vacancies_check_state($current_nid, $state) {
	$results = db_select('workbench_moderation_node_history', 'wmnh')
		->fields('wmnh')
		->condition('nid', $current_nid)
		->condition('is_current', 1)
		->condition('state', $state, 'IN')
		->execute()
		->fetchAll();
	return (count($results)) ? TRUE : FALSE;
}

function apsjobs_vacancies_check_permisison_edit_archived($current_nid) {
	global $user;
	$results = array();
	if (in_array('APS Agency Approver', $user->roles)) {
		$results = db_select('workbench_moderation_node_history', 'wmnh')
			->fields('wmnh')
			->condition('nid', $current_nid)
			->condition('is_current', 1)
			->condition('state', 'rejected', '=')
			->execute()
			->fetchAll();
	}
	return (count($results)) ? FALSE : TRUE;
}
