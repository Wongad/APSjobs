<?php

use Goutte\Client;
use Symfony\Component\DomCrawler\Crawler;
use PhpOffice\PhpWord;
// use PhpOffice\PhpWord\PhpWord;
use PhpOffice\PhpWord\IOFactory;
use PhpOffice\PhpWord\Element;
use PhpOffice\PhpWord\Section;
use PhpOffice\PhpWord\Reader\Word2007;
use PhpOffice\Common\XMLReader;
// use PhpOffice\PhpWord\Element\Text;

function apsjobs_parser_form () {
  $rows = db_select("node", "n")
    ->fields("n")
    ->condition("type", "aps_agencies")
    ->execute()
    ->fetchAll();
  $options = array();
  foreach($rows as $row) {
  	$options[$row->nid] = $row->title;
  }
  asort($options);
  $form = array();
  $form['agency_nid'] = array(
	'#type' => 'select',
	'#title' => t('Agency'),
	'#options' => array(
		'' => t("-- Select Agency --"),
	) + $options,
	'#required' => TRUE,
	'#default_value' => 160,
  );
  $form['actions'] = array(
	'#type' => 'submit',
	'#value' => t('Run')
  );
  return $form;
}

function apsjobs_parser_form_submit($form, &$form_state) {
  $data = $form_state['input'];
  $agency_nid = $data['agency_nid'];
  $data_url = "https://www.apsjobs.gov.au/quickSearch.aspx?mn=JobSearch&ifm=true";
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_VERBOSE, 1);
  curl_setopt($ch, CURLOPT_COOKIEJAR, "cookie.txt");
  curl_setopt($ch, CURLOPT_COOKIEFILE, "cookie.txt");
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt($ch, CURLOPT_FAILONERROR, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);	
  curl_setopt($ch, CURLOPT_URL, $data_url);
  $content = curl_exec($ch);

  $params_query = array(
    '__EVENTTARGET' => '',
    '__EVENTARGUMENT' => '',
    '__LASTFOCUS' => '',
    '__VIEWSTATE' => '',
    '__SCROLLPOSITIONX' => 0,
    '__SCROLLPOSITIONY' => 0,
    '__VIEWSTATEENCRYPTED' => '',
    'ctl00$ContentPlaceHolderSite$ucAgency' => '',
    'ctl00$ContentPlaceHolderSite$ucJobCategory' => '',
    'ctl00$ContentPlaceHolderSite$ucClassification' => '',
    'ctl00$ContentPlaceHolderSite$ucState' => '',
    'ctl00$ContentPlaceHolderSite$txtKeywords' => '',
    'ctl00$ContentPlaceHolderSite$txtMinSalary' => '',
    'ctl00$ContentPlaceHolderSite$txtMaxSalary' => '',
    'ctl00$ContentPlaceHolderSite$btnSearch' => 'Search',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$ucPageSize$lPS' => 10,
    'ctl00$ContentPlaceHolderSite$lvSearchResults$lstSortOptions' => 'Agency',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hSelectAll' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hSelected' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hUnselected' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hY' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hRestoreY' => 'false',
    'ctl00$ContentPlaceHolderSite$hMessage' => '',
  );

  // foreach ($params as $key => $value) {
  //   $params_query[$key] = $value;
  // }
  curl_setopt($ch, CURLOPT_URL, $data_url);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $params_query);
  $content = curl_exec($ch);


  $params_query = array(
    '__EVENTTARGET' => 'ctl00$ContentPlaceHolderSite$lvSearchResults$ucPageSize$lPS',
    '__EVENTARGUMENT' => '',
    '__LASTFOCUS' => '',
    '__VIEWSTATE' => '',
    '__SCROLLPOSITIONX' => 0,
    '__SCROLLPOSITIONY' => 717,
    '__VIEWSTATEENCRYPTED' => '',
    'ctl00$ContentPlaceHolderSite$ucAgency' => '',
    'ctl00$ContentPlaceHolderSite$ucJobCategory' => '',
    'ctl00$ContentPlaceHolderSite$ucClassification' => '',
    'ctl00$ContentPlaceHolderSite$ucState' => '',
    'ctl00$ContentPlaceHolderSite$txtKeywords' => '',
    'ctl00$ContentPlaceHolderSite$txtMinSalary' => '',
    'ctl00$ContentPlaceHolderSite$txtMaxSalary' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$ucPageSize$lPS' => 1000,
    'ctl00$ContentPlaceHolderSite$lvSearchResults$lstSortOptions' => 'Agency',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hSelectAll' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hSelected' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hUnselected' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hY' => '',
    'ctl00$ContentPlaceHolderSite$lvSearchResults$hRestoreY' => 'false',
    'ctl00$ContentPlaceHolderSite$hMessage' => '',
  );

  curl_setopt($ch, CURLOPT_URL, $data_url);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $params_query);
  $content = curl_exec($ch);

  $crawler = new Crawler($content);

  print_r($content);
  exit;
  curl_setopt($ch, CURLOPT_URL, $result_url);
  curl_setopt($ch, CURLOPT_POSTFIELDS, NULL);
  $content = curl_exec($ch);
  curl_close ($ch);
  return $content;
}

