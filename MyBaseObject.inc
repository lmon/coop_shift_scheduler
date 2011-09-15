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

    public static $member_role_id = null;
    
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
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_job} WHERE job_id = %d ORDER BY name", $id  );
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_job} ORDER BY name");
      }
       while ($row = db_fetch_object($result)) {
        $options[$row->job_id] = $row->name;
      }
      return $options;
    }
    public function get_role_id(){
      if(self::$member_role_id != null){
        return self::$member_role_id;
      }
      $result = db_query("SELECT ur.rid as roleid FROM users_roles ur, role r 
      WHERE ur.rid = r.rid");
       while ($row = db_fetch_object($result)) {
        self::$member_role_id = $row->roleid;
      }
      return self::$member_role_id;
    }  
    /*
    *
    */
    public function get_user_records($id = false){
      // user ids
      if( $id != false ){ 
        $result = db_query("SELECT uid, name FROM {users} WHERE uid = %d ORDER BY name", $id);
      }else{
          $result = db_query("SELECT u.uid, u.name  
            FROM users u, users_roles ur 
            WHERE u.uid = ur.uid
            AND ur.rid = ".self::get_role_id()."
            ORDER BY name" );
      }
       while ($row = db_fetch_object($result)) {
        $options[$row->uid] = $row->name;
      }
      return $options;
    }
     /*
    *
    */
    public function get_shift_records($id = false){
       // shift ids
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_shift} WHERE shift_id = %d ORDER BY name", $id);
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_shift} ORDER BY name" );
      }
      while ($row = db_fetch_object($result)) {
        $options[$row->shift_id] = $row->name;
      }
      return $options;
    }
     /*
    *
    */
    public function get_slot_records($id = false){
      // slot ids
      if( $id != false ){ 
        $result = db_query("SELECT * FROM {coop_shift_scheduler_slot} WHERE slot_id = %d ORDER BY slot_id", $id);
      }else{
        $result = db_query("SELECT * FROM {coop_shift_scheduler_slot} ORDER BY slot_id" );
      }
      while ($row = db_fetch_object($result)) {
        $options[$row->slot_id] = $row;
      }
      return $options;
    }
    public function get_fullslot_records($id = false){
      // slots w/assoc values 
      $result = db_query("SELECT slot.*, shift.name as shiftname, users.uid, users.name as username 
      FROM coop_shift_scheduler_slot as slot, coop_shift_scheduler_shift as shift , users 
      WHERE slot.shift_id = shift.shift_id 
      AND users.uid = slot.worker_user_id" );
       while ($row = db_fetch_object($result)) {
        //print_r($row);
        $options[$row->slot_id] = $row;
      }
       return $options;
    }
    
    
    /*
    */
    


}
