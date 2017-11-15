<div class="view-content">
		<?php foreach ($items as $item): ?>
		<?php if ($item['type'] == 'awaiting_approval' && !empty($item['class'])): ?>
		<div class="view-row <?php print $item['class']; ?>">
		<?php else:?>
		<div class="view-row">
		<?php endif ?>
				<div class="views-field views-field-value">
					<span class="field-content"><?php print($item['value']); ?><span>
				</div>
				<div class="views-field views-field-title">
					<a href="<?php print($item['link']); ?>"><span class="field-content"><?php print($item['title']); ?></span></a>
				</div>
		</div>
		
		<?php endforeach; ?>
</div>
