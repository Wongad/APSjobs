<div class="dropdown-menu">
	<?php foreach($items as $tid => $item): ?>
		<div>
			<h3><?php print $item['name']; ?></h3>
			<?php if (!empty($item['childs'])): ?>
				<ul>
					<?php foreach($item['childs'] as $child): ?>
						<li><input type="checkbox" name="<?php print $child->vocabulary_machine_name; ?>" value="<?php print $child->tid; ?>"><?php print $child->name; ?></li>
					<?php endforeach; ?>
				</ul>
			<?php endif; ?>
		</div>
	<?php endforeach; ?>
</div>