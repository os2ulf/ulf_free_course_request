<?php

/**
 * @file
 * Holds alterations for forms related to ulf_free_course_request module.
 */

/**
 * Implements hook_form_FORMID_alter().
 */
function ulf_free_course_request_form_user_profile_form_alter(&$form, $form_state) {
  // If on a 'skole' profile.
  if (in_array('skole', $form['#user']->roles)) {
    $form['picture']['#access'] = FALSE;
    // Hide all fields
    foreach ($form as $key => $value) {
      if (strpos($key, 'field_') !== FALSE) {
        $form[$key]['#access'] = FALSE;
      }
    }

    // But show name.
    $form['field_profile_name']['#access'] = TRUE;
  }
}

/**
 * Implements hook_form_FORMID_alter().
 */
function ulf_free_course_request_form_free_course_request_entityform_edit_form_alter(&$form, $form_state) {
  global $user;
  drupal_set_title(variable_get('ufcr_title'));

  $form['field_rfc_follow_up_mail_sent']['#access'] = FALSE;

  $form['#logged_in_user'] = $user;
  // Set layout of page.
  $intro_text = variable_get(
    'ufcr_intro', [
      'value' => '',
      'format' => NULL,
    ]
  )['value'];
  $form['#prefix']
    = '<div class="fcr-form"><div class="user-message--wrapper"><div class="user-message">'
    . $intro_text . '</div></div>';
  $form['#suffix'] = '</div>';

  // Add js and css to form.
  $form['#attached']['css'][] = drupal_get_path('module', 'ulf_free_course_request') . '/css/styling.css';
  $form['#attached']['library'] = [
    ['system', 'ui.datepicker'],
  ];

  // Hide editorial fields for non editors.
  if (!in_array('editor', $user->roles)) {
    $form['field_rfc_status']['#access'] = FALSE;
    $form['field_rfc_granted_amount']['#access'] = FALSE;
    $form['field_rfc_remarks']['#access'] = FALSE;
    $form['field_rfc_send_mail']['#access'] = FALSE;
  }
  // Always hide mails sent field. Set in code, and only used for display.
  $form['field_rfc_mails_sent']['#access'] = FALSE;
  $form['field_rfc_latest_editor']['#access'] = FALSE;
  $form['field_rfc_date'][LANGUAGE_NONE][0]['#required'] = TRUE;


  // Auto fill send mail checkbox when editing an application.
  if ($form_state['entityform_form_mode'] == 'edit') {
    $form['field_rfc_send_mail'][LANGUAGE_NONE]['#default_value'] = 1;
  }

  // Hide format options from description field.
  $form['field_rfc_remarks']['#after_build'][] = '_rfc_after';

  // Fill input fields for school users.
  if (in_array('skole', $user->roles)) {
    // Add user email if user is logged in.
    $form['field_rfc_mail'][LANGUAGE_NONE]['0']['value']['#default_value']
      = $user->mail;
    $form['field_rfc_school'][LANGUAGE_NONE]['#default_value'][0] = $user->uid;
  }

  $entity = $form_state['entityform'];

  if (property_exists($entity, 'is_new') && $entity->is_new == TRUE) {
    $status = 'new';
  }
  else {
    $status = $entity->field_rfc_status[LANGUAGE_NONE][0]['value'];
    array_unshift($form['actions']['save']['#submit'], '_rfc_request_form_submit_handle_status_change');
  }


  if (in_array('editor', $user->roles)) {
    //felter skjules aldrig for redaktører
    unset($form['actions']['captcha']);
  }
  else {
    if (in_array('skole', $user->roles)
      && ($status == 'accepted'
        || $status == 'awaiting_payment')
    ) {
      //hvis bruger har rollen "skole" og hvis ansøgningen har status "Godkendt" eller "Afventer udbetaling"
      //vises payment felter og alle andre felter deaktiveres
      $form['field_rfc_conditions'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_course'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_course_not_found'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_date'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_gdpr'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_grade'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_mail'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_name'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_new_course_description'][LANGUAGE_NONE]['#disabled']
        = TRUE;
      $form['field_rfc_new_course_name'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_phone'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_provider'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_requested_amount'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_school'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['field_rfc_subject'][LANGUAGE_NONE]['#disabled'] = TRUE;

      $form['field_rfc_spent_amount'][LANGUAGE_NONE][0]['value']['#required']
        = TRUE;
      $form['field_rfc_attachment'][LANGUAGE_NONE][0]['#required'] = TRUE;
      $form['field_rfc_payment_date']['#access'] = FALSE;

      $form['field_rfc_granted_amount']['#access'] = TRUE;
      $form['field_rfc_granted_amount'][LANGUAGE_NONE]['#disabled'] = TRUE;
      $form['#group_children']['field_rfc_granted_amount']
        = 'group_rfc_payment';

      array_unshift($form['actions']['save']['#submit'], '_rfc_request_form_submit_payment_details');
      unset($form['actions']['captcha']);

    }
    else {
      //ellers skjules payment felter
      $form['field_rfc_spent_amount']['#access'] = FALSE;
      $form['field_rfc_attachment']['#access'] = FALSE;
      $form['field_rfc_payment_date']['#access'] = FALSE;
    }
  }


  $form['#attached']['js'] = [
    drupal_get_path('module', 'ulf_free_course_request')
    . '/js/change-course-value.js',
    drupal_get_path('module', 'ulf_free_course_request')
    . '/js/calendar-popup.js',
    drupal_get_path('module', 'ulf_free_course_request')
    . '/js/change-subject.js',
    [
      'type' => 'setting',
      'data' => [
        'fcr' => [
          'from' => explode('-', variable_get('ufcr_from')),
          'to' => explode('-', variable_get('ufcr_to')),
          'subjects' => _rfc_get_subjects(),
          'create_mode' => $form_state['entityform_form_mode'] == 'submit' ? 1
            : 0,
        ],
      ],
    ],
  ];

  // Auto fill send mail checkbox when editing an application.
  if ($form_state['entityform_form_mode'] == 'edit') {
    $form['field_rfc_send_mail'][LANGUAGE_NONE]['#default_value'] = 1;
  }


  // Add ajax to provider field.
  $form['field_rfc_provider'][LANGUAGE_NONE]['#ajax'] = [
    'event' => 'change',
    'callback' => '_rfc_change_course_dropdown',
    'wrapper' => 'course-dropdown-wrapper',
  ];

  // Add ajax to course field.
  $form['field_rfc_course'][LANGUAGE_NONE]['#ajax'] = [
    'event' => 'change',
    'callback' => '_rfc_show_price_info',
    'wrapper' => 'course-price-wrapper',
  ];

  // Add ID wrapper to course dropdown for ajax use.
  $form['field_rfc_course']['#prefix'] = '<div id="course-dropdown-wrapper">';
  $form['field_rfc_course']['#suffix'] = '</div>';

  // Add ID wrapper to price dropdown for ajax use.
  $form['field_rfc_requested_amount']['#prefix']
    = '<div id="course-price-wrapper">';
  $form['field_rfc_requested_amount']['#suffix'] = '</div>';

  if ($form_state['entityform_form_mode'] == 'submit') {
    // Add option _none for dropdowns if it does not exist. (Due to ajax).
    if (!array_key_exists('_none', $form['field_rfc_course'][LANGUAGE_NONE]['#options'])) {
      $form['field_rfc_course'][LANGUAGE_NONE]['#options']
        = ['_none' => t('- Select a course  -')] +
        $form['field_rfc_course'][LANGUAGE_NONE]['#options'];
    }
    else {
      $form['field_rfc_course'][LANGUAGE_NONE]['#options']['_none']
        = t('- Select a course  -');
    }
    if (!array_key_exists('_none', $form['field_rfc_provider'][LANGUAGE_NONE]['#options'])) {
      $form['field_rfc_provider'][LANGUAGE_NONE]['#options']
        = ['_none' => t('- Select a provider -')] +
        $form['field_rfc_provider'][LANGUAGE_NONE]['#options'];
    }
    else {
      $form['field_rfc_provider'][LANGUAGE_NONE]['#options']['_none']
        = t('- Select a provider  -');
    }

    // Add no select options on create mode.
    $form['field_rfc_subject'][LANGUAGE_NONE]['#options']['_none']
      = t('- Select a subject  -');
    $form['field_rfc_school'][LANGUAGE_NONE]['#options']['_none']
      = t('- Select a school  -');
  }

  if (!property_exists($form['#entity'], 'is_new') || $form['#entity']->is_new != TRUE) {
    if ($form['#entity']->field_rfc_course_not_found[LANGUAGE_NONE][0]['value'] && !empty($form['#entity']->field_rfc_new_course_name[LANGUAGE_NONE][0]['value'])) {
      $form['field_rfc_course'][LANGUAGE_NONE]['#options'] = _rfc_get_hacked_course_options($user, '');
      $form['field_rfc_course'][LANGUAGE_NONE]['#default_value'] = '';
    } else {
      $form['field_rfc_course'][LANGUAGE_NONE]['#options'] = _rfc_get_hacked_course_options($user, $form['#entity']->field_rfc_course[LANGUAGE_NONE][0]['target_id']);
      $form['field_rfc_course'][LANGUAGE_NONE]['#default_value'] = $form['#entity']->field_rfc_course[LANGUAGE_NONE][0]['target_id'];
    }
    array_unshift($form['actions']['save']['#submit'], '_rfc_request_form_submit_handle_course');
  }
  else {
    // Get new courses for dropdown if provider is set.
    if (isset($form_state['values']['field_rfc_provider'][LANGUAGE_NONE]['0'])) {
      $field_value
        = $form_state['values']['field_rfc_provider'][LANGUAGE_NONE]['0'];
      $form['field_rfc_course'][LANGUAGE_NONE]['#options'] = _rfc_get_courses(
        isset($field_value['target_id']) ? $field_value['target_id']
          : $field_value,
        $form['field_rfc_course'][LANGUAGE_NONE]['#options']
      );
    }
  }

  // Set required and hide fields based on field_rfc_course_not_found checkbox.
  $form['field_rfc_course']['#states'] = [
    'required' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => FALSE],
    ],
    'visible' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => FALSE],
    ],
  ];
  $form['field_rfc_new_course_name']['#states'] = [
    'required' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => TRUE],
    ],
    'visible' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => TRUE],
    ],
  ];
  $form['field_rfc_new_course_description']['#states'] = [
    'required' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => TRUE],
    ],
    'visible' => [
      ':input[id="edit-field-rfc-course-not-found-und"]' => ['checked' => TRUE],
    ],
  ];

  // Add custom validation handler.
  $form['#validate'][] = '_rfc_request_form_validate';

  // Get price info if course is selected.
  if (isset($form_state['values']['field_rfc_course'][LANGUAGE_NONE]['0']['target_id'])) {
    $price_info
      = _rfc_get_course_price_info($form_state['values']['field_rfc_course'][LANGUAGE_NONE]['0']['target_id']);
    $form['field_rfc_requested_amount']['#prefix']
      = '<div id="course-price-wrapper">' . $price_info;
  }

  // Set GDPR and Cancellation texts.
  $form['field_rfc_conditions']['#prefix']
    = variable_get(
    'ufcr_cancellation', [
      'value' => '',
      'format' => NULL,
    ]
  )['value'];
  $form['field_rfc_gdpr']['#prefix'] = variable_get(
    'ufcr_gdpr', [
      'value' => '',
      'format' => NULL,
    ]
  )['value'];


  if ($form_state['entityform_form_mode'] == 'submit') {
    $form['field_rfc_provider_name']['#access'] = FALSE;
    $form['field_rfc_course_name']['#access'] = FALSE;
    array_unshift($form['actions']['submit']['#submit'], '_rfc_request_form_submit');
  }

  if ($form_state['entityform_form_mode'] == 'edit') {
    $form['field_rfc_provider_name']['#access'] = FALSE;
    $form['field_rfc_course_name']['#access'] = FALSE;
    array_unshift($form['actions']['save']['#submit'], '_rfc_request_form_submit');
  }

  if (isset($_GET['course']) && is_numeric($_GET['course'])) {
    $course = node_load($_GET['course']);
    if ($course !== NULL) {
      $form['field_rfc_provider'][LANGUAGE_NONE]['#default_value'] = $course->uid;
      $form['field_rfc_course'][LANGUAGE_NONE]['#default_value'] = $course->nid;
    }
  }
}

