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


}
