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
        $result = db_query("SELECT ".self::$keyname."_id, name FROM {".self::$tablename."} ORDER BY name");
//drupal v7.x      foreach ($result as $row){
       while ($row = db_fetch_object($result)) {
          $rows[] = array(
            $row->name,
            l(t('edit '.self::$keyname), self::$admin_full_url ."/". $row->committee_id),
          );
        }
        if($rows != NULL){
          $form['table'] = array(
//drupal v7.x      '#value' => theme('table', array('header' => $header, 'rows' => $rows))
          '#value' => theme('table', $header, $rows)
          );
//drupal v7.x      $output .= theme('table', array('header' => $header, 'rows' => $rows));
          $output .= theme('table', $header, $rows);
        }
      }
      $form = self::get_form( array(get_class(),'admin') );
//drupal v7.x   $output .= self::render($form); 
        $output .= $form;
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
//drupal v7.x     $row_query = db_query("SELECT * FROM {".self::$tablename."} WHERE ".self::$keyname."_id = :".self::$keyname."id", array(':'.self::$keyname.'id' => $object_id));	
      $row = db_fetch_object(db_query("SELECT * FROM {".self::$tablename."} WHERE ".self::$keyname."_id = %d", $object_id));	
//drupal v7.x     foreach ($row_query as $row){
        // display the edit slot form
        
        $instructions = t('La la La la La la La la ');
        $form['instructions'] = array(
          '#type'        => 'item',
          '#title'       => t('Instructions'),
          '#description' => $instructions,
        );

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
 
    $form['start_datetime'] = _util_form_date('start_datetime', 'Committee Start Time', $row->start_datetime, $format);
    $form['end_datetime'] = _util_form_date('end_datetime', 'Committee End Time', $row->end_datetime, $format);
 //drupal v7.x          }
      
//drupal v7.x    $form['actions'] = array('#type' => 'actions');
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
      $instructions = t('La la La la La la La la ');
      $form['instructions'] = array(
          '#type'        => 'item',
          '#title'       => t('Instructions'),
          '#description' => $instructions,
      );
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
     
//drupal v7.x    $form['actions'] = array('#type' => 'actions');
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
function admin_validate($form, &$form_state){
  if ($form_state['values']['name']) {
    // Edit slot
    if ($form_state['values']['op'] == t('Save '.self::$displayname)) {
      // check for duplicate names
//drupal v7.x updated:
      if (db_result(db_query("SELECT COUNT(*) FROM {".self::$tablename."} WHERE name = '%s' AND ".self::$keyname."_id <> %d", $form_state['values']['name'], $form_state['values'][self::$keyname.'_id'] ) )) {
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
 //drupalv7.x     if (db_query("SELECT * FROM {".self::$tablename."} WHERE name = :name", array(':name' => $form_state['values']['name'])) )
      if (db_result(db_query("SELECT COUNT(*) FROM {".self::$tablename."} WHERE name = '%s'", $form_state['values']['name']))) {
        form_set_error('name', t('The '.self::$displayname.' name %name already exists. Please choose another name.', array('%name' => $form_state['values']['name'])));
      }
      }
  }
  else {
    form_set_error('name', t('You must specify a valid '.self::$displayname.' name.'));
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
  
//drupalv7.x
  	/*db_update(self::$tablename)
			->fields(array(
				'name'=> $form_state['values']['name'],
				'description'=> $form_state['values']['description'],
				'start_datetime'   => $start,
				'end_datetime'   => $end,
				)
			 )
			->condition(self::$keyname.'_id', $form_state['values'][self::$keyname.'_id'], '=')
			->execute();
    */
 db_query("UPDATE {".self::$tablename."} SET name = '%s', description = '%s', start_datetime = '%s', end_datetime = '%s' WHERE ".self::$keyname."_id = %d", $form_state['values']['name'], $form_state['values']['description'], $start, $end, $form_state['values'][self::$keyname.'_id']);
 
	   //
     self::set_message(t('The '.self::$displayname.' has been updated.'));
	   $form_state['redirect'] = array(self::$admin_full_url);
  }
  elseif ($form_state['values']['op'] == t('Delete '.self::$displayname)) {
//drupal v7.x
  	/*db_delete(self::$tablename)
		->condition(self::$keyname.'_id', $form_state['values'][self::$keyname.'_id'], '=')
		->execute();
    */
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
