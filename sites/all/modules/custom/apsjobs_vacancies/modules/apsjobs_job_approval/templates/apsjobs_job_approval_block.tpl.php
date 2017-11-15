<?php
  $display_result_date = ($state_date != 'none') ? 'display: inline-block' : 'display: none';
  $display_result_detail = ($state_detail != 'none') ? 'display: inline-block' : 'display: none';
  $display_approval_date = ($state_date != 'none') ? 'display: none' : 'display: inline-block';
  $display_approval_detail = ($state_detail != 'none') ? 'display: none' : 'display: inline-block';
  $class_date = '';
  if ($state_date == 'Rejected') $class_date = 'rejected';
  if ($state_date == 'Approved') $class_date = 'approved';
  $class_detail = '';
  if ($state_detail == 'Rejected') $class_detail = 'rejected';
  if ($state_detail == 'Approved') $class_detail = 'approved';
?>

<div class="approval-block-wrapper">
  <div class="approval-date" data-state="<?php echo $state_date?>">
    <span class="approval-label">Publication date approval:</span>
    <span class="approval-group">

      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/reject-date';?>" title="" class="approval-reject" rel="nofollow" style="<?php echo $display_approval_date;?>">Reject</span>

      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/approve-date';?>" title="" class="approval-approve" rel="nofollow" style="<?php echo $display_approval_date;?>">Approve</span>

    </span>
    <span class="reset-group">

      <span class="approval-result <?php echo $class_date;?>" style="<?php echo $display_result_date;?>"><?php echo $state_date;?></span>

      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/reset-date'; ?>" title="" class="approval-reset" rel="nofollow" style="<?php echo $display_result_date;?>">Reset</span>

    </span>
  </div>
  <div class="approval-detail" data-state="<?php echo $state_detail?>">
    <span class="approval-label">Job detail approval:</span>
    <span class="approval-group">
      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/reject-detail';?>" title="" class="approval-reject" rel="nofollow" style="<?php echo $display_approval_detail;?>">Reject</span>
      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/approve-detail';?>" title="" class="approval-approve" rel="nofollow" style="<?php echo $display_approval_detail;?>">Approve</span>
    </span>
    <span class="reset-group">
      <span class="approval-result <?php echo $class_detail;?>" style="<?php echo $display_result_detail;?>"><?php echo $state_detail;?></span>
      <span data-path="<?php echo '/approve-vacancy/' . $nid . '/reset-detail';?>" title="" class="approval-reset" rel="nofollow" style="<?php echo $display_result_detail;?>">Reset</span>
    </span>
  </div>
</div>
