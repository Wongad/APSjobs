<?php

/**
 * @file
 * 
 */
class FeedsGoutteParserResult extends FeedsParserResult {
  
}

class FeedsGoutteParser extends FeedsParser {

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    // Setup mappings so they can be used in variable replacement.
    $mappings = $source->importer->processor->config['mappings'];
    $mappings = $this->filterMappings($mappings);

    // Set source config, if it's empty get config from importer.
    $this->source_config = $source->getConfigFor($this);

    // Allow config inheritance.
    if (empty($this->source_config)) {
      $this->source_config = $this->config;
    }

    $raw = trim($fetcher_result->getRaw());
    if (empty($raw)) {
      throw new Exception(t('Feeds Goutte parser: The document is empty.'));
    }

    if (!method_exists($fetcher_result, 'getCrawler')) {
      throw new Exception(t('Feeds Goutte parser: Fetcher must be Goutte fetcher.'));
    }

    $client = $fetcher_result->getClient();
    $crawler = $fetcher_result->getCrawler();

    $result = array();
    $last_num_items = 0;
    foreach ($this->source_config['sources'] as $mapping_source => $selector) {
      $type = $this->source_config['types'][$mapping_source];
      $attribute = $this->source_config['attributes'][$mapping_source];
      $raw = $this->source_config['raws'][$mapping_source];
      $same_value = $this->source_config['same_values'][$mapping_source];
      $follow = $this->source_config['follows'][$mapping_source];
      $resolve_url = $this->source_config['resolve_urls'][$mapping_source];
      
      if(empty($selector)) {
        $result[$mapping_source] = array_fill(0, $last_num_items - 1, '');
        continue;
      }
      
      // follow
      if (!empty($follow)) {
        $values = array();
        $links = $crawler->filter($follow)->links();
        $uri = $client->getRequest()->getUri();
        foreach ($links as $link) {
          $crawler = $client->click($link);
          $value = $this->filter($type, $selector, $crawler, $raw, $attribute, $last_num_items);
          $values[] = reset($value);
        }
        $crawler = $client->request('GET', $uri);
      }
      else {
        $values = $this->filter($type, $selector, $crawler, $raw, $attribute, $last_num_items);
      }

      // same value used to get last value of first item to following item.
      // useful in context value just appear one or out of selector scope
      if (!empty($same_value)) {
        $values = array_fill(0, $last_num_items - 1, $values[0]);
      }
      
      // resolve absolute url
      if (!empty($resolve_url)) {
        $uri = $client->getRequest()->getUri();
        foreach ($values as $id => $value) {
          $urlEngine = new GoutteUrl();
          $value = $urlEngine->url_to_absolute($uri, $value);
          $values[$id] = $urlEngine->normalizeURL($value);
        }
      }

      $result[$mapping_source] = $values;
      $last_num_items = count($values);
    }

    // mapping
    $result_items = array();
    foreach ($result as $mapping_source => $items) {
      foreach ($items as $delta => $item) {
        $result_items[$delta][$mapping_source] = $item;
      }
    }

