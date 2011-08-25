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
        foreach ($result as $row){
          $rows[] = array(
            $row->name,
            l(t('edit '.self::$keyname), self::$admin_full_url.'/' . $row->job_id),
          );
        }
        
        if($rows != NULL){
          $form['table'] = array(
          '#value' => theme('table', array('header' => $header, 'rows' => $rows))
          );
          $output .= theme('table', array('header' => $header, 'rows' => $rows));
        }
      }
      $form = drupal_get_form('coop_shift_scheduler_admin_'.self::$keyname.'_form');//returns array
      $output .= drupal_render($form);
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
    $row_query = db_query("SELECT * FROM {".self::$tablename."} WHERE ".self::$keyname."_id = :".self::$keyname."id", array(':'.self::$keyname.'id' => $object_id));	
      foreach ($row_query as $row){
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
          '#default_value' =>  1,
          '#options'       => array(
            t('Yes'),
            t('No'),
          ),
        );
        $form['training'] = array(
          '#title'           => self::$displayname.' Training Required?',
          '#type'          => 'radios',
          '#default_value' =>  1,
          '#options'       => array(
            t('Yes'),
            t('No'),
          ),
        );        
       }
      
    $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Save '.self::$displayname),
      );
      $form['actions']['delete'] = array(
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
        '#default_value' =>  1,
        '#options'       => array(
          t('Yes'),
          t('No'),
        ),
      );
      $form['training'] = array(
        '#title'           => self::$displayname.' Training Required?',
        '#type'          => 'radios',
        '#default_value' =>  1,
        '#options'       => array(
          t('Yes'),
          t('No'),
        ),
      );
   
        
    $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
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
      if (db_query("SELECT * FROM {".self::$tablename."} WHERE name = :name", array(':name' => $form_state['values']['name']))->rowCount() > 0) {
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
  
  	db_update(self::$tablename)
			->fields(array(
				'name'=> $form_state['values']['name'],
				'description'=> $form_state['values']['description'],
				'training'   => $form_state['values']['training'],
				'repeats'   => $form_state['values']['repeats'],
				)
			 )
			->condition(self::$keyname.'_id', $form_state['values'][self::$keyname.'_id'], '=')
			->execute();
	
	   // update eligible roles
     drupal_set_message(t('The '.self::$displayname.' has been updated.'));
	   $form_state['redirect'] = array(self::$admin_full_url);
  }
  elseif ($form_state['values']['op'] == t('Delete '.self::$displayname)) {
  	db_delete(self::$tablename)
		->condition(self::$keyname.'_id', $form_state['values'][self::$keyname.'_id'], '=')
		->execute();
    //
    // TODO: cascade effects? 
    //
    $form_state['redirect'] = array(self::$admin_full_url);
    drupal_set_message(t('The '.self::$displayname.' and *TODO* all of its associated shifts have been deleted.'));
  }
  elseif ($form_state['values']['op'] == t('Add '.self::$displayname)) {
    $new_record = array(
				'name'=> $form_state['values']['name'],
				'description'   => $form_state['values']['description'],
				'training'   => $form_state['values']['training'],
				'repeats'   => $form_state['values']['repeats'],
    );
    drupal_write_record(self::$tablename, $new_record);
    $slot_id = db_query("SELECT ".self::$keyname."_id FROM {".self::$tablename."} WHERE name = :name", array(':name' => $form_state['values']['name']))->fetchField();
    // 
    // TODO Cascade on Add???
    // 
    drupal_set_message(t('The '.self::$displayname.' has been added.'));
  }
}

}//end class
