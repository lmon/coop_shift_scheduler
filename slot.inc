<?php
require_once ( dirname(__FILE__) . '/MyBaseObject.inc');

/**
 * @file 
 *
 * The slot functions etc.
 * To call a method in my class inside a procedural function, I type: 
 * my_module_class::method_name()
 */
 
 /**
 * TODO - move utility functions into here
 */
 class Slot extends MyBaseObject{
    public static $keyname = "slot";
    public static $displayname = "Slot";
    public static $tablename = "coop_shift_scheduler_slot";
    public static $admin_full_url = "admin/settings/coop_shift_scheduler/slot";
    
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
      $result = db_query("SELECT ".self::$keyname."_id, shift_id, start_datetime, end_datetime FROM {".self::$tablename."} ORDER BY slot_id");
        foreach ($result as $row){
          $list = self::get_shift_records($row->shift_id);
          $rows[] = array(
            'Slot for \''. $list[$row->shift_id] .'\' on ('.$row->start_datetime.' '.$row->end_datetime.')',
            l(t('edit '.self::$keyname), self::$admin_full_url ."/". $row->slot_id),
          );
        }
        
        if($rows != NULL){
          $form['table'] = array(
          '#value' => theme('table', array('header' => $header, 'rows' => $rows))
          );
          $output .= theme('table', array('header' => $header, 'rows' => $rows));
        }
      }
      $form = self::get_form( array(get_class(),'admin') );
      $output .= self::render($form); 
      return $output;
    }

/**
 * Form builder for changing slot.
 *
 * @see coop_shift_scheduler_admin_slot_validate()
 * @see coop_shift_scheduler_admin_slot_submit()
 *
 *
 * @ingroup forms
 * @ingroup admin
 */
  function admin_form() {

    // shift ids
    $shift_id_options = parent::get_shift_records();
    // user ids
    $worker_id_options = parent::get_user_records();
     
     //the default date value
    $date = '2011-08-09 00:00:00';
    $format = 'Y-m-d h:i A';

    // Takes the 5th argument on the drupal path.
    // e.g. takes 4 from admin/settings/coop_shift_scheduler/slot/4
    $object_id = arg(4);
    if ($object_id) {   
      $row_query = db_query("SELECT * FROM {".self::$tablename."} WHERE ".self::$keyname."_id = :".self::$keyname."id", array(':'.self::$keyname.'id' => $object_id));	
    
      foreach ($row_query as $row){
        // display the edit slot form
         $form[self::$keyname.'_id'] = array(
          '#type'            => 'value',
          '#value'           => $object_id,
        );    
        //'slot_id' , 'shift_id', 'start_datetime', 'end_datetime', 'worker_user_id' , 'isWorked' ,
        $form['shift_id'] = array(
          '#type'          => 'select',
          '#title'         => t('Shift'),
          '#default_value' => $row->shift_id,
          '#options'       => $shift_id_options,
        ); 
        $form['start_datetime'] = _util_form_date('start_datetime', 'Slot Start Time', $row->start_datetime, $format);
        $form['end_datetime'] = _util_form_date('end_datetime', 'Slot End Time', $row->end_datetime, $format);
    
         $form['worker_user_id'] = array(
          '#type'          => 'select',
          '#title'         => t('Worker User Id'),
          '#default_value' => $row->worker_user_id,
          '#options'       => $worker_id_options,

        );
        $form['isWorked'] = array(
          '#title'         => t('Is the Slot worked'),
          '#type'          => 'radios',
          '#default_value' =>  $row->isworked,
          '#options'       => array(
            t('No'),
            t('Yes'),
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
        
        //'slot_id' , 'shift_id', 'start_datetime', 'end_datetime', 'worker_user_id' , 'isWorked' ,
        $form['shift_id'] = array(
          '#type'          => 'textfield',
          '#title'         => t('Name'),
          '#default_value' => "",
        );
        $form['start_datetime'] = _util_form_date('start_datetime', 'Slot Start Time', $date, $format);
        $form['end_datetime'] = _util_form_date('end_datetime', 'Slot End Time', $date, $format);
    
         $form['worker_user_id'] = array(
          '#type'          => 'select',
          '#title'         => t('Worker User Id'),
          '#default_value' => "",
          '#options'       => $worker_id_options,

        );
        $form['isWorked'] = array(
          '#title'         => t('Is the Slot worked'),
          '#type'          => 'radios',
          '#default_value' =>  0,
          '#options'       => array(
            t('No'),
            t('Yes'),
            ),
        );
        $form['shift_id'] = array(
          '#type'          => 'select',
          '#title'         => t('Job'),
          '#default_value' => "",
          '#options'       => $shift_id_options,
        );  
      $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Add '.self::$displayname),
      );
    }

    $form['#submit'][] = 'coop_shift_scheduler_admin_slot_submit';
    $form['#validate'][] = 'coop_shift_scheduler_admin_slot_validate';
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
   
    // Edit slot
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {       
     
      // validate job?? 

      // validate isworked? 

      // validate start_datetime 

      // validate start_datetime 

      // validate worker_user_id 
      
    }
    // Add slot
    if ($form_state['values']['op'] == t('Add '.self::$displayname)) {
      // validation?
    }
   
}

/**
 * Form submission handler for coop_shift_scheduler_admin_slot_form()
 *
 * @see coop_shift_scheduler_admin_slot_form()
 * @see coop_shift_scheduler_admin_slot_validate()
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
    if(isset($form_state['values']['start_datetime'])){
      $start = $form_state['values']['start_datetime'];
    }
    if(isset($form_state['values']['end_datetime'])){
      $end = $form_state['values']['end_datetime'];
    }
    
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {
  
  	db_update(self::$tablename)
			->fields(array(
				'shift_id'  =>    $form_state['values']['shift_id'],
        'start_datetime'   => $start,
				'end_datetime'   => $end,
        'isWorked'   => $form_state['values']['isWorked'],
				'worker_user_id'   => $form_state['values']['worker_user_id'],
				)
			 )
			->condition(self::$keyname.'_id', $form_state['values'][self::$keyname.'_id'], '=')
			->execute();
	
	   // update eligible roles
     self::set_message(t('The '.self::$displayname.' has been updated.'));
     //drupal_set_message(t('The '.self::$displayname.' has been updated.'));
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
    self::set_message(t('The '.self::$displayname.' has been deleted.')); 
  }
  elseif ($form_state['values']['op'] == t('Add '.self::$displayname)) {
    $new_record = array( 
        'shift_id'  =>    $form_state['values']['shift_id'],
        'start_datetime'   => $start,
				'end_datetime'   => $end,
        'isWorked'   => $form_state['values']['isWorked'],
				'worker_user_id'   => $form_state['values']['worker_user_id'],
    );
    self::write(self::$tablename, $new_record);
     
    // 
    // TODO Cascade on Add???
    // 
    self::set_message(t('The '.self::$displayname.'  has been added.'));
  }else{
  
  }
}

}//end class
