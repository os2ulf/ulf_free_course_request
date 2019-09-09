<?php
/**
 * @file
 * ulf_free_course_request.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function ulf_free_course_request_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_gdpr|entityform|free_course_request|form';
  $field_group->group_name = 'group_gdpr';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'free_course_request';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Databeskyttelse',
    'weight' => '4',
    'children' => array(
      0 => 'field_rfc_gdpr',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Databeskyttelse',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => 'group-gdpr field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_gdpr|entityform|free_course_request|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rfc_course|entityform|free_course_request|form';
  $field_group->group_name = 'group_rfc_course';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'free_course_request';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Forløb',
    'weight' => '2',
    'children' => array(
      0 => 'field_rfc_course',
      1 => 'field_rfc_course_not_found',
      2 => 'field_rfc_date',
      3 => 'field_rfc_new_course_description',
      4 => 'field_rfc_new_course_name',
      5 => 'field_rfc_provider',
      6 => 'field_rfc_requested_amount',
      7 => 'field_rfc_subject',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Forløb',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => 'group-rfc-course field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_rfc_course|entityform|free_course_request|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rfc_editors|entityform|free_course_request|form';
  $field_group->group_name = 'group_rfc_editors';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'free_course_request';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Redaktion',
    'weight' => '0',
    'children' => array(
      0 => 'field_rfc_granted_amount',
      1 => 'field_rfc_remarks',
      2 => 'field_rfc_send_mail',
      3 => 'field_rfc_status',
      4 => 'field_rfc_mails_sent',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Redaktion',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-rfc-editors field-group-fieldset',
        'description' => 'Kun synlig for redaktører.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_rfc_editors|entityform|free_course_request|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rfc_school|entityform|free_course_request|form';
  $field_group->group_name = 'group_rfc_school';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'free_course_request';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skole',
    'weight' => '1',
    'children' => array(
      0 => 'field_rfc_grade',
      1 => 'field_rfc_mail',
      2 => 'field_rfc_name',
      3 => 'field_rfc_phone',
      4 => 'field_rfc_school',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Skole',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => 'group-rfc-school field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_rfc_school|entityform|free_course_request|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_rfc_submission|entityform|free_course_request|form';
  $field_group->group_name = 'group_rfc_submission';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'free_course_request';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Det med småt',
    'weight' => '3',
    'children' => array(
      0 => 'field_rfc_conditions',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Det med småt',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_rfc_submission|entityform|free_course_request|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Databeskyttelse');
  t('Det med småt');
  t('Forløb');
  t('Redaktion');
  t('Skole');

  return $field_groups;
}
