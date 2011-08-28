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
    public static $keyname = "committee";
    public static $displayname = "Committee";
    public static $tablename = "coop_shift_scheduler_committee";
    public static $admin_full_url = "admin/settings/coop_shift_scheduler/committee";
    
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

      $result = db_query("SELECT ".self::$keyname."_id, name, start_datetime, end_datetime FROM {".self::$tablename."} ORDER BY name");
        foreach ($result as $row){
          $rows[] = array(
            $row->name,
            l(t('edit '.self::$keyname), self::$admin_full_url ."/". $row->committee_id),
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
    
    //the default date value
    $date = '2011-08-09 00:00:00';
    $format = 'Y-m-d h:i A';

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
          '#title'         => t(self::$displayname.' name'),
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
/*        $form['start_datetime'] = array(
          '#type'          => 'textfield',
          '#title'         => t('start_datetime'),
          '#default_value' => $row->start_datetime,
        );
        $form['end_datetime'] = array(
          '#type'          => 'textfield',
          '#title'         => t('start_datetime'),
          '#default_value' => $row->start_datetime,
        );
*/
    $form['start_datetime'] = _util_form_date('start_datetime', 'Committee Start Time', $row->start_datetime, $format);
    $form['end_datetime'] = _util_form_date('end_datetime', 'Committee End Time', $row->end_datetime, $format);
        
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
   
    $form['start_datetime'] = _util_form_date('start_datetime', 'Committee Start Time', $date, $format);
    $form['end_datetime'] = _util_form_date('end_datetime', 'Committee End Time', $date, $format);
     
    $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type'            => 'submit',
        '#value'           => t('Add '.self::$displayname),
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
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {
      // check for duplicate names
      if (db_query("SELECT * FROM {".self::$tablename."} WHERE name = :name AND ".self::$keyname."_id <> :".self::$keyname."id", array(
	':name' => $form_state['values']['name'], 
	':'.self::$keyname.'id' => $form_state['values'][self::$keyname.'_id']))->rowCount() > 0) {
        form_set_error('name', t('The '.self::$displayname.' name %name already exists. Please choose another name.', array('%name' => $form_state['values']['name'])));
      }
     
      //TODO validate description?? 

      //TODO validate start date 

      //TODO validate end date 
      
      //TODO validate start is less than end date ?
      
    }
    // Add Item
    if ($form_state['values']['op'] == t('Add '.self::$displayname)) {
      // check that there isn't already a slot with that name 
      if (db_query("SELECT * FROM {".self::$tablename."} WHERE name = :name", array(':name' => $form_state['values']['name']))->rowCount() > 0) {
        form_set_error('name', t('The '.self::$displayname.' name %name already exists. Please choose another name.', array('%name' => $form_state['values']['name'])));
      }
    }
  }
  else {
    form_set_error('name', t('You must specify a valid '.self::$displayname.'name.'));
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
    if(isset($form_state['values']['start_datetime'])){
      $start = $form_state['values']['start_datetime'];
    }
    if(isset($form_state['values']['end_datetime'])){
      $end = $form_state['values']['end_datetime'];
    }
//    print "using start $start <br>";
//    print "using end $end ";
    
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {
  
  	db_update(self::$tablename)
			->fields(array(
				'name'=> $form_state['values']['name'],
				'description'=> $form_state['values']['description'],
				'start_datetime'   => $start,
				'end_datetime'   => $end,
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
    self::set_message(t('The '.self::$displayname.' and *TODO* all of its associated shifts have been deleted.'));
    //drupal_set_message(t('The '.self::$displayname.' and *TODO* all of its associated shifts have been deleted.'));
  }
  elseif ($form_state['values']['op'] == t('Add '.self::$displayname)) {
    $new_record = array(
				'name'=> $form_state['values']['name'],
				'description'   => $form_state['values']['description'],
				'start_datetime'   => $start,
				'end_datetime'   => $end,
    );
    self::write(self::$tablename, $new_record);
    // 
    // TODO Cascade on Add???
    // 
    self::set_message(t('The '.self::$displayname.' has been added.'));
  }
}

}//end class
