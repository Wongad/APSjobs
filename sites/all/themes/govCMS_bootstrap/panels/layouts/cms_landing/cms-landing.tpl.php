<div class="panel-display boxton clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <?php if (!empty($content['content_banner'])): ?>
  <section id="banner" class="banner">
    <div class="container-fluid">
      <?php print $content['content_banner']; ?>
    </div>
  </section>
  <?php endif; ?>

  <?php if (!empty($content['sub_nav'])): ?>
   <section id="subnav">
     <div class="container-fluid">
       <?php print $content['sub_nav']; ?>
     </div>
   </section>
  <?php endif; ?>

  <?php if (!empty($content['content_feature'])): ?>
   <section id="content-feature-wrapper" class="content-feature">
     <div class="container-fluid">
       <?php print $content['content_feature']; ?>
     </div>
   </section>
  <?php endif; ?>

  <?php if (!empty($content['content_main'])): ?>
    <section id="content-main-wrapper" class="content-main">
      <div class="container-fluid">
        <div class="row">
            <div class="main-inner ">
              <?php print $content['content_main']; ?>
            </div>
        </div>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_first'])): ?>
    <section id="content-first-wrapper" class="content-first">
      <div class="container">
        <?php print $content['content_first']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_second'])): ?>
    <section id="content-second-wrapper" class="content-second">
      <div class="container">
        <?php print $content['content_second']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_second_sub'])): ?>
    <section id="content-second-sub-wrapper" class="content-second-sub">
      <div class="container">
        <?php print $content['content_second_sub']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_third'])): ?>
    <section id="content-third-wrapper" class="content-third">
      <div class="container">
        <?php print $content['content_third']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_fourth'])): ?>
    <section id="content-fourth-wrapper" class="panel-section content-fourth">
      <div class="container">
        <?php print $content['content_fourth']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_fifth'])): ?>
    <section id="content-fifth-wrapper" class="panel-section content-fifth">
      <div class="container">
        <?php print $content['content_fifth']; ?>
      </div>
    </section>
  <?php endif; ?>

  <?php if (!empty($content['content_sixth'])): ?>
    <section id="content-sixth-wrapper" class="panel-section content-sixth">
      <div class="container-fluid">
        <?php print $content['content_sixth']; ?>
      </div>
    </section>
  <?php endif; ?>


</div>