/**
 * Implements hook_form_FORMID_alter().
 */
function ulf_free_course_request_form_views_exposed_form_alter(&$form, $form_state) {
  if (
    $form['#id'] === 'views-exposed-form-ulf-free-course-requests-free-course-requests-table' ||
    $form['#id'] === 'views-exposed-form-ulf-free-course-requests-free-course-requests-user'
  ) {
    $form['#attached']['css'][]
      = drupal_get_path('module', 'ulf_free_course_request')
      . '/css/styling.css';
    $query = db_select('field_data_field_rfc_date');
    // exclude results older than 2019, since they must be a mistake
    $query->condition('field_rfc_date_value', '1546297200', '>');
    $query->addExpression('MIN(field_rfc_date_value)', 'min');
    $query->addExpression('MAX(field_rfc_date_value)', 'max');
    $result = $query->execute()->fetchAll();
    $result = reset($result);
    $min = $result->min;
    $max = $result->max;
    $options = [];
    while ($min < $max) {
      $month = date('n', $min);
      $year = date('Y', $min);
      if ($month > 6) {
        $options[$year . '_2'] = '1. halvår ' . $year . '/' . ($year + 1);
        $min = strtotime(($year + 1) . '-01-01');
      }
      else {
        $options[$year . '_1'] = '2. halvår ' . ($year - 1) . '/' . ($year);
        $min = strtotime($year . '-07-01');
      }
    }

    $form['field_rfc_date_value_2'] = [
      '#title' => 'Semester',
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $options,
    ];
    $form['field_rfc_date_value_2']['min']['#disabled'] = TRUE;
    $form['field_rfc_date_value_2']['max']['#disabled'] = TRUE;
    $form['submit']['#weight'] = 100;
  }
}

