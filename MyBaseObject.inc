<?php

/**
 * @file 
 *
 * The Base Object ( common ) functions etc.
 * This method should not be called directly. Only extended 
 */
 
 /**
 * TODO - move utility functions into here
 */
 class MyBaseObject{

    public function __construct()
    {
      parent::__construct();
    }

    public static function class_test()
    {
        $result = 'The Class exists: '.get_class();
        return $result;
    }
    
    /* 
    * wrapper for drupal functions
    * attempt to abstract the Data Objects somewhat
    * @see drupal_get_form
    * 
    */
    public function get_form($args){
      $objname = $args[0];
      $modename = $args[1];
      $module = 'coop_shift_scheduler';
      return drupal_get_form($module . '_' . $modename . '_' . strtolower($objname) . '_form' );
    }
    /* 
    * wrapper for drupal functions
    * attempt to abstract the Data Objects somewhat
    * @see drupal_render
    * 
    */
    public function render($form){
      return drupal_render($form);
    }
    /* 
    * wrapper for drupal functions
    * attempt to abstract the Data Objects somewhat
    * @see drupal_set_message
    * 
    */
    public function set_message($msg){
       drupal_set_message($msg);
    }
    /* 
    * wrapper for drupal functions
    * attempt to abstract the Data Objects somewhat
    * @see drupal_set_message
    * 
    */
    public function write($tablename, $new_record){
       drupal_write_record($tablename, $new_record);
    }
    /*
    *
    */
    public function get_job_records($id = false){
       // job ids
      $where = "";
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_job} WHERE job_id = :id ORDER BY name",array(':id'=> $id) );
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_job} ORDER BY name" );
      }
      foreach($result as $options_row){
        $options[$options_row->job_id] = $options_row->name;
      }
      return $options;
    }
    /*
    *
    */
    public function get_user_records($id = false){
      // user ids
      $where = "";
      if(isset($id)){ $where = " WHERE uid = $id ";}
      $result = db_query("SELECT uid, name FROM {users} ORDER BY name" );
      foreach($result as $options_row){
        $options[$options_row->uid] = $options_row->name;
      }
      return $options;
    }
     /*
    *
    */
    public function get_shift_records($id = false){
       // shift ids
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_shift} WHERE shift_id = :id ORDER BY name",array(':id'=> $id) );
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_shift} ORDER BY name" );
      }
      foreach($result as $options_row){
        $options[$options_row->shift_id] = $options_row->name;
      }
      return $options;
    }
     /*
    *
    */
    public function get_slot_records($id = false){
      // slot ids
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_slot} WHERE slot_id = :id ORDER BY slot_id",array(':id'=> $id) );
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_slot} ORDER BY slot_id" );
      }
      foreach($result as $options_row){
        $options[$options_row->slot_id] = $options_row;
      }
      return $options;
    }
    public function get_fullslot_records($id = false){
      // slots w/assoc values 
      $result = db_query("SELECT slot.*, shift.name as shiftname, users.uid, users.name as username 
      FROM coop_shift_scheduler_slot as slot, coop_shift_scheduler_shift as shift , users 
      WHERE slot.shift_id = shift.shift_id 
      AND users.uid = slot.worker_user_id" );
      foreach($result as $row){
        //print_r($row);
        $options[$row->slot_id] = $row;
      }
       return $options;
    }
    
    
    /*
    */
    


}
