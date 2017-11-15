<?php global $user ;  
if($user->uid != 0) { ?>
	<div>
		<p class="welcome-back"><strong>Welcome Back <?php global $user;
	  							print ucfirst($user->name); ?></strong></p>
	</div>
	<div class="last-login">
		<p>Last login <?php print date("d/n/Y", $user->access);  ?></p>
	</div>
<?php } ?>