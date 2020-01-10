<?php

/**
 * @file
 * Permission settings for the loop_documents module.
 */

/**
 * Implements hook_permissions_secure_permissions_roles().
 */
function ulf_free_course_request_secure_permissions_roles() {
  return array(
    'skole',
  );
}

/**
 * Implements hook_permissions().
 *
 * Add permission to restrict access to certain admin paths.
 */
function ulf_free_course_request_permission() {
  $permissions = array(
    'administer ulf free course request settings' => array(
      'title' => t('Administer free course request settings'),
      'description' => t('Administer free course request settings'),
    ),
  );

  return $permissions;
}

/**
 * Implements hook_secure_permissions().
 */
function ulf_free_course_request_secure_permissions($role) {
  $permissions = array(
    'skole' => array(
//      'view own entityform',
      'view the administration theme',
      'access toolbar',
    ),
    'editor' => array(
      'view any entityform',
      'edit any entityform',
      'delete any entityform',
      'assign skole role',
      'administer ulf free course request settings',
    ),
  );

  return isset($permissions[$role]) ? $permissions[$role] : NULL;
}
