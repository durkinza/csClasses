<?php 
if(!defined('ROOTPATH')){
	define('ROOTPATH', './');
}
require_once('config.php');

$conn = mysqli_connect($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME);
/* check connection */
if (mysqli_connect_errno()) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
require_once('template.php');