/**
 * Change the course dropdown.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function _rfc_change_course_dropdown($form, $form_state) {
  return $form['field_rfc_course'];
}

/**
 * Show price information.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function _rfc_show_price_info($form, $form_state) {
  return $form['field_rfc_requested_amount'];
}

/**
 * Get info about course prices.
 *
 * @param $nid
 *
 * @return string
 */
function _rfc_get_course_price_info($nid) {
  // If _none is selected.
  if (!is_numeric($nid)) {
    return '';
  }

  // Get node values.
  $wrapper = entity_metadata_wrapper('node', $nid);
  $price_description = $wrapper->field_price_description->value();
  $price_ref = $wrapper->field_collection_price->value();
  $free = $wrapper->field_free->value();

  // Get allowed values list for units and vat.
  $field_unit_info = list_allowed_values(field_info_field('field_unit_price'));
  $field_vat_info = list_allowed_values(field_info_field('field_moms'));

  $text = '';

  // Start wrapping tag.
  if (!empty($price_description) || !empty($price_ref) || $free) {
    $text .= '<div class="price-info">';
  }
  else {
    $text .= '<div>';
  }

  // Add header
  $text .= '<strong>' . t('Price') . '</strong>';

  // Add free notification.
  if ($free) {
    $text .= '<div>' . t('This course is free.') . '</div>';
  }

  // Add price lines.
  if (!empty($price_ref)) {
    $text .= '<ul class="price-list">';

    foreach ($price_ref as $price_line) {
      $text .= '<li>';
      if (!empty($price_line->field_price)) {
        $text .= '<span>'
          . str_replace('.', ',', $price_line->field_price[LANGUAGE_NONE]['0']['value'])
          . '</span>';
      }
      if (!empty($price_line->field_unit_price)) {
        $text .= '<span>'
          . $field_unit_info[$price_line->field_unit_price[LANGUAGE_NONE]['0']['value']]
          . '</span>';
      }
      if (!empty($price_line->field_moms)) {
        $text .= '<span>('
          . $field_vat_info[$price_line->field_moms[LANGUAGE_NONE]['0']['value']]
          . ')</span>';
      }
      $text .= '</li>';
    }

    $text .= '</ul>';
  }

  // Add price description.
  if (!empty($price_description['safe_value'])) {
    $text .= $price_description['safe_value'];
  }

  // Close wrapping tag.
  $text .= '<div><a target="_blank" href="/node/' . $nid . '">' . t('Read more')
    . '</a></div>';
  $text .= '</div>';

  // Add subject selected information as well.
  $subject_ids = [];
  $subjects = $wrapper->field_subject->value();
  foreach ($subjects as $subject) {
    $subject_ids[] = $subject->tid;
  }
  $text .= '<div class="data-selected-subject" data-course-subject="'
    . implode(':', $subject_ids) . '"></div>';

  return $text;
}

