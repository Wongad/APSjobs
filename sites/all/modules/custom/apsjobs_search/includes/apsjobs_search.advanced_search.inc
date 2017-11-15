<?php

  function apsjobs_blocks_render_terms($vocab_name) {
		$vocab_name = 'aps_job_levels';
		$voc = taxonomy_vocabulary_machine_name_load($vocab_name);
		$tree = taxonomy_get_tree($voc->vid, 0, 1);
		$terms = array();
		foreach ($tree as $term) {
			$results[$term->tid]['tid'] = $term->tid;
			$results[$term->tid]['name'] = $term->name;
			$results[$term->tid]['childs'] = taxonomy_get_children_all($term->tid, $voc->vid);
		}
		$content = array('items' => $results);
		$path_tpl = drupal_get_path('module', 'apsjobs_blocks') . '/templates/apsjobs_blocks_term_filter.tpl.php';
		$markup = theme_render_template($path_tpl, $content);

		return $markup;
	}

	function taxonomy_get_children_all($tid, $vid = 0, $key = 'tid'){
		$c = taxonomy_get_children($tid, $vid, $key);
		foreach ($c as $t => $d){
			$result[$t] = $d;
			$below = taxonomy_get_children_all($t, $vid, $key);
			if (!empty($below)) {
				foreach ($below as $nt => $nd){
					$result[$nt] = $nd;
				}
			}
		}
		return $result;
	}