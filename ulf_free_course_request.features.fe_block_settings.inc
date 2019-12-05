<?php

/**
 * @file
 * ulf_free_course_request.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function ulf_free_course_request_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['ulf_free_course_request-free_course_request_summary'] = array(
    'cache' => 1,
    'custom' => 0,
    'delta' => 'free_course_request_summary',
    'module' => 'ulf_free_course_request',
    'node_types' => array(),
    'pages' => 'admin/config/system/free_course_request
admin/udgiftsdaekning/ansoegninger/*',
    'roles' => array(),
    'themes' => array(
      'ulf_aarhus' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'ulf_aarhus',
        'weight' => 0,
      ),
      'ulf_admin' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'ulf_admin',
        'weight' => -20,
      ),
    ),
    'title' => '',
    'visibility' => 1,
  );

  return $export;
}