/**
 * Custom validation to handle form states required fields.
 *
 * @param $element
 * @param $form_state
 */
function _rfc_request_form_validate($element, $form_state) {
  if ($form_state['triggering_element']['#id'] == 'edit-submit'
    || $form_state['triggering_element']['#id'] == 'edit-save'
  ) {
    // Validate on course or course name required.
    if ($form_state['values']['field_rfc_course_not_found'][LANGUAGE_NONE][0]['value']
      == TRUE
    ) {
      if (empty($form_state['values']['field_rfc_new_course_name'][LANGUAGE_NONE][0]['value'])) {
        form_set_error('field_rfc_new_course_name', t('The field %field is required.', ['%field' => $element['field_rfc_new_course_name'][LANGUAGE_NONE]['#title']]));
      }
      if (empty($form_state['values']['field_rfc_new_course_description'][LANGUAGE_NONE][0]['value'])) {
        form_set_error('field_rfc_new_course_description', t('The field %field is required.', ['%field' => $element['field_rfc_new_course_description'][LANGUAGE_NONE]['#title']]));
      }
    }
    else {
      if ((!property_exists($element['#entity'], 'is_new')
        || $element['#entity']->is_new != TRUE)
      ) {
        //dont validate field, if we are editing existing entityform
      }
      else {
        if (empty($form_state['values']['field_rfc_course'][LANGUAGE_NONE][0]['target_id'])) {
          form_set_error('field_rfc_course', t('The field %field is required.', ['%field' => $element['field_rfc_course'][LANGUAGE_NONE]['#title']]));
        }
      }
    }

    $granted_amount
      = $form_state['values']['field_rfc_granted_amount'][LANGUAGE_NONE][0]['value']
      ??
      $element['#entity']->field_rfc_granted_amount[LANGUAGE_NONE][0]['value'];
    if (!empty($form_state['values']['field_rfc_spent_amount'][LANGUAGE_NONE][0]['value'])
      && $granted_amount
      < $form_state['values']['field_rfc_spent_amount'][LANGUAGE_NONE][0]['value']
    ) {
      form_set_error(
        'field_rfc_spent_amount', t(
          '%name: the value may be no greater than %max.', [
            '%name' => $element['field_rfc_spent_amount'][LANGUAGE_NONE]['#title'],
            '%max' => $element['field_rfc_granted_amount'][LANGUAGE_NONE]['#title'],
          ]
        )
      );
    }

    // Validate on no acceptance without amount.
    if (empty($form_state['values']['field_rfc_granted_amount'][LANGUAGE_NONE][0]['value'])
      && $form_state['values']['field_rfc_status'][LANGUAGE_NONE][0]['value'] == 'accepted'
    ) {
      form_set_error('field_rfc_granted_amount', t('The field %field is required to accept an application.', ['%field' => $element['field_rfc_granted_amount'][LANGUAGE_NONE]['#title']]));
    }
  }
}

