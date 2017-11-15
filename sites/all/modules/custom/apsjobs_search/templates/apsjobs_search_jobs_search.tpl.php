<div class="edit-form-aps custom-advance-search show">
	<p class="fitler-togled"><span>Hide Filters</span></p>
	<form class="form-aps-job-secrh">
		<div class="form-search-jobs">
			<p class="find-job">Find Your Job</p>
			<div class="form-item form-type-textfield form-item-key-word">
			<label for="">Keyword Search<span class="form-required" title="">*</span></label>
			<input type="text" id="" name="" value="" size="30" maxlength="128" class="form-text required" placeholder="Department, Category...">
			</div>
			<div class="form-item form-type-textfield form-item-key-location">
				<label for="">Location<span class="form-required" title="">*</span></label>
				<input type="text" id="edit-key-location" name="edit-key-location" value="" size="30" maxlength="128" class="form-text required" placeholder="State, City, Suburb...">
			</div>

			<div class="form-item salary">
				<!-- <div class="form-item form-type-textfield form-item-key-salary show">
					<label for="">Salary<span class="form-required" title="">*</span></label>
					<select name="" id="edit_form_salary">
						<option value="All">Any Salary</option>
						<?php //foreach ($data['salary_range'] as $key => $value) : ?>
						<option value="<?php //echo $key; ?>"><?php //echo $value->name; ?></option>
						<?php //endforeach; ?>
					</select>
				</div> -->

				<div class="form-item form-type-textfield form-item-salary-range">
					<label for="amount">Salary</label>
					<div class="select-range"><span class="from-val"></span> to <span class="to-val"></span> per annum</div>
					<div id="slider-salary-range"></div>
					<div class="type-rate">
						<ul>
							<li class="active"><div class="salary-type" data-type="annually">Annual</div></li>
							<li><div class="salary-type" data-type="hourly">Hourly</div></li>
						</ul>
					</div>
				</div>

				<div class="form-item classification" style="display:none;">
					<label for="">Classification<span class="form-required" title="">*</span></label>
					<div class="dropdown advanced-search-job-level">
						<button class="btn dropdown-toggle item-classification" type="button" data-toggle="dropdown"><span>Any Classification</span>
						<span class="caret"></span></button>
						<ul class="dropdown-menu">
							<?php foreach ($data['job_level'] as $delta => $item) : ?>
								<?php
									$job_level = taxonomy_term_load($delta);
									if(sizeof($item)) {
								?>
								<div>
									<h3><?php echo $job_level->name; ?></h3>
									<ul>
										<li><input id="job-level-all-<?php print $delta;?>" class="job-level-all" type="checkbox" name="level" value="all"><label for="job-level-all-<?php print $delta;?>">All</label></li>
										<?php foreach ($item as $key => $value) : ?>
											<li><input id="<?php echo 'job-level' . $value->tid ;  ?>" type="checkbox" name="level" value='<?php echo $value->tid; ?>'><label for="<?php echo 'job-level' . $value->tid ;  ?>"><?php echo $value->name; ?></label></li>
										<?php endforeach; ?>
									</ul>
								</div>
							<?php } endforeach; ?>
						</ul>
					</div>
				</div>

				<span id="switch-select-slider">Classification</span>
			</div>
			<!---->
			<div class="form-item form-item-find-button">
				<div class="item-add-jobs form-submit">Find Jobs</div>
				<?php
        global $base_path;
        $img_loading = $base_path . drupal_get_path('theme', 'govCMS_bootstrap') . '/assets/images/loading.gif';
        ?>
        <img src="<?php echo $img_loading; ?>" height="15" width="15" style="display:none">
			</div>
			<p class="find-under"><a>FAQs</a></p>
		</div>

		<div class="advanced-search">
			<p class="togled"><span>Advanced</span></p>
			<div class="adv-search-wrapper">
				<div class="advanced">
					<div>
					<!---->
					<ul class="nav nav-tabs">
						<li class="dropdown advanced-search-job-level2">
							<span class="menu-label">Job Level</span>
							<a class="dropdown-toggle" data-toggle=""><span class="menu-label">Any</span><span class="plural">Job Level</span><span class="caret"></span></a>
							<div class="dropdown-block">
							<?php foreach ($data['job_level'] as $delta => $item) : ?>
								<?php
									$job_level = taxonomy_term_load($delta);
									if(sizeof($item)) {
								?>
								<div>
									<h3><?php echo $job_level->name; ?></h3>
									<ul>
										<li><input id="job-level-all2-<?php print $delta;?>" class="job-level-all" type="checkbox" name="level" value="all"><label for="job-level-all2-<?php print $delta;?>">All</label></li>
										<?php foreach ($item as $key => $value) : ?>
											<li><input id="<?php echo 'ajob-level' . $value->tid ;  ?>" type="checkbox" name="level2" value='<?php echo $value->tid; ?>'><label for="<?php echo 'ajob-level' . $value->tid ;  ?>"><?php echo $value->name; ?></label></li>
										<?php endforeach; ?>
									</ul>
								</div>
							<?php } endforeach; ?>
							</div>
						</li>
						<li class="dropdown advanced-search-job-category">
							<span class="menu-label">Occupation</span>
							<a class="dropdown-toggle" data-toggle=""><span class="menu-label">Any</span><span class="plural">Occupation</span><span class="caret"></span></a>
							<div class="dropdown-block">
								<div>
									<ul>
										<?php foreach ($data['occupation'] as $key => $value) : ?>
										<li><input id="<?php echo 'occupation' . $key ;  ?>" type="checkbox" name="level" value="<?php echo $key; ?>"><label for="<?php echo 'occupation' . $key ;  ?>"><?php echo $value->name ?></label></li>
										<?php endforeach; ?>
									</ul>
								</div>
							</div>
						</li>
						<li class="dropdown advanced-search-agency">
							<span class="menu-label">Agency</span>
							<a class="dropdown-toggle" data-toggle=""><span class="menu-label">Any</span><span class="plural">Agency</span><span class="caret"></span></a>
							<div class="dropdown-block">
								<div>
									<div class="form-filter">
										<input type="text" value="" placeholder="Enter Government Agency Name...">
										<span class="btn-clear">&times;</span>
									</div>
									<ul id="agency-list">
									<?php foreach ($data['agency'] as $key => $value) : ?>
										<li><input id="<?php echo 'agency' . $key ;  ?>" type="checkbox" name="level" value="<?php echo $key; ?>"><label for="<?php echo 'agency' . $key ;  ?>"><?php echo $value->name ; ?></label></li>
									<?php endforeach; ?>
									</ul>
								</div>
							</div>
						</li>
						<li class="dropdown advanced-search-job-type">
							<span class="menu-label">Job Type</span>
							<a class="dropdown-toggle" data-toggle=""><span class="menu-label">Any</span><span class="plural">Job Type</span><span class="caret"></span></a>
							<div class="dropdown-block">
								<div><a>Need more information?</a></div>
								<div class="working-hours">
									<h3>Working Hours</h3>
									<ul>
									<?php foreach ($data['working_hours'] as $key => $working) : ?>
										<li><input id="<?php echo 'working-hours' . $key ?>" type="checkbox" name="workinghours" value="<?php echo $key; ?>"><label for="<?php echo 'working-hours' . $key ?>"><?php echo $working->name; ?></label></li>
									<?php endforeach; ?>
									</ul>
								</div>

								<div class="engagement-type">
									<h3>Engagement Type</h3>
									<ul>
										<?php foreach ($data['engagement'] as $key => $engagement) : ?>
											<li><input id="<?php echo 'engagement' . $key ?>" type="checkbox" name="engagement" value="<?php echo $key ?>"><label for="<?php echo 'engagement' . $key ?>"><?php echo $engagement->name ?></label></li>
										<?php endforeach; ?>
									</ul>
								</div>

								<div class="specific-considerations">
									<h3>Specific Considerations</h3>
									<ul>
										<?php foreach ($data['considerations'] as $key => $considerations) : ?>
											<li><input id="<?php echo 'considerations' . $key ?>" type="checkbox" name="considerations" value="<?php echo $key ?>"><label for="<?php echo 'considerations' . $key ?>"><?php echo $considerations->name ?></label></li>
										<?php endforeach; ?>
									</ul>
								</div>

							</div>
						</li>
						<li class="dropdown advanced-search-clearance-level">
							<span class="menu-label">Clearance Level</span>
							<a class="dropdown-toggle" data-toggle=""><span class="menu-label">Any</span><span class="plural">Clearance Level</span><span class="caret"></span></a>
							<div class="dropdown-block">
								<div><a>Need more information?</a></div>
								<div>
									<ul>
										<?php foreach ($data['clearance'] as $key => $clearance) : ?>
											<li><input id="<?php echo 'clearance' . $key ?>" type="checkbox" name="clearance" value="<?php echo $key?>"><label for="<?php echo 'clearance' . $key ?>"><?php echo $clearance->name ?> </label></li>
										<?php endforeach; ?>
									</ul>
								</div>

							</div>
						</li>
					</ul>
					<!---->
					</div>
				</div>
			</div>
		</div>
	</form>

	<div class="ad-location">
		<p class="location-australia"><a>Locations in Australia</a></p>
	</div>
</div>
