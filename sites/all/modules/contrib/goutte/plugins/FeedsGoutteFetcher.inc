<?php

//$Id$;
/**
 * @file
 * 
 */
module_load_include('phar', 'goutte', 'goutte');

use Goutte\Client;

class GoutteClient extends Client {

  /**
   * Creates a crawler.
   *
   * This method returns null if the DomCrawler component is not available.
   *
   * @param string $uri     A uri
   * @param string $content Content for the crawler to use
   * @param string $type    Content type
   *
   * @return Crawler|null
   */
  protected function createCrawlerFromContent($uri, $content, $type) {
    if (!class_exists('Symfony\Component\DomCrawler\Crawler')) {
      return null;
    }

    $crawler = new Symfony\Component\DomCrawler\Crawler(null, $uri);

    // fix charset encoding 
    // @see: https://github.com/fabpot/Goutte/issues/59
    if (!preg_match('/charset=.+/', $type)) {
      $type .= ';charset=UTF-8';
    }
    $crawler->addContent($content, $type);

    return $crawler;
  }

}

class FeedsGoutteFetcherResult extends FeedsHTTPFetcherResult {

  private $goutteClient;
  private $goutteCrawler;

  public function __construct($client = NULL, $crawler = NULL) {
    $this->goutteClient = $client;
    $this->goutteCrawler = $crawler;
  }

  public function getRaw() {
    return $this->goutteClient->getResponse()->getContent();
  }

  public function getCrawler() {
    return $this->goutteCrawler;
  }
  
  public function getClient() {
    return $this->goutteClient;
  }

}

class FeedsGoutteFetcher extends FeedsHTTPFetcher {

  public $goutteClient;
  public $goutteCrawler;
  private $linksToFollow;
  private $linksFollowed;

  public function __construct($id) {
    parent::__construct($id);
  }

  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $url = $source_config['source'];
    $this->goutteClient = new GoutteClient();
    $this->goutteCrawler = $this->goutteClient->request('GET', $url);

    $crawler_config = $this->config['crawler'];
    $hrefs = array();
    if (!empty($crawler_config['pattern'])) {
      $hrefs = $this->getNextUrls($crawler_config, $url);
    }

    // don't have any links to follow
    if (empty($hrefs)) {
      return new FeedsGoutteFetcherResult($this->goutteClient, $this->goutteCrawler);
    }
    
    // Batch process
    $state = $source->state(FEEDS_FETCH);
    // don't forget myself
    array_unshift($hrefs, $url);
    if (!isset($state->hrefs)) {
      $state->hrefs = $hrefs;
      $state->total = count($state->hrefs);
    }
    if (count($state->hrefs)) {
      $href = array_shift($state->hrefs);
      $state->progress($state->total, $state->total - count($state->hrefs));
      $client = new GoutteClient();
      $crawler = $client->request('GET', $href);
      return new FeedsGoutteFetcherResult($client, $crawler);
    }
  }

  /**
   * Get remain link
   * @return type 
   */
  function linkGetRemain() {
    $remain = array_diff($this->linkFollow, $this->linkCrawled);
    sort($remain);
    return $remain;
  }

  /**
   * Get links crawled
   * @return type 
   */
  function linkGetCrawled() {
    return $this->linkCrawled;
  }

  /**
   * Mark a link is crawled and remove it from linkFollow list.
   * @param type $links
   * @return \SpiderClass 
   */
  function linkCrawled($links) {
    if (!is_array($links)) {
      $links = array($links);
    }
    $this->linkCrawled = array_unique(array_merge($this->linkCrawled, $links));
    return $this;
  }

  /**
   * Add link to follow link list
   * @param type $link
   * @return \SpiderClass 
   */
  function linkToFollowAdd($links) {
    if (!is_array($links)) {
      $links = array($links);
    }
    $this->linkToFollow = array_unique(array_merge($this->linkToFollow, $links));
    return $this;
  }

  /**
   * Remove links and return remain.
   * @param type $links
   * @return type 
   */
  function linkToFollowRemove($links) {
    if (!is_array($links)) {
      $links = array($links);
    }
    $this->linkToFollow = array_diff($this->linkToFollow, $links);
    return $this;
  }

  /**
   * Get next urls to follow
   * @param type $crawler_config
   * @param type $base_url
   * @return type 
   */
  private function getNextUrls($crawler_config, $base_url) {
    $href_pattern = $crawler_config['pattern'];

    $hrefs = $this->goutteCrawler->filterXPath('//a')->extract(array('href'));

    // filter url
    foreach ($hrefs as $delta => $href) {
      $href = trim($href);
      if (!preg_match("/$href_pattern/", $href)) {
        unset($hrefs[$delta]);
      }
      else {
        $urlEngine = new GoutteUrl();
        $href = $urlEngine->url_to_absolute($base_url, $href);
        $hrefs[$delta] = $urlEngine->normalizeURL($href);
      }
    }

    $hrefs = array_unique($hrefs);
    return $hrefs;
  }

  public function sourceForm($source_config) {
    $form = parent::sourceForm($source_config);

    $form['crawler'] = array(
      '#type' => 'fieldset',
      '#title' => t('Feeds Goutte Crawler settings'),
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#tree' => TRUE,
    );

    $form['crawler']['pattern'] = array(
      '#type' => 'textfield',
      '#title' => t(''),
      '#description' => t('Regular expression to match url'),
      '#default_value' => isset($source_config['crawler']['pattern']) ? $source_config['crawler']['pattern'] : '',
    );

    $form['crawler']['url'] = array(
      '#type' => 'fieldset',
      '#title' => t('URL replacement options'),
    );

    $form['crawler']['url']['url_pattern'] = array(
      '#type' => 'textfield',
      '#title' => t('URL pattern'),
      '#description' => t('A URL with the variable $index replaced with an increnting number. For example: http://example.com?page=$index.'),
      '#default_value' => isset($source_config['crawler']['url']['url_pattern']) ? $source_config['crawler']['url']['url_pattern'] : '',
      '#maxlength' => NULL,
    );

    $form['crawler']['url']['initial'] = array(
      '#type' => 'textfield',
      '#title' => t('Initial value of $index'),
      '#description' => t('The initial value of the $index variable.'),
      '#default_value' => isset($source_config['crawler']['url']['initial']) ? $source_config['crawler']['url']['initial'] : '',
    );

    $form['crawler']['url']['increment'] = array(
      '#type' => 'textfield',
      '#title' => t('Increment $index by'),
      '#description' => t('The increment the value of $index variable.'),
      '#default_value' => isset($source_config['crawler']['url']['increment']) ? $source_config['crawler']['url']['increment'] : '',
    );

    return $form;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = $this->sourceForm($this->config);
    unset($form['source']);
    unset($form['crawled']);
    $form['crawler']['#title'] = t('Feeds Crawler default settings');

    return parent::configForm($form_state) + $form;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $defaults = parent::configDefaults();
    $defaults['crawler'] = array(
      'pattern' => '',
      'url' => array(
        'url_pattern' => '',
        'initial' => 0,
        'increment' => 1,
      ),
    );
    return $defaults;
  }

}