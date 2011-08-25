<?php
require_once ( dirname(__FILE__) . '/MyBaseObject.inc');

/**
 * @file 
 *
 * The committee functions etc.
 * To call a method in my class inside a procedural function, I type: 
 * my_module_class::method_name()
 */
 
 /**
 * TODO - move utility functions into here
 */
 class Committee extends MyBaseObject{

    public function __construct()
    {
      parent::__construct();
    }
  
  
  function admin() {
      $output = '';
      $rows = array();
      if (!arg(4)) {
        $header = array(
          t('Name'),
          array(
            'data'    => t('Operations'),
            'colspan' => 2,
          ),
        );       

      $result = db_query("SELECT committee_id, name, start_datetime, end_datetime FROM {coop_shift_scheduler_committee} ORDER BY name");
        foreach ($result as $row){
          if ($row->name ==0){
            $edit_default_committee = l(t('edit default committee'), 'admin/settings/coop_shift_scheduler/default_committee/' . $row->committee_id);
          } 
          $rows[] = array(
            $row->name,
            l(t('edit committee'), 'admin/settings/coop_shift_scheduler/committee/' . $row->committee_id),
          );
        }
        
        if($rows != NULL){
          $form['table'] = array(
          '#value' => theme('table', array('header' => $header, 'rows' => $rows))
          );
          $output .= theme('table', array('header' => $header, 'rows' => $rows));
        }
      }
      $form = drupal_get_form('coop_shift_scheduler_admin_committee_form');//returns array
      $output .= drupal_render($form);
      return $output;
    }

/**
 * Form builder for changing committee.
 *
 * @see coop_shift_scheduler_admin_committee_validate()
 * @see coop_shift_scheduler_admin_committee_submit()
 *
 *
 * @ingroup forms
 * @ingroup admin
 */
  function admin_form() {

    // Takes the 5th argument on the drupal path.
    // e.g. takes 4 from admin/settings/coop_shift_scheduler/committee/4
    $slot_id = arg(4);
    if ($slot_id) {   
    $row_query = db_query("SELECT * FROM {coop_shift_scheduler_committee} WHERE committee_id = :slotid", array(':slotid' => $slot_id));	
      foreach ($row_query as $row){
        // display the edit slot form
        $form['name'] = array(
          '#title'         => t('committee name'),
          '#type'          => 'textfield',
          '#size'          => 32,
          '#maxlength'     => 64,
          '#default_value' => $row->name,
        );
        $form['status'] = array(
          '#type'          => 'textfield',
          '#title'         => t('Description'),
          '#default_value' => $row->description,
        );
        
        $instructions = t('La la La la La la La la ');
        $form['instructions'] = array(
          '#type'        => 'item',
          '#title'       => t('Instructions'),
          '#description' => $instructions,
        );
      }
      
    $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Save slot'),
      );
      $form['actions']['delete'] = array(
        '#type'            => 'submit',
        '#value'           => t('Delete slot'),
      );
    }
    // If you aren't editing a particular slot, assume we're creating a new one
    else {
      $form['name'] = array(
        '#title'           => 'Committee Name',
        '#type'            => 'textfield',
        '#size'            => 52,
        '#maxlength'       => 64,
      );

      $form['description'] = array(
        '#title'           => 'Committee Description',
        '#type'            => 'textfield',
        '#size'            => 52,
        '#maxlength'       => 128,
      );
   
    $date = '2008-12-31 00:00:00';
    $format = 'Y-m-d H';
    $form['start_datetime'] = _util_form_date('start_datetime', 'Committee Start Time', $date, $format);
    $form['end_datetime'] = _util_form_date('end_datetime', 'Committee End Time', $date, $format);
    
    $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Add Committee'),
      );
    }

    $form['#submit'][] = 'coop_shift_scheduler_admin_committee_submit';
    $form['#validate'][] = 'coop_shift_scheduler_admin_committee_validate';
    return $form;
  }

  /**
 * Validate Committe Add/Edit form
 *
 *
 * @ingroup forms
 * @ingroup admin
**/
function admin_validate($form, &$form_state){
  if ($form_state['values']['name']) {
    // Edit slot
    if ($form_state['values']['op'] == t('Save Committee')) {
      // check for duplicate names
      if (db_query("SELECT * FROM {coop_shift_scheduler_committee} WHERE name = :name AND committee_id <> :committeeid", array(
	':name' => $form_state['values']['name'], 
	':committeeid' => $form_state['values']['committee_id']))->rowCount() > 0) {
        form_set_error('name', t('The committee name %name already exists. Please choose another committee name.', array('%name' => $form_state['values']['name'])));
      }
     
      // validate description?? 

      // validate start date 

      // validate end date 
      
    }
    // Add slot
    if ($form_state['values']['op'] == t('Add Committee')) {
      // check that there isn't already a slot with that name 
      if (db_query("SELECT * FROM {coop_shift_scheduler_committee} WHERE name = :name", array(':name' => $form_state['values']['name']))->rowCount() > 0) {
        form_set_error('name', t('The committee name %name already exists. Please choose another committee name.', array('%name' => $form_state['values']['name'])));
      }
    }
  }
  else {
    form_set_error('name', t('You must specify a valid Committee name.'));
  }
}

/**
 * Form submission handler for coop_shift_scheduler_admin_committee_form()
 *
 * @see coop_shift_scheduler_admin_committee_form()
 * @see coop_shift_scheduler_admin_committee_validate()
 *
 *
 * @ingroup admin
 */
function admin_submit($form, &$form_state) {

    //
    // TODO need to put the various field form values into one string
    //
    //default values
      $end = '2008-12-30 00:00:00';
      $start = '2008-12-31 00:00:00'; 
    
    if ($form_state['values']['op'] == t('Save committee')) {
  
  	db_update('coop_shift_scheduler_committee')
			->fields(array(
				'name'=> $form_state['values']['name'],
				'description'=> $form_state['values']['description'],
				'start_datetime'   => $start,
				'end_datetime'   => $end,
				)
			 )
			->condition('committee_id', $form_state['values']['committee_id'], '=')
			->execute();
	
	   // update eligible roles
     drupal_set_message(t('The committee has been updated.'));
	   $form_state['redirect'] = array('admin/settings/coop_shift_scheduler');
  }
  elseif ($form_state['values']['op'] == t('Delete committee')) {
  	db_delete('coop_shift_scheduler_committee')
		->condition('committee_id', $form_state['values']['committee_id'], '=')
		->execute();
    //
    // TODO: cascade effects? 
    //
    $form_state['redirect'] = array('admin/settings/coop_shift_scheduler/committee');
    drupal_set_message(t('The committee and *TODO* all of its associated shifts have been deleted.'));
  }
  elseif ($form_state['values']['op'] == t('Add Committee')) {
    $new_record = array(
				'name'=> $form_state['values']['name'],
				'description'   => $form_state['values']['description'],
				'start_datetime'   => $start,
				'end_datetime'   => $end,
    );
    drupal_write_record('coop_shift_scheduler_committee', $new_record);
    $slot_id = db_query("SELECT committee_id FROM {coop_shift_scheduler_committee} WHERE name = :name", array(':name' => $form_state['values']['name']))->fetchField();
    // 
    // TODO Cascade on Add???
    // 
    drupal_set_message(t('The committee has been added.'));
  }
}

}//end class
