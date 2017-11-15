<div id="modalSaveSearch" class="modal fade" role="dialog">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal">&times;</button>
        <h4 class="modal-title">Create Alert</h4>
      </div>
      <div class="modal-body">
        <?php
        $content = str_replace("Save search", "Save", $content);
        print $content;
        ?>
      </div>
    </div>
  </div>
</div>
