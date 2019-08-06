<?php
/**
 * @file
 * Actions setup for the Ulf free course request feature.
 */

/**
 * Add actions to Views bulk operations.
 *
 * Implements hook_action_info().
 */
function ulf_free_course_request_action_info() {
  $actions = array();

  $actions['_rfc_change_status_accepted'] = array(
    'type' => 'entityform',
    'label' => t('Change status to accepted (No mail)'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_change_status_refused'] = array(
    'type' => 'entityform',
    'label' => t('Change status to refused (No mail)'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_accept'] = array(
    'type' => 'entityform',
    'label' => t('Accept (With mail)'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_refuse'] = array(
    'type' => 'entityform',
    'label' => t('Refuse (With mail)'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_close'] = array(
    'type' => 'entityform',
    'label' => t('Close'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );


  return $actions;
}

/**
 * Custom action for bulk operation. (Accepted)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_change_status_accepted($entity) {
  $a =1;
  _rfc_change_entity($entity, 'accepted');
}

/**
 * Custom action for bulk operation. (Accepted and send mail)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_accept($entity) {
  $a =1;
  _rfc_change_entity($entity, 'accepted_mail');
}

/**
 * Custom action for bulk operation. (Refused)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_change_status_refused($entity) {
  $a =1;
  _rfc_change_entity($entity, 'refused');
}

/**
 * Custom action for bulk operation. (Refused and send mail)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_refuse($entity) {
  $a =1;
  _rfc_change_entity($entity, 'refused_mail');
}

/**
 * Custom action for bulk operation. (Close)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_close($entity) {
  $a =1;
  _rfc_change_entity($entity, 'closed');
}

/**
 * Set status and send mail based on action selected.
 *
 * @param object $entity
 *   The entity to change.
 * @param string $action
 *   The action to take on the entity.
 */
function _rfc_change_entity($entity, $action) {
  $wrapper = entity_metadata_wrapper('entityform', $entity);
  switch ($action) {
    case 'accepted':
      $wrapper->field_rfc_status = 'accepted';
      $wrapper->save();
      break;

    case 'accepted_mail':
      $wrapper->field_rfc_status = 'accepted';
      $wrapper->save();
      _rfc_notify($wrapper, 'on_change_accepted');
      break;

    case 'refused':
      $wrapper->field_rfc_status = 'refused';
      $wrapper->save();
      break;

    case 'refused_mail':
      $wrapper->field_rfc_status = 'refused';
      $wrapper->save();
      _rfc_notify($wrapper, 'on_change_refused');
      break;

    case 'closed':
      $wrapper->field_rfc_status = 'closed';
      $wrapper->save();
      break;
  }
}
