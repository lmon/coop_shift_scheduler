<?php
require_once ( dirname(__FILE__) . '/MyBaseObject.inc');

/**
 * @file 
 *
 * The job functions etc.
 * To call a method in my class inside a procedural function, I type: 
 * my_module_class::method_name()
 */
 
 /**
 * TODO - move utility functions into here
 */
 class Job extends MyBaseObject{
    public static $keyname = "job";
    public static $displayname = "Job";
    public static $tablename = "coop_shift_scheduler_job";
    public static $admin_full_url = "admin/settings/coop_shift_scheduler/job";
    
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
        $result = db_query("SELECT ".self::$keyname."_id, name FROM {".self::$tablename."} ORDER BY name");
        while ($row = db_fetch_object($result)) {
          $rows[] = array(
            $row->name,
            l(t('edit '.self::$keyname), self::$admin_full_url.'/' . $row->job_id),
          );
        }
        if($rows != NULL){
          $form['table'] = array(
          '#value' => theme('table', $header, $rows)
          );
          $output .= theme('table', $header, $rows);
        }
      }
      $form = self::get_form( array(get_class(),'admin') );
      $output .= $form;
      return $output;
    }

/**
 * Form builder for changing job.
 *
 * @see coop_shift_scheduler_admin_job_validate()
 * @see coop_shift_scheduler_admin_job_submit()
 *
 *
 * @ingroup forms
 * @ingroup admin
 */
  function admin_form() {

    // Takes the 5th argument on the drupal path.
    // e.g. takes 4 from admin/settings/coop_shift_scheduler/job/4
    $object_id = arg(4);
    if ($object_id) {   
    $row = db_fetch_object(db_query("SELECT * FROM {".self::$tablename."} WHERE ".self::$keyname."_id = %d", $object_id));	

      // display the edit slot form
         $form[self::$keyname.'_id'] = array(
          '#type'            => 'value',
          '#value'           => $object_id,
        );    
        $form['name'] = array(
          '#title'         => t(self::$keyname.' name'),
          '#type'          => 'textfield',
          '#size'          => 32,
          '#maxlength'     => 64,
          '#default_value' => $row->name,
        );
        $form['description'] = array(
          '#type'          => 'textfield',
          '#title'         => t('Description'),
          '#default_value' => $row->description,
        );
        $form['repeats'] = array(
          '#title'           => self::$displayname.' Repeats?',
          '#type'          => 'radios',
          '#default_value' =>  $row->repeats,
          '#options'       => array( 
            t('No'),
            t('Yes'),
          ),
        );
        $form['training'] = array(
          '#title'           => self::$displayname.' Training Required?',
          '#type'          => 'radios',
          '#default_value' =>  $row->training,
          '#options'       => array(
            t('No'),
            t('Yes'),
          ),
        );        
       
      $form['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Save '.self::$displayname),
      );
      $form['delete'] = array(
        '#type'            => 'submit',
        '#value'           => t('Delete '.self::$displayname),
      );
    }
    // If you aren't editing a particular slot, assume we're creating a new one
    else {
      $form['name'] = array(
        '#title'           => self::$displayname.' Name',
        '#type'            => 'textfield',
        '#size'            => 52,
        '#maxlength'       => 64,
      );

      $form['description'] = array(
        '#title'           => self::$displayname.' Description',
        '#type'            => 'textfield',
        '#size'            => 52,
        '#maxlength'       => 128,
      );
 
      $form['repeats'] = array(
        '#title'           => self::$displayname.' Repeats?',
        '#type'          => 'radios',
        '#default_value' =>  0,
        '#options'       => array(
          t('No'),
          t('Yes'),
        ),
      );
      $form['training'] = array(
        '#title'           => self::$displayname.' Training Required?',
        '#type'          => 'radios',
        '#default_value' =>  0,
        '#options'       => array(
          t('No'),
          t('Yes'),
        ),
      );
   
      $form['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Add '.self::$displayname),
      );
    }

    $form['#submit'][] = 'coop_shift_scheduler_admin_'.self::$keyname.'_submit';
    $form['#validate'][] = 'coop_shift_scheduler_admin_'.self::$keyname.'_validate';
    return $form;
  }

  /**
 * Validate Committe Add/Edit form
 *
 *
 * @ingroup forms
 * @ingroup admin
**/
function admin_validate($form, $form_state){
  if ($form_state['values']['name']) {
    // Edit slot
    if ($form_state['values']['op'] == t('Save Job')) {
     
      // validate description?? 

      // validate repeats ?

      // validate training req'd ?
      
    }
    // Add slot
    if ($form_state['values']['op'] == t('Add '.self::$displayname)) {
      // check that there isn't already a slot with that name 
       if (db_result(db_query("SELECT COUNT(*) FROM {".self::$tablename."} WHERE name = '%s'", $form_state['values']['name'])) ) {
        form_set_error('name', t('The '.self::$keyname.' name %name already exists. Please choose another '.self::$keyname.' name.', array('%name' => $form_state['values']['name'])));
      }
    }
  }
  else {
    form_set_error('name', t('You must specify a valid '.self::$displayname.' name.'));
  }
}

/**
 * Form submission handler for coop_shift_scheduler_admin_job_form()
 *
 * @see coop_shift_scheduler_admin_job_form()
 * @see coop_shift_scheduler_admin_job_validate()
 *
 *
 * @ingroup admin
 */
function admin_submit($form, $form_state) {
  
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {
  
  	db_query("UPDATE {".self::$tablename."} SET name = '%s', description = '%s', training = '%s', repeats = '%s' WHERE ".self::$keyname."_id = %d", $form_state['values']['name'], $form_state['values']['description'], $form_state['values']['training'], $form_state['values']['repeats'], $form_state['values'][self::$keyname.'_id']);
    	
	   // update eligible roles
     self::set_message(t('The '.self::$displayname.' '.$form_state['values']['name'].'  has been updated.'));
	   $form_state['redirect'] = array(self::$admin_full_url);
  }
  elseif ($form_state['values']['op'] == t('Delete '.self::$displayname)) {

    db_query('DELETE FROM {'.self::$tablename.'} WHERE '.self::$keyname.'_id = %d', $form_state['values'][self::$keyname.'_id']);
    //
    // TODO: cascade effects? 
    //
    $form_state['redirect'] = array(self::$admin_full_url);
    self::set_message(t('The '.self::$displayname.' and *TODO* all of its associated shifts have been deleted.'));
  }
  elseif ($form_state['values']['op'] == t('Add '.self::$displayname)) {
    $new_record = array(
				'name'=> $form_state['values']['name'],
				'description'   => $form_state['values']['description'],
				'training'   => $form_state['values']['training'],
				'repeats'   => $form_state['values']['repeats'],
    );
    self::write(self::$tablename, $new_record);
    //$slot_id = db_query("SELECT ".self::$keyname."_id FROM {".self::$tablename."} WHERE name = :name", array(':name' => $form_state['values']['name']))->fetchField();
    // 
    // TODO Cascade on Add???
    // 
    self::set_message(t('The '.self::$displayname.' '.$form_state['values']['name'].' has been added.'));
  }
}

}//end class
