<div class="container">
  <h2>Hello <?php echo $mail; ?></h2>
  <p>Here are your alerts:</p>
  <table class="table table-hover">
    <thead>
      <tr>
        <th><input type="checkbox" id="apsjobs-alerts-all" value="all"><label for="apsjobs-alerts-all"></label></th>
        <?php foreach ($header as $title) : ?>
        <th><?php echo $title; ?></th>
        <?php endforeach; ?>
      </tr>
    </thead>
    <tbody>
      <?php foreach ($rows as $row) : ?>
      <tr>
        <td><input type="checkbox" id="apsjobs-alerts-<?php echo $row['id']; ?>" value="<?php echo $row['id']; ?>" class="form-checkbox"><label for="apsjobs-alerts-<?php echo $row['id']; ?>"></label></td>
        <?php foreach ($row['info'] as $col) : ?>
        <td><?php echo $col; ?></td>
        <?php endforeach; ?>
        <td>
          <?php foreach ($row['op'] as $op) :?>
          <?php
            $op_attr = "";
            if (isset($op['attributes'])) {
              foreach ($op['attributes'] as $key => $value) {
                $op_attr .= $key . '="' . $value . '" ';
              }
            }
          ?>
          <button type="button" class="btn <?php echo $op['class']; ?>" <?php echo $op_attr?>>
            <?php echo $op['title']; ?>
          </button>
          <?php endforeach; ?>
        </td>
      </tr>
      <?php endforeach; ?>
    </tbody>
  </table>
  <?php
    $cancel_all_attr = "";
    if (isset($cancel_all['attributes'])) {
      foreach ($cancel_all['attributes'] as $key => $value) {
        $cancel_all_attr .= $key . '="' . $value . '" ';
      }
    }
    $delete_all_attr = "";
    if (isset($delete_all['attributes'])) {
      foreach ($delete_all['attributes'] as $key => $value) {
        $delete_all_attr .= $key . '="' . $value . '" ';
      }
    }
  ?>
  <button type="button" class="btn <?php echo $cancel_all['class']; ?>" <?php echo $cancel_all_attr?>>
    <?php echo $cancel_all['title']; ?>
  </button>
  <button type="button" class="btn <?php echo $delete_all['class']; ?>" <?php echo $delete_all_attr?>>
    <?php echo $delete_all['title']; ?>
  </button>
</div>

<div id="modal-alert-action" class="modal fade" role="dialog">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-body">
        <div class="modal-title">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Manage Alert</h4>
        </div>
      </div>
      <div class="modal-footer">
        <?php
        global $base_path;
        $img_loading = $base_path . drupal_get_path('theme', 'govCMS_bootstrap') . '/assets/images/loading.gif';
        ?>
        <img src="<?php echo $img_loading; ?>" class="center-block">
      </div>
    </div>
  </div>
</div>
