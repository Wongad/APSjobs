<?php global $user ;  
if($user->uid != 0) { ?>
<div class="block-menu-agency-right">
   <div class="block-menu-view-public">
    <div class="content">
        <p><a class="btn-a" href="/">View Public Site (APS Jobs)</a></p>
     </div>
   </div>
   <div class="block-menu-jop-agency-name">

    <h2><a class="show-full-user"><?php global $user;
      print $user->name; ?>
    </a></h2>
    <div class="content">
      <ul class="menu">
        <li class="first last leaf"><a href="/aps-agency/dashboard" title="">Dashboard</a></li>
        <!-- <li class="first last leaf"><a href="/aps-agency/jobs-approval" title="">Jobs Approval List</a></li> -->
        <li class="first last leaf"><a href="/user/logout" title="">Log Out</a></li>
      </ul>  
    </div>
   </div>
  </div>

<?php } ?>