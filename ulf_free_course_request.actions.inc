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

  $actions['_rfc_cancelled'] = array(
    'type' => 'entityform',
    'label' => t('Change status to cancelled'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_ready_for_payment'] = array(
    'type' => 'entityform',
    'label' => t('Change status to Ready for Payment'),
    'configurable' => FALSE,
    'triggers' => array('any'),
    'behavior' => array('changes_property'),
    'vbo_configurable' => FALSE,
    'pass rows' => TRUE,
  );

  $actions['_rfc_payment_completed'] = array(
    'type' => 'entityform',
    'label' => t('Change status to Payment Completed'),
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
  _rfc_change_entity($entity, 'accepted');
}

/**
 * Custom action for bulk operation. (Accepted and send mail)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_accept($entity) {
  _rfc_change_entity($entity, 'accepted_mail');
}

/**
 * Custom action for bulk operation. (Refused)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_change_status_refused($entity) {
  _rfc_change_entity($entity, 'refused');
}

/**
 * Custom action for bulk operation. (Refused and send mail)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_refuse($entity) {
  _rfc_change_entity($entity, 'refused_mail');
}

/**
 * Custom action for bulk operation. (Change status to ready for payment)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_ready_for_payment($entity) {
  _rfc_change_entity($entity, 'ready_for_payment');
}

/**
 * Custom action for bulk operation. (Change status to payment completed)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_payment_completed($entity) {
  _rfc_change_entity($entity, 'payment_completed');
}

/**
 * Custom action for bulk operation. (Change status to payment cancelled)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_cancelled($entity) {
  _rfc_change_entity($entity, 'cancelled');
}

/**
 * Custom action for bulk operation. (Close)
 *
 * @param object $entity
 *   The entity to act on.
 */
function _rfc_close($entity) {
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
      if (!empty($wrapper->field_rfc_granted_amount->value())) {
        $wrapper->field_rfc_status = 'accepted';
        $wrapper->save();
      }
      else {
        drupal_set_message(t('The application with id: @id was not accepted due to missing granted amount.', array(
          '@id' => $wrapper->getIdentifier(),
        )), 'warning');
      }
      break;

    case 'accepted_mail':
      if (!empty($wrapper->field_rfc_granted_amount->value())) {
        $wrapper->field_rfc_status = 'accepted';
        $wrapper->save();
        _rfc_notify($wrapper, 'on_change_accepted');
      }
      else {
        drupal_set_message(t('The application with id: @id was not accepted due to missing granted amount.', array(
          '@id' => $wrapper->getIdentifier(),
        )), 'warning');
      }
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

    case 'ready_for_payment':
      $wrapper->field_rfc_status = 'ready_for_payment';
      $wrapper->save();
      break;

    case 'payment_completed':
      $wrapper->field_rfc_status = 'payment_completed';
      $wrapper->field_rfc_payment_date = time();
      $wrapper->save();
      break;

    case 'cancelled':
      $wrapper->field_rfc_status = 'cancelled';
      $wrapper->save();
      break;

    case 'cancelled':
      $wrapper->field_rfc_status = 'cancelled';
      $wrapper->save();
      break;

    case 'closed':
      $wrapper->field_rfc_status = 'closed';
      $wrapper->save();
      break;
  }
}
