<div>
	<div><a class="btn-a" href="#">+ New Alert</a></div>
	<div>
		<?php foreach ($items as $item): ?>
		<div class="views-row">
			<div class="group-info">
				<div class="views-field views-field-title">
					<span class="field-content"><?php print($item['title']); ?><span>
				</div>
				<div class="views-field views-field-type">
					<span class="field-content"><?php print($item['type']); ?><span>
				</div>
			</div>
			<div class="btn-group">
				<a href="#">Edit Alert Preferences</a>
				<a href="#">Search Now</a>
			</div>
		</div>
		<?php endforeach; ?>
	</div>
	<div class="btn-show"><a class="views-more" href="#">View More Alerts</a></div>
</div>