function apsjobs_agency_parser_form() {
  $form = array();
  $rows = db_select("node", "n")
    ->fields("n")
    ->condition("type", "aps_agencies")
    ->execute()
    ->fetchAll();
  $options = array();
  foreach($rows as $row) {
    $agency = node_load($row->nid);
    $agency_id = apsjobs_utils_get_field_from_entity($agency, 'field_agency_id');
    if(!empty($agency_id)) {
      $options[$agency->field_agency_id['und'][0]['value']] = $row->title;
    }
  }
  asort($options);
  $form = array();
  $form['agency_id'] = array(
    '#type' => 'select',
    '#title' => t('Agency'),
    '#options' => array(
      '' => t("-- Select Agency --"),
    ) + $options,
    '#required' => TRUE,
    '#default_value' => 117,
  );
  $form['actions'] = array(
    '#type' => 'submit',
    '#value' => t('Run')
  );
  return $form;  
}

function apsjobs_agency_parser_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $agency_id = $values['agency_id'];
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'aps_agencies')
    ->fieldCondition('field_agency_id', 'value', $agency_id, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    $news_items_nids = array_keys($result['node']);
    $news_items = entity_load('node', $news_items_nids);
    $agency = reset($news_items);
    apsjobs_parser_content_load($agency);
  } 
}

function apsjobs_word_parser_form() {
  $form = array();
  $rows = db_select("node", "n")
    ->fields("n")
    ->condition("type", "aps_agencies")
    ->execute()
    ->fetchAll();
  $options = array();
  foreach($rows as $row) {
    $agency = node_load($row->nid);
    $agency_id = apsjobs_utils_get_field_from_entity($agency, 'field_agency_id');
    if(!empty($agency_id)) {
      $options[$agency->field_agency_id['und'][0]['value']] = $row->title;
    }
  }
  asort($options);
  $form = array();
  $form['agency_id'] = array(
    '#type' => 'select',
    '#title' => t('Agency'),
    '#options' => array(
      '' => t("-- Select Agency --"),
    ) + $options,
    '#required' => TRUE,
    '#default_value' => 117,
  );
  $form['actions'] = array(
    '#type' => 'submit',
    '#value' => t('Run')
  );
  return $form;
}

function apsjobs_word_parser_form_submit($form, &$form_state) {
  global $user;
  $url = "http://aciar.gov.au/files/aciar_candidate_information_pack.docx";
  // $url = "/Volumes/Data/testphpword/Sample_11_ReadWord2007.docx";
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

  // $filename = 


  $phpWord = IOFactory::load($url);
  $document = new \PhpOffice\PhpWord\Reader\Word2007\Document();
  $document->read($phpWord);
  print_r($document);
  exit;
  // $phpWord->generateXHTML();
  $html = $phpWord->getStrXHTML();

  print_r($html);
  exit;
  // print_r($phpWord);
  $sections = $phpWord->getSections();

  foreach ($sections as $s) {
      $els = $s->getElements();
      foreach ($els as $e) {
        print_r($e);
        // get_class($e);
          if (get_class($e) === 'Text') {
              $text .= $e->getText();
          } elseif (get_class($e) === 'TextBreak') {
              $text .= " \n";
          } else {
              // throw new Exception('Unknown class type ' . get_class($e));
          }
      }
  }

  print $text;

  exit;

  // $phpWord = new \PhpOffice\PhpWord\PhpWord();

  // $section = $phpWord->addSection();
  // $section->addText(
  //     '"Learn from yesterday, live for today, hope for tomorrow. '
  //         . 'The important thing is not to stop questioning." '
  //         . '(Albert Einstein)'
  // );
  // $section->addText(
  //     '"Great achievement is usually born of great sacrifice, '
  //         . 'and is never the result of selfishness." '
  //         . '(Napoleon Hill)',
  //     array('name' => 'Tahoma', 'size' => 10)
  // );
  // $fontStyleName = 'oneUserDefinedStyle';
  // $phpWord->addFontStyle(
  //     $fontStyleName,
  //     array('name' => 'Tahoma', 'size' => 10, 'color' => '1B2232', 'bold' => true)
  // );
  // $section->addText(
  //     '"The greatest accomplishment is not in never falling, '
  //         . 'but in rising again after you fall." '
  //         . '(Vince Lombardi)',
  //     $fontStyleName
  // );

  // $fontStyle = new \PhpOffice\PhpWord\Style\Font();
  // $fontStyle->setBold(true);
  // $fontStyle->setName('Tahoma');
  // $fontStyle->setSize(13);
  // $myTextElement = $section->addText('"Believe you can and you\'re halfway there." (Theodor Roosevelt)');
  // $myTextElement->setFontStyle($fontStyle);

  // $objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($phpWord, 'Word2007');
  // $objWriter->save('/Volumes/Data/testphpword/helloWorld.docx');

  // $objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($phpWord, 'ODText');
  // $objWriter->save('/Volumes/Data/testphpword/helloWorld.odt');

  // $objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($phpWord, 'HTML');
  // $objWriter->save('/Volumes/Data/testphpword/helloWorld.html');

  // print_r("afasfafasf");
  // exit;
}
