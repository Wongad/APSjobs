<?php
/**
 * @file
 * Define entity class, entity action (like add, save, delete, view,... callback).
 */

/**
 * Project entity class extending the Entity class
 */
class APSEmailSubscriptionsEntity extends Entity {
  /**
   * Change the default URI from default/id to project/id
   */
  protected function defaultUri() {
    return array('path' => 'aps_email_subscriptions/' . $this->identifier());
  }
}