    return new FeedsGoutteParserResult($result_items);
  }

  private function filter($type, $selector, $crawler, $raw = FALSE, $attribute = '', $num_items = 0) {
    // filter
    if ($type == 'css') {
      $filtered = $crawler->filter($selector);
    }
    else if ($type == 'xpath') {
      $filtered = $crawler->filterXPath($selector);
    }
    else if ($type == 'plain') {
      $values = array_fill(0, $num_items, $selector);
      return $values;
    }

    // get result
    if (!empty($raw)) {
      $values = $filtered->each(function($node, $i) {
                return $node->ownerDocument->saveXML($node);
              });
    }
    else if (!empty($attribute)) {
      $values = $filtered->each(function($node, $i) use ($attribute) {
                return trim($node->getAttribute($attribute));
              });
    }
    else {
      $values = $filtered->each(function($node, $i) {
                return trim($node->nodeValue);
              });
    }

    return $values;
  }

  public function sourceForm($source_config) {
    $form = array();
    // Allow for config inheritance.
    if (empty($source_config)) {
      $source_config = $this->config;
    }
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $form['goutteparser'] = array(
      '#type' => 'fieldset',
      '#title' => t('QueryPath Parser Settings'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['goutteparser']['context'] = array(
      '#type' => 'textfield',
      '#title' => t('Context'),
      '#required' => TRUE,
      '#description' => t('This is the base query, all other queries will run in this context.'),
      '#default_value' => isset($source_config['context']) ? $source_config['context'] : '',
      '#maxlength' => 1024,
    );
    $form['goutteparser']['default_type'] = array(
      '#type' => 'radios',
      '#title' => t('Default selector type'),
      '#options' => array('css' => 'CSS', 'xpath' => 'XPath', 'plain' => 'Plain text'),
      '#default_value' => isset($source_config['default_type']) ? $source_config['default_type'] : 'css',
    );
    $form['goutteparser']['sources'] = array(
      '#type' => 'fieldset',
      '#title' => t('Selector')
    );
    $form['goutteparser']['types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Selector type'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['goutteparser']['attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attirbutes'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['goutteparser']['raws'] = array(
      '#type' => 'fieldset',
      '#title' => t('Get raw values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['goutteparser']['same_values'] = array(
      '#type' => 'fieldset',
      '#title' => t('Use same values for all items'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['goutteparser']['follows'] = array(
      '#type' => 'fieldset',
      '#title' => t('Follows and applies'),
      '#description' => t('Get link use selector bellow.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['goutteparser']['resolve_urls'] = array(
      '#type' => 'fieldset',
      '#title' => t('Resolve absolute url.'),
      '#description' => t('Usefull for image field.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    foreach ($mappings as $mapping) {
      $form['goutteparser']['sources'][$mapping['source']] = array(
        '#type' => 'textfield',
        '#title' => $mapping['target'],
        '#description' => t('The CSS/Xpath selector for this field.'),
        '#default_value' => isset($source_config['sources'][$mapping['source']]) ? $source_config['sources'][$mapping['source']] : '',
        '#maxlength' => 1024,
        '#required' => $mapping['unique']
      );
      $form['goutteparser']['types'][$mapping['source']] = array(
        '#type' => 'radios',
        '#title' => $mapping['target'],
        '#options' => array('css' => 'CSS', 'xpath' => 'XPath', 'plain' => 'Plain text'),
        '#default_value' => isset($source_config['types'][$mapping['source']]) ? $source_config['types'][$mapping['source']] : $form['goutteparser']['default_type']['#default_value'],
      );
      $form['goutteparser']['attributes'][$mapping['source']] = array(
        '#type' => 'textfield',
        '#title' => $mapping['target'],
        '#description' => t('The attribute to return.'),
        '#default_value' => isset($source_config['attributes'][$mapping['source']]) ? $source_config['attributes'][$mapping['source']] : '',
        '#maxlength' => 1024,
      );
      $form['goutteparser']['raws'][$mapping['source']] = array(
        '#type' => 'checkbox',
        '#title' => $mapping['target'],
        '#default_value' => isset($source_config['raws'][$mapping['source']]) ? $source_config['raws'][$mapping['source']] : '',
      );
      $form['goutteparser']['same_values'][$mapping['source']] = array(
        '#type' => 'checkbox',
        '#title' => $mapping['target'],
        '#default_value' => isset($source_config['same_values'][$mapping['source']]) ? $source_config['same_values'][$mapping['source']] : '',
      );
      $form['goutteparser']['follows'][$mapping['source']] = array(
        '#type' => 'textfield',
        '#title' => $mapping['target'],
        '#description' => t('The CSS/Xpath selector for this field.'),
        '#default_value' => isset($source_config['follows'][$mapping['source']]) ? $source_config['follows'][$mapping['source']] : '',
        '#maxlength' => 1024
      );
      $form['goutteparser']['resolve_urls'][$mapping['source']] = array(
        '#type' => 'checkbox',
        '#title' => $mapping['target'],
        '#default_value' => isset($source_config['resolve_urls'][$mapping['source']]) ? $source_config['resolve_urls'][$mapping['source']] : '',
      );
    }

    return $form;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = $this->sourceForm($this->config);
    return $form['goutteparser'];
  }

  /**
   * Override parent::sourceFormValidate().
   *
   * If the values of this source are the same as the base config we set them to
   * blank to that the values will be inherited from the importer defaults.
   *
   * @param &$values
   *   The values from the form to validate, passed by reference.
   */
  public function sourceFormValidate(&$values) {
    $values = $values['goutteparser'];
    asort($values);
    asort($this->config);
    if ($values === $this->config) {
      $values = array();
      return;
    }
    $this->configFormValidate($values);
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function configFormValidate(&$values) {
    
  }

  /**
   * Override parent::sourceDefaults().
   */
  public function sourceDefaults() {
    return array();
  }

  /**
   * Define defaults.
   *
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'context' => '',
      'sources' => array(),
      'types' => array(),
      'attributes' => array(),
      'raws' => array(),
      'follows' => array(),
      'resolve_urls' => array(),
      'debug' => array(),
    );
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $next = 0;
    foreach ($mappings as $delta => $mapping) {
      $sources = explode(':', $mapping['source']);
      $current = $sources[1];
      $next = $current + 1;
    }

    return array(
      'goutteparser:' . $next => array(
        'name' => t('Goutte Expression ' . $next),
        'description' => t('Allows you to configure a CSS/XPath selector expression that will populate this field.'),
      ),
            ) + parent::getMappingSources();
  }

  /**
   * Filters mappings, returning the ones that belong to us.
   */
  private function filterMappings($mappings) {
    $our_mappings = array();
    foreach ($mappings as $mapping) {
      if (strpos($mapping['source'], 'goutteparser:') === 0) {
        $our_mappings[$mapping['source']] = $mapping['target'];
      }
    }
    return $our_mappings;
  }

}