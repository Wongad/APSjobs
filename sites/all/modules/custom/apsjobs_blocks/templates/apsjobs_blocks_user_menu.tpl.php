<?php 
  global $user;
  $user_is_logged_in        = user_is_logged_in($user);
  $user_is_agency_user      = in_array('APS Agency User', $user->roles);
  $user_is_agency_approvers = in_array('APS Agency Approver', $user->roles);
  $user_is_admin            = in_array('administrator', $user->roles);
  $text                     = t('View agency site (APS-Agency)');
?>
<div class="block-menu-agency-right">
  <?php if ($user_is_logged_in && ($user_is_admin || $user_is_agency_user || $user_is_agency_approvers)) : ?>
    <div class="block-menu-view-public">
      <div class="content">
        <p><a href="/aps-agency/dashboard"><?php print $text; ?></a></p>
      </div>
    </div>
    
    <div class="block-menu-jop-agency-name">
    <h2><a class="show-full-user"><?php global $user;
      print $user->name; ?>
    </a></h2>
    <div class="content">
      <ul class="menu">
        <li class="first last leaf"><a href="/user" title="">Manage Account</a></li>
        <li class="first last leaf"><a href="/user/logout" title="">Log Out</a></li>
      </ul>  
    </div>
  </div>
  <?php elseif ($user_is_logged_in): ?>
  <?php print $block1;?>  
  <?php else: ?>
  <?php print $block2;?>
  <?php endif; ?>  
  
</div>