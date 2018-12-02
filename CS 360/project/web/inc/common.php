<?php 
if(!defined('ROOTPATH')){
	define('ROOTPATH', './');
}
define('BACKENDPATH', ROOTPATH.'backend/');
define('APIPATH', BACKENDPATH.'api.php');

require_once('config.php');

//include all objects
foreach(glob(ROOTPATH.'inc/objects/*.php') as $file){
    require_once($file);
}

$conn = mysqli_connect($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
/* check connection */
if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
require_once('template.php');

