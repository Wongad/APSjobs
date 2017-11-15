<?php
use Goutte\Client;
use Symfony\Component\DomCrawler\Crawler;

function apsjobs_parser_content_load($agency) {
  // print_r($agency);
  $recruitment_urls = apsjobs_utils_get_field_from_entity($agency, 'field_recruitment_url', 'url', TRUE);
  $agency_id = apsjobs_utils_get_field_from_entity($agency, 'field_agency_id');
  foreach($recruitment_urls as $url) {
  	apsjobs_parser_content_load_page($url, $agency_id);
  }
  exit;
}

function apsjobs_parser_curl_get_content($url) {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_VERBOSE, 1);
  curl_setopt($ch, CURLOPT_COOKIEJAR, "cookie.txt");
  curl_setopt($ch, CURLOPT_COOKIEFILE, "cookie.txt");
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt($ch, CURLOPT_FAILONERROR, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); 
  curl_setopt($ch, CURLOPT_URL, $url);
  $content = curl_exec($ch);
  return $content;
}

function apsjobs_parser_content_load_page($url, $agency_id) {
  $content = apsjobs_parser_curl_get_content($url);
  $parts = parse_url($url);
  $root_url = (isset($parts['scheme']) ? $parts['scheme'] : "http") . "://" . $parts['host'];
  $crawler = new Crawler($content);
  // print_r($content);
  switch($agency_id) {
  	case 117:
      // $vacancies_text = $crawler->filter('#current-vacancies')->text();
  	  $sections = $crawler->filter('#jobs-listing > .child-pages');
      $root_url .= "/jobs";
      foreach($sections as $section) {
        $cs = new Crawler($section);
        $jobs = $cs->filter("ul > li");
        foreach($jobs as $job) {
          $cj = new Crawler($section);
          $href = $cj->filter("a")->attr('href');
          $ps = parse_url($href);
          $job_url = isset($ps['domain']) ? $href : ($root_url . "/" . $href);
          $detail_content = apsjobs_parser_curl_get_content($job_url);
          print_r($detail_content);
          exit;
        }
        print_r($jobs);
        exit;
      }
  	  break;
  	default: 
  	  break;
  }
}

