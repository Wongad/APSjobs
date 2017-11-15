<?php

  global $base_url;
  $url = $base_url . '/validate-account?code=' . $code . '&username=' . urlencode($username);
?>
<p>Hi <?php echo $username;?>,</p>
<p>Welcome to APSJobs</p>
<p>Email: <?php echo $username;?>.</p>
<p>Please click this link <?php print $url ?> to validate your account.</p>