/**
 * Custom submit to alter submission values.
 *
 * @param $form
 * @param $form_state
 */
function _rfc_request_form_submit($form, &$form_state) {
  // Add 3 hours to date timestamp to ensure correct date (Timezones and summertime)
  // We don't care about the exact time.
  $form_state['values']['field_rfc_date'][LANGUAGE_NONE][0]['value']
    = $form_state['values']['field_rfc_date'][LANGUAGE_NONE][0]['value']
    + 10800;
  // Save provider name in case the provider is deleted in the future
  $provider_id = $form_state['values']['field_rfc_provider'][LANGUAGE_NONE][0]['target_id'];
  $provider = user_load($provider_id);
  $provider_name = $provider->field_profile_name[LANGUAGE_NONE][0]['safe_value'];
  form_set_value($form['field_rfc_provider_name'], [LANGUAGE_NONE => [['value' => $provider_name]]], $form_state);
  // Save course name in case the course is deleted in the future
  if ($form_state['values']['field_rfc_course_not_found'][LANGUAGE_NONE][0]['value'] && !empty($form_state['values']['field_rfc_new_course_name'][LANGUAGE_NONE][0]['value'])) {
    $course_name = $form_state['values']['field_rfc_new_course_name'][LANGUAGE_NONE][0]['value'];
  } else {
    $course_id = $form_state['values']['field_rfc_course'][LANGUAGE_NONE][0]['target_id'];
    $course = node_load($course_id);
    $course_name = $course->title;
  }

  form_set_value($form['field_rfc_course_name'], [LANGUAGE_NONE => [['value' => $course_name]]], $form_state);

}

/**
 * Custom submit to alter submission values.
 *
 * @param $form
 * @param $form_state
 */
