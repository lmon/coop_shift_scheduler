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
    */
    


}