function _rfc_request_form_submit_handle_course($form, &$form_state) {
  $entity = $form['#entity'];
  if ((!property_exists($entity, 'is_new') || $entity->is_new != TRUE)
    && empty($form_state['values']['field_rfc_course'][LANGUAGE_NONE][0]['target_id'])
  ) {
    form_set_value($form['field_rfc_course'], $entity->field_rfc_course, $form_state);
  }
}

/**
 * Custom submit to alter submission values.
 *
 * @param $form
 * @param $form_state
 */
function _rfc_request_form_submit_handle_status_change($form, &$form_state) {
  if ($form['#entity']->wrapper()->field_rfc_status->value()
    != 'ready_for_payment'
    && $form_state['values']['field_rfc_status'][LANGUAGE_NONE][0]['value']
    == 'ready_for_payment'
  ) {
    //if status is being changed to "ready_for_payment" set payment date
    form_set_value($form['field_rfc_payment_date'], [LANGUAGE_NONE => [['value' => NULL]]], $form_state);
  }
  if ($form['#entity']->wrapper()->field_rfc_status->value()
    != 'payment_completed'
    && $form_state['values']['field_rfc_status'][LANGUAGE_NONE][0]['value']
    == 'payment_completed'
  ) {
    //if status is being changed to "payment_completed" set payment date
    form_set_value($form['field_rfc_payment_date'], [LANGUAGE_NONE => [['value' => time()]]], $form_state);
  }

}

/**
 * Custom submit to alter submission values.
 *
 * @param $form
 * @param $form_state
 */
function _rfc_request_form_submit_payment_details($form, &$form_state) {

  form_set_value($form['field_rfc_status'], [LANGUAGE_NONE => [['value' => 'awaiting_payment']]], $form_state);

}

/**
 * Get all middle school courses by user.
 *
 * @param $uid
 * @param $current_options
 *
 * @return mixed
 */
function _rfc_get_courses($uid, $current_options) {

  if ($uid === '_none') {
    return $current_options;
  }

  // Set target group pre school and middle school.
  $term_ids = ['148'];

  // Get courses.
  $query = new EntityFieldQuery;
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->propertyCondition('status', 1)
    ->propertyCondition('type', 'course')
    ->propertyCondition('uid', $uid)
    ->fieldCondition('field_target_group', 'tid', $term_ids, 'IN')
    ->execute();

  if (empty($result['node'])) {
    return ['_none' => t('Ingen kurser fundet')];
  }

  $nids = array_keys($result['node']);

  // Compare user courses to the original options.
  foreach ($current_options as $id => $title) {
    if (!empty($nids)) {
      if (!in_array($id, $nids) && $id != '_none') {
        // Remove options if not in query.
        unset($current_options[$id]);
      }
    }
  }
  return $current_options;
}

/**
 * Get all middle school courses by user.
 *
 * @return array
 */
function _rfc_get_hacked_course_options($user, $target_id) {
  $options = [];
  $query = db_select('node', 'n');
  $query->condition('n.type', 'course', '=');
  if (!in_array('editor', $user->roles) && in_array('skole', $user->roles)) {
    $query->condition('n.nid', $target_id, '=');
  }
  $query->fields('n', ['nid', 'title']);
  $query->orderBy('title', 'ASC');
  $result = $query->execute();
  foreach ($result as $record) {
    $options[$record->nid] = $record->title;
  }
  return $options;
}

/**
 * Remove format options from form element.
 *
 * @param $form
 *
 * @return mixed
 */
function _rfc_after(&$form) {
  $form[LANGUAGE_NONE][0]['format']['guidelines']['#access'] = FALSE;
  $form[LANGUAGE_NONE][0]['format']['format']['#access'] = FALSE;
  $form[LANGUAGE_NONE][0]['format']['help']['#access'] = FALSE;
  $form[LANGUAGE_NONE][0]['format']['#theme_wrappers'] = NULL;
  return $form;
}

/**
 * Get all topics for dropdown.
 *
 * @return array
 */
function _rfc_get_subjects() {
  $options = [];
  $vid = taxonomy_vocabulary_machine_name_load('topic')->vid;
  $options_source = taxonomy_get_tree($vid);
  foreach ($options_source as $item) {
    $key = $item->tid;
    $value = $item->name;
    $options[$key] = $value;
  }
  natsort($options);

  return $options;
